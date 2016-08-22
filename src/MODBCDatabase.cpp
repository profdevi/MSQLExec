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


//v2.8  copyright Comine.com 20160822M0124
#include "MStdLib.h"
#include <windows.h>
#include <sqlext.h>
#include "MBuffer.h"
#include "MString.h"
#include "MErrorInfo.h"
#include "MODBCDatabase.h"

////////////////////////////////////////////////////////
// Link to SQL
#if (defined(MSTDLIB_OS_WINDOWS) )
	#pragma comment(lib,"odbc32.lib")
	#pragma comment(lib,"odbccp32.lib")
#endif

///////////////////////////////////////////////////////////////////////////
static const int MMaxErrorBuffer=500;
static const int SMAXCONNECTIONSTRING=1000;

//************************************************************************
//*     MODBCEnvironment  
//************************************************************************
void MODBCEnvironment::ClearObject(void)
	{
	hEnvironment=SQL_NULL_HANDLE;
	}


void MODBCEnvironment::SetError(void)
	{
	SQLRETURN ret;
	SQLCHAR state[7];
	SQLINTEGER nat;SQLSMALLINT len;
	MBuffer tErrorBuf(MMaxErrorBuffer);  *tErrorBuf=0;
	
	if(hEnvironment==SQL_NULL_HANDLE)
		{
		MErrorInfo::SetError("MEnvironment::ThrowError hEnvironment is SQL_NULL_HANDLE");
		return;
		}

	ret=SQLGetDiagRecA(SQL_HANDLE_ENV,hEnvironment,1,
			state,&nat,(SQLCHAR*)tErrorBuf.GetBuffer(),(SQLSMALLINT)MMaxErrorBuffer,&len);
		
	if(SQL_SUCCEEDED(ret))
		{ MErrorInfo::SetError(tErrorBuf); return ;}
	
	MErrorInfo::SetError("MEnvironment::ThrowError Unexpected Error");
	}


/////////////////////////////////
MODBCEnvironment::MODBCEnvironment(void)
	{
	ClearObject();
	}  


//////////////////////////////////
MODBCEnvironment::~MODBCEnvironment(void)
	{  Destroy(); }


///////////////////////////////////
bool MODBCEnvironment::Create(void)
	{
	Destroy();

	SQLRETURN ret=SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnvironment);

	if(!SQL_SUCCEEDED(ret))
		{
		MErrorInfo::SetError("Alloc ODBC Env. Fail");
		return false;
		}

	ret=SQLSetEnvAttr(hEnvironment,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,0);
	if(ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
		MErrorInfo::SetError("Setting ODBC Version Failed");
		return false; 
		}

	return true;
	}


////////////////////////////////////
bool MODBCEnvironment::Destroy(void)
	{
	if(hEnvironment==SQL_NULL_HANDLE) { return false; }
	SQLFreeHandle(SQL_HANDLE_ENV,hEnvironment);
	ClearObject();
	return true;
	}


///////////////////////////////////
SQLHANDLE MODBCEnvironment::GetHandle(void)
	{  return hEnvironment; }


///////////////////////////////////
bool MODBCEnvironment::RollBack(void)
	{
	if(hEnvironment==SQL_NULL_HANDLE)
		{
		MErrorInfo::SetError("MEnvironment::RollBack() hEnvironment is SQL_NULL_HANDLE"); 
		return false; 
		}

	SQLRETURN ret=SQLEndTran(SQL_HANDLE_ENV,hEnvironment,SQL_ROLLBACK);
	if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
		SetError();
		return false;
		}
	
	return true;
	}


//////////////////////////////////
bool MODBCEnvironment::Commit(void)
	{
	if(hEnvironment==SQL_NULL_HANDLE)
		{
		MErrorInfo::SetError("MEnvironment::Commit() hEnvironment is SQL_NULL_HANDLE");
		return false;
		}

	SQLRETURN ret=SQLEndTran(SQL_HANDLE_ENV,hEnvironment,SQL_COMMIT);
	if(!SQL_SUCCEEDED(ret))
		{
		SetError();  
		return false;
		}

	return true;
	}


//************************************************************************
//*     MODBCConnection
//************************************************************************
void MODBCConnection::SetError(void)
	{
	SQLRETURN ret;
	SQLCHAR state[7];
	SQLINTEGER nat;
	SQLSMALLINT len;
	MBuffer tErrorBuf(MMaxErrorBuffer);  *tErrorBuf=0;
	
	if(mEnviron->GetHandle()==SQL_NULL_HANDLE)
		{
		MErrorInfo::SetError("MConnection::SetError() Environment not set");  
		return;
		}

	if(hConnection==SQL_NULL_HANDLE)
		{
		MErrorInfo::SetError("MConnection::SetError() Connection not set");  
		return;
		}
	
	ret=SQLGetDiagRecA(SQL_HANDLE_DBC,hConnection,1,
			state,&nat,(SQLCHAR*)tErrorBuf.GetBuffer(),(SQLSMALLINT)MMaxErrorBuffer,&len);
	
	if(SQL_SUCCEEDED(ret))
		{ 
		MErrorInfo::SetError(tErrorBuf);
		return;
		}
	
	MErrorInfo::SetError("MEnvironment::SetError() Unexpected Error");
	}


/////////////////////////////////
MODBCConnection::MODBCConnection(void)
	{
	mEnviron=SQL_NULL_HANDLE;
	hConnection=SQL_NULL_HANDLE;
	mConnectionString="";
	}


/////////////////////////////////	
MODBCConnection::~MODBCConnection(void) 
	{  
	Destroy();
	}


/////////////////////////////////
bool MODBCConnection::Create(MODBCEnvironment &env) 
	{
	Destroy();
	mEnviron=&env;
	
	SQLHANDLE hCon=SQL_NULL_HANDLE;
	SQLRETURN ret=SQLAllocHandle(SQL_HANDLE_DBC,mEnviron->GetHandle(),&hCon);
	if(!SQL_SUCCEEDED(ret))
		{
		MErrorInfo::SetError("Unable to alloc Connection handle");
		return false; 
		}

	hConnection=hCon;
	return true;
	}


/////////////////////////////////
bool MODBCConnection::Destroy(void)
	{
	if(hConnection==SQL_NULL_HANDLE) { return false; }
	Close();
	SQLFreeHandle(SQL_HANDLE_DBC,hConnection);
	hConnection=SQL_NULL_HANDLE;
	mEnviron=NULL;
	return true;
	}


////////////////////////////////
SQLHANDLE MODBCConnection::GetHandle(void)
	{
	return hConnection;
	}


////////////////////////////////
SQLHANDLE MODBCConnection::GetEnvHandle(void)
	{
	if(mEnviron==NULL)
		{
		return SQL_NULL_HANDLE;
		}

	return mEnviron->GetHandle();
	}


/////////////////////////////////
bool MODBCConnection::Open(const char *constring,HWND hWnd)
	{
	SQLSMALLINT len=0;  
	MBuffer connect(SMAXCONNECTIONSTRING);  *connect=0;
	int prompt=SQL_DRIVER_NOPROMPT;
	if(hWnd!=0) { prompt=SQL_DRIVER_PROMPT; }
	SQLRETURN ret=SQLDriverConnectA(hConnection,hWnd,(SQLCHAR*)constring
			,SQL_NTS,(SQLCHAR*)connect.GetBuffer(),SMAXCONNECTIONSTRING-1,&len,prompt);

	mConnectionString=connect;
	if(*connect==0) { MErrorInfo::SetError(""); return false; }

	if(!SQL_SUCCEEDED(ret))
		{
		SetError();
		return false;
		}

	return true;
	}


////////////////////////////////
bool MODBCConnection::Close(void) 
	{
	if(hConnection==SQL_NULL_HANDLE) { return false; 	}
	SQLDisconnect(hConnection);
	hConnection=SQL_NULL_HANDLE; 
	return true;
	}


//////////////////////////////////////////////////////
bool MODBCConnection::AutoCommit(bool opt)
	{  SQLRETURN ret;
	if(opt)
		{
		ret=SQLSetConnectAttr(hConnection,SQL_ATTR_AUTOCOMMIT,
				(SQLPOINTER)SQL_AUTOCOMMIT_ON,0);
		}
	else
		{
		ret=SQLSetConnectAttr(hConnection,SQL_ATTR_AUTOCOMMIT,
				(SQLPOINTER)SQL_AUTOCOMMIT_OFF,0);
		}


	if(!SQL_SUCCEEDED(ret))
		{  
		SetError();  
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////////
bool MODBCConnection::Commit(void)
	{
	SQLRETURN ret=SQLEndTran(SQL_HANDLE_DBC,hConnection,SQL_COMMIT);
	if(!SQL_SUCCEEDED(ret))
		{
		SetError();
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////
bool MODBCConnection::RollBack(void)
	{
	SQLRETURN ret=SQLEndTran(SQL_HANDLE_DBC,hConnection,SQL_ROLLBACK);
	if(!SQL_SUCCEEDED(ret))
		{
		SetError();  
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////////
const char *MODBCConnection::GetConnectionString(void)
	{
	return mConnectionString.Get();
	}


//************************************************************************
//*     MODBCStatement
//************************************************************************
void MODBCStatement::SetError(SQLHANDLE hStmt)
	{	SQLRETURN ret;  SQLCHAR state[7]; SQLINTEGER nat;SQLSMALLINT len;
	MBuffer tErrorBuf(MMaxErrorBuffer);  *tErrorBuf=0;

	ret=SQLGetDiagRecA(SQL_HANDLE_STMT,hStmt,1,
			state,&nat,(SQLCHAR*)tErrorBuf.GetBuffer(),(SQLSMALLINT)MMaxErrorBuffer,&len);
	
	if(SQL_SUCCEEDED(ret)){ MErrorInfo::SetError(tErrorBuf); return;}
	
	MErrorInfo::SetError("MODBCStatement::SetError() Unexpected Error");
	}


//////////////////////////////////////////////////////////
SQLHANDLE MODBCStatement::GetHandle(void)
	{
	return hStatement;
	}
	

//////////////////////////////////////////////////////////
MODBCStatement::MODBCStatement(void)
	{  hStatement=SQL_NULL_HANDLE;  }


//////////////////////////////////////////////////////////
MODBCStatement::~MODBCStatement(void)
	{  Destroy();  }


//////////////////////////////////////////////////////////
bool MODBCStatement::Create(MODBCConnection &con)
	{
	Destroy();

	SQLHANDLE hStmt=SQL_NULL_HANDLE;
	SQLRETURN ret=SQLAllocHandle(SQL_HANDLE_STMT,con.GetHandle(),&hStmt);
	if(!SQL_SUCCEEDED(ret))
		{
		MErrorInfo::SetError("MSqlConnection::SQLExec Alloc Statement");
		return false;
		}

	hStatement=hStmt;
	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::Destroy(void)
	{
	if(hStatement==SQL_NULL_HANDLE) { return false; }
	SQLFreeHandle(SQL_HANDLE_STMT,hStatement);
	hStatement=SQL_NULL_HANDLE;  return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::ExecDirect(const char *sql)
	{
	if(hStatement==SQL_NULL_HANDLE)
		{
		MErrorInfo::SetError("MODBCStatement::ExecDirect()-Statement Handle is NULL");
		return false;
		}
		
	SQLRETURN ret=SQLExecDirectA(hStatement,(SQLCHAR*)sql,SQL_NTS);

	if(SQL_SUCCEEDED(ret)) {  return true; }

	SetError(hStatement); 
	return false;	
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::Execute(void)
	{  
	if(hStatement==SQL_NULL_HANDLE)
		{
		MErrorInfo::SetError("MODBCStatement::ExecDirect()-Statement Handle is NULL");
		return false;
		}
		
	SQLRETURN ret=SQLExecute(hStatement);

	if(SQL_SUCCEEDED(ret)) {  return true; }

	SetError(hStatement); 
	return false;
	}


///////////////////////////////////////////////////////
bool MODBCStatement::Prepare(const char *sql)	 // "insert into TCar(CName,CType,CAge) values(?,?,?)
	{
	SQLRETURN ret=SQLPrepareA(hStatement,(SQLCHAR*)sql,SQL_NTS);
	if(SQL_SUCCEEDED(ret)) {  return true; }
	
	SetError(hStatement);
	return false;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::Fetch(void)		// Fetch a row
	{
	SQLRETURN ret=SQLFetch(hStatement);
	if(!SQL_SUCCEEDED(ret)) { return false; }
	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::BindCol(int index,int &variable)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLINTEGER cbVariable=0;
	SQLRETURN ret=SQLBindCol(hStatement, index+1, SQL_C_LONG,&variable, 0, &cbVariable);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::Cancel(void)
	{
	SQLRETURN ret;
	ret=SQLCancel(hStatement);
	if(!SQL_SUCCEEDED(ret)) { SetError(hStatement); return false; }

	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::BindCol(int index,float &variable)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLINTEGER cbVariable=0;
	SQLRETURN ret=SQLBindCol(hStatement, index+1, SQL_C_FLOAT,&variable, 0, &cbVariable);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::BindCol(int index,double &variable)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLINTEGER cbVariable=0;
	SQLRETURN ret=SQLBindCol(hStatement, index+1, SQL_C_DOUBLE,&variable, 0, &cbVariable);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::BindCol(int index,char *lpszBuffer,int bufsize,int &retlen)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLRETURN ret=SQLBindCol(hStatement, index+1, SQL_C_CHAR,lpszBuffer, bufsize
			,(SQLINTEGER *)&retlen);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::BindParameter(int index,int &variable)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLRETURN ret=SQLBindParameter(hStatement, index+1, SQL_PARAM_INPUT, SQL_C_LONG,
			SQL_INTEGER, 0, 0, &variable, 0, 0);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////////////
bool MODBCStatement::BindParameter(int index,float &variable)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLRETURN ret=SQLBindParameter(hStatement, index+1, SQL_PARAM_INPUT, SQL_C_FLOAT,
			SQL_REAL, 0, 0, &variable, 0, 0);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::BindParameter(int index,double &variable)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLRETURN ret=SQLBindParameter(hStatement, index+1, SQL_PARAM_INPUT, SQL_C_DOUBLE,
			SQL_DOUBLE, 0, 0, &variable, 0, 0);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::BindParameter(int index,char *lpszBuffer,int bufsize)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLRETURN ret=SQLBindParameter(hStatement, index+1, SQL_PARAM_INPUT, SQL_C_CHAR,
			SQL_CHAR, bufsize, 0,lpszBuffer, 0, 0);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MODBCStatement::BindParameter(int index,int bufsize,char *memo)
	{
	if(index<0)
		{
		MErrorInfo::SetError("Column Index is negative");
		return false;
		}

	SQLRETURN ret=SQLBindParameter(hStatement, index+1, SQL_PARAM_INPUT, SQL_C_CHAR,
			SQL_LONGVARCHAR, bufsize, 0, memo, 0,0);

	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	return true;
	}


////////////////////////////////////////////////////////
bool MODBCStatement::NativeSql(char *sql,char *buffer,int buflen,int *neededlen)
	{
	SQLRETURN ret; char temp[1];  int len=0;
	if(buffer==0) { buffer=temp; buflen=1; }

	SQLINTEGER needlen=0;
	ret=SQLNativeSqlA(hStatement,(SQLCHAR*)sql,strlen(sql),(SQLCHAR*)buffer,buflen-1,&needlen);
	
	if(!SQL_SUCCEEDED(ret))
		{
		SetError(hStatement);
		return false;
		}

	if(neededlen!=0) { *neededlen=needlen; }

	return true;
	}


////////////////////////////////////////////////////////
int MODBCStatement::NumResultCols(void)	//=0 if error
	{   SQLSMALLINT tColumns;
	SQLRETURN ret=SQLNumResultCols(hStatement,&tColumns);
	if(!SQL_SUCCEEDED(ret)) {  return 0; }
	return tColumns;
	}


///////////////////////////////////////////////////////
bool MODBCStatement::GetData(int index,char *col,int collen,int *max)
	{
	if(index<0)
		{
		MErrorInfo::SetError("MODBCStatement::GetData(...) Index is out of bounds");
		return false;
		}

	char temp[1];
	if(col==0) { col=temp; collen=0; }
	else if(collen>0) { col[0]=0; }
	
	SQLINTEGER maxlen=0;

	SQLRETURN ret=SQLGetData(hStatement,(SQLSMALLINT)index+1,SQL_C_CHAR
				,(SQLPOINTER)col,(SQLINTEGER)collen,&maxlen);
	if(!SQL_SUCCEEDED(ret))
		{ 
		SetError(hStatement);
		return false;
		}

	if(maxlen==SQL_NULL_DATA) { maxlen=-1; }  // NULL Data
	if(max!=0) { *max=maxlen+1; }

	return true;
	}

////////////////////////////////////////////////////
bool MODBCStatement::ParamData(void *bufferaddr)
	{
	SQLRETURN ret=SQLParamData(hStatement,(SQLPOINTER*)bufferaddr);

	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement);
	return false;
	}


////////////////////////////////////////////////////
bool MODBCStatement::PutData(void *data,int len)
	{
	SQLRETURN ret=SQLPutData(hStatement,(SQLPOINTER)data,(SQLINTEGER)len);

	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement); 
	return false;
	}


/////////////////////////////////////////////////////
bool MODBCStatement::Tables(const char *name,const char *type)
	{ 
	SQLRETURN ret;
	ret=SQLTablesA(hStatement, NULL, 0, NULL, 0,(SQLCHAR*)name,(SQLSMALLINT)strlen(name)
			,(SQLCHAR*)type,SQL_NTS);
	
	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement);
	return false;
	}


/////////////////////////////////////////////////////
bool MODBCStatement::Columns(const char *tablename,const char *colpattern)
	{  SQLRETURN ret;
	ret=SQLColumnsA(hStatement, NULL, 0, NULL, 0,(SQLCHAR*)tablename,SQL_NTS
			,(SQLCHAR*)colpattern,(SQLSMALLINT)strlen(colpattern));
	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement);
	return false;
	}



/////////////////////////////////////////////////////
bool MODBCStatement::PrimaryKeys(const char *tablename)
	{  SQLRETURN ret;
	ret=SQLPrimaryKeys(hStatement, NULL, 0, NULL, 0,(SQLTCHAR*)tablename
			,(SQLSMALLINT)strlen(tablename));
	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement);
	return false;
	}


/////////////////////////////////////////////////////
bool MODBCStatement::Procedures(const char *pattern)
	{
	SQLRETURN ret;
	ret=SQLProcedures(hStatement, NULL, 0, NULL, 0,(SQLTCHAR*)pattern
			,(SQLSMALLINT)strlen(pattern));
	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement);
	return false;
	}


////////////////////////////////////////////////////
bool MODBCStatement::ProcedureColumns(const char *tablename,const char *colpattern)
	{   SQLRETURN ret;
	ret=SQLProcedureColumnsA(hStatement, NULL, 0, NULL, 0
			,(SQLCHAR*)tablename,(SQLSMALLINT)strlen(tablename)
			,(SQLCHAR*)colpattern,(SQLSMALLINT)strlen(colpattern));
	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement);
	return false;
	}


///////////////////////////////////////////////////
bool MODBCStatement::ForeignKeys(const char *tablename)
	{   SQLRETURN ret;
	ret=SQLForeignKeysA(hStatement, NULL, 0, NULL, 0,(SQLCHAR*)tablename
				,(SQLSMALLINT)strlen(tablename),NULL,0,NULL,0,NULL,0);
			
	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement);
	return false;
	}


///////////////////////////////////////////////////
bool MODBCStatement::DescribeCol(int colindex,char *data,int  datasize,int &colsize)
	{   SQLRETURN ret;  SQLSMALLINT actsize=0,type=0,decsize=0,nullable=0; 
	colsize=0; *data=0;
	ret=SQLDescribeColA(hStatement,(SQLSMALLINT)colindex+1,(SQLCHAR*)data
			,(SQLSMALLINT)datasize,&actsize,&type,(SQLUINTEGER*)&colsize
			,&decsize,&nullable);
			
	if(SQL_SUCCEEDED(ret)) { return true; }

	SetError(hStatement);
	return false;
	}



