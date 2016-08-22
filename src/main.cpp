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


//v3.1 copyright Comine.com 20160822M0131
#include "MStdLib.h"
#include "MCommandArg.h"
#include "MSQLExec.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MSQLExec";	// Used in Help
static const char *GApplicationVersion="3.1";	// Used in Help

////////////////////////////////////////////////////
static void GDisplayHelp(void);
static void CheckArgs(bool &stopflag,bool &pauseflag,bool &quiteflag,MCommandArg &args);


//////////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	if(args.CheckRemoveHelp()==true ||  args.GetArgCount()<3  )
		{
		GDisplayHelp();
		return 0;
		}

	//////////////////////////////////////////////////
	bool stopflag=false,pauseflag=false,quiteflag=false;
	CheckArgs(stopflag,pauseflag,quiteflag,args);


	MSQLExec processor;
	processor.ProcessSQLFile(args.GetArg(1),args.GetArg(2)
			,stopflag,pauseflag,quiteflag);
	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(		"\n\n"
					" %s v%s  copyright Comine.com\n"
					"\n"
					"   Usage: %s <odbc> <file> [-p|-s|-?]\n"
					"             <odbc> = ODBC Connection String\n"
					"             <file> = File containing SQL\n"
					"              -s    = Stop on First Error\n"
					"              -p    = Pause after every Statement\n"
					"              -p -s = Pause on an Error\n"
					"              -q    = Do not Display SQL Execution stmts\n"
					"\n"
					"   Description:\n"
					"   **This program will connect to a ODBC data source and execute\n"
					"   SQL statements against that data source.  The SQL File may have\n"
					"   SQL statements seperated with a semicolon(;) and C like comments:\n"
					"        ie   /* Comments */   or  // Comments \n"
					"   There is no maximum size for either the SQL Statements or the src file.\n"
					"\n"
					"   Example Usage:\n"
					"        SQLExec \"DSN=TestDB;UID=john;PWD=doe\"  input.sql \n"
					"\n",GApplicationName,GApplicationVersion,GApplicationName);
	}


//////////////////////////////////////////////////////////
static void CheckArgs(bool &stopflag,bool &pauseflag,bool &quiteflag,MCommandArg &args)
	{
	stopflag=false;  pauseflag=false;  quiteflag=false;
	for(int i=3;i<args.GetArgCount();++i)
		{
		if(MStdStrICmp(args.GetArg(i),"-s")==0) { stopflag=true; }
		else if(MStdStrICmp(args.GetArg(i),"-p")==0) {  pauseflag=true; }
		else if(MStdStrICmp(args.GetArg(i),"-q")==0) {  quiteflag=true; }
		}
	}


