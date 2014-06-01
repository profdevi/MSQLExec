/*
Copyright (C) 2011-2014, Comine.com ( comine.com@gmail.com )
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
- The the names of the contributors of this project may not be used to 
  endorse or promote products derived from this software without specific 
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
`AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


//v2.7  copyright Comine.com 20140531S1820
#ifndef MODBCDatabase_h
#define MODBCDatabase_h

/////////////////////////////////////////////////////
#include "MStdLib.h"
#include <windows.h>
#include <sqlext.h>
#include "MString.h"
#include "MErrorInfo.h"

/////////////////////////////////////////////////////
class MODBCEnvironment : public virtual MErrorInfo
	{
	SQLHANDLE hEnvironment;
	

	///////////////////////////////
	void ClearObject(void);
	void SetError(void);

	////////////////////////////////
	public:
	MODBCEnvironment(void);
	virtual ~MODBCEnvironment(void);
	bool Create(void);
	bool Destroy(void);
	SQLHANDLE GetHandle(void ); // =
	bool RollBack(void);
	bool Commit(void);
	};

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
class MODBCConnection:public virtual MErrorInfo
	{
	MODBCEnvironment *mEnviron;
	SQLHANDLE hConnection;
	MString mConnectionString;
	////////////////////////////////////////
	protected:
	void SetError(void);

	////////////////////////////////////////
	public:
	MODBCConnection(void);
	virtual ~MODBCConnection(void);
	bool Create(MODBCEnvironment &env);
	bool Destroy(void);
	SQLHANDLE GetHandle(void);
	SQLHANDLE GetEnvHandle(void);
	bool Open(const char *constr,HWND hWnd=NULL);
	bool Close(void);
	bool AutoCommit(bool opt); 
	bool Commit(void);
	bool RollBack(void);
	const char *GetConnectionString(void);
	};


////////////////////////////////////////////////////
////////////////////////////////////////////////////
class MODBCStatement:public virtual MErrorInfo
	{
	SQLHANDLE hStatement;

	///////////////////////////////////////////
	void SetError(SQLHANDLE hStmt);

	///////////////////////////////////////////
	public:
	SQLHANDLE GetHandle(void);
	MODBCStatement(void);
	virtual ~MODBCStatement(void);
	bool Create(MODBCConnection &con);
	virtual bool Destroy(void);
	bool ExecDirect(const char *sql);
	bool Execute(void);
	bool Prepare(const char *sql);		// "insert into TCar(CName,CType,CAge) values(?,?,?)
	bool Cancel(void);
	bool Fetch(void);  // Fetch a row
	bool BindCol(int index,int &variable);
	bool BindCol(int index,float &variable);
	bool BindCol(int index,double &variable);
	bool BindCol(int index,char *lpszBuffer,int bufsize,int &retlen);
	bool BindParameter(int index,int &variable);
	bool BindParameter(int index,float &variable);
	bool BindParameter(int index,double &variable);
	bool BindParameter(int index,char *lpszBuffer,int bufsize);
	bool BindParameter(int index,int bufsize,char *memo);
	bool NativeSql(char *sql,char *buffer,int buflen,int *neededlen=0);
	int NumResultCols(void); //=0 if error
		// *requiredbufsize==0 if col value is null
	bool GetData(int index,char *col,int collen,int *requiredbufsize=0); 
	bool ParamData(void *bufferaddr);
	bool PutData(void *data,int len=SQL_NTS);
	bool Tables(const char *NamePattern="%",const char *type="TABLE");// try "VIEW",...
	bool Columns(const char *tablename,const char *colpattern="%");
	bool PrimaryKeys(const char *tablename);
	bool Procedures(const char *NamePattern="%");
	bool ProcedureColumns(const char *ProcName,const char *ColPattern="%");
	bool ForeignKeys(const char *tablename);
	bool DescribeCol(int colindex,char *data,int datalen,int &colsize);
	};

#endif // MODBCDatabase_h

