// File: dbcommand.cpp
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "StdAfx.h"
#include "dbcommand.h"

DBCommand::DBCommand(IResultsBuilder		*builder,
					 DataConnectionFactory	*dataConnectionFactory,
					 CmdInfoFactory			*cmdInfoFactory)
{
	Trace::TraceMsg("DBCommand constructor");

	m_resultBuilder			= builder;
	m_dataConnectionFactory = dataConnectionFactory;
	m_cmdInfoFactory		= cmdInfoFactory;
}

DBCommand::~DBCommand(void)
{
	Trace::TraceMsg("DBCommand destructor");

	m_resultBuilder			= NULL;
	m_dataConnectionFactory = NULL;
	m_cmdInfoFactory		= NULL;
}

void DBCommand::IterateRowset(CommandType&	 command,
							  DBORDINAL		 ulColumns,
							  struct MYBIND* pBind)
{	
	long rowsAffected = 0;

	// get the rowset items	
	while (command.MoveNext() == S_OK)
	{
		for (ULONG j = 0; j < ulColumns; j++)
		{
			if (pBind[j].dwStatus == DBSTATUS_S_ISNULL)
				pBind[j].szValue[0] = '\0';

			m_resultBuilder->AddColumnValue(CStringA(pBind[j].szValue));
		}			
		m_resultBuilder->DoneRow();
	}	
	command.ReleaseRows();
}

HRESULT DBCommand::GetResults(CommandType& command, size_t numParams)
{	
	HRESULT hr = S_OK;
//	LONG lRows = 0;
	DBROWCOUNT lRows = 0;

	// Get the column information and bind the output columns.
//	ULONG           ulColumns = NULL;
	DBORDINAL       ulColumns = NULL;
	DBCOLUMNINFO*   pColumnInfo = NULL;
	OLECHAR*        pColumnNames = NULL;
	struct MYBIND*  pColumns    = NULL;
	
	// this loop will handle stored procs that handle multiple result sets
	while (hr == S_OK)
	{		
		if (command.m_spRowset != NULL)	
		{			
			ulColumns	 = NULL;
			pColumnInfo	 = NULL;
			pColumnNames = NULL;			
			pColumns     = NULL;

			hr = command.GetColumnInfo(&ulColumns, &pColumnInfo, &pColumnNames);

			if (pColumnNames != NULL)
			{
				CoTaskMemFree(pColumnNames);
			}

			ATLENSURE(!FAILED(hr));
			if (FAILED(hr))
				return hr;
	
			if (ulColumns > 0)
			{
				pColumns = new MYBIND[ulColumns];

				ATLENSURE(pColumns);
				if (pColumns == NULL)
				{
					// Clean up and quit
					if (pColumnInfo)
						CoTaskMemFree(pColumnInfo);
					return E_OUTOFMEMORY;
				}
				
				hr = command.CreateAccessor((int)ulColumns, &pColumns[0], sizeof(MYBIND)*ulColumns);

				ATLENSURE(!FAILED(hr));
				if (FAILED(hr))
				{
					// Clean up and quit
					if (pColumnInfo)
						CoTaskMemFree(pColumnInfo);
					return hr;
				}
				
				for (ULONG l = 0; l < ulColumns; l++)
				{
					command.AddBindEntry(	l+1,
											DBTYPE_STR,
											sizeof(TCHAR) * MAX_SIZE, 
											&pColumns[l].szValue,
											NULL, 
											&pColumns[l].dwStatus);
				}
			}

			// bind our command to get column, output parameter and return values
			command.Bind();
	
			// iterate over our results			
			IterateRowset(command, ulColumns, pColumns);

			// we are finished with 1 result set, move on to the others
			Trace::TraceMsg("DBCommand done result set");
			m_resultBuilder->DoneResultSet();
		}		
		lRows = 0;						

		// Get the next result set
		hr = command.GetNextResult(&lRows, false);

		// clean up
		if (pColumnInfo != NULL)
		{
			CoTaskMemFree(pColumnInfo);
			pColumnInfo = NULL;
		}

		if (pColumns)
		{
			delete[] pColumns;	
			pColumns = NULL;
		}
	}
	command.Close();
	return hr;
}


template <class T> class smart_ptr_bind
{
private:
   T *cmdParams; 

public:
   smart_ptr_bind(size_t numParams)
   {
		cmdParams= new T[numParams];
   }
   ~smart_ptr_bind()
   {
	   if(cmdParams)
		   delete[] cmdParams;
   }
   T* get()
   {
	   return cmdParams;
   }
};


HRESULT DBCommand::ExecuteCommand(	const CSession&	session,						   
									CmdInfo			*cmdInfo,						   
									StringList&		inputParams)
{
	ATLENSURE(cmdInfo != NULL);
	if (cmdInfo == NULL)
	{
		return E_FAIL;
	}	

	HRESULT hr = S_OK;

	CommandType command;

	// create the command using our call string
	hr = command.Create(session, cmdInfo->cmdCallString);

	ATLENSURE(!FAILED(hr));
	if (FAILED(hr))
	{
		return hr;
	}

	// only going to call this once
	hr = command.Prepare(1);

	ATLENSURE(!FAILED(hr));
	if (FAILED(hr))
	{
		return hr;
	}

	// if this stored proc takes parameters pass them in
	size_t numParams = cmdInfo->paramTypes.GetCount();

	// need to declare this here since we might need this array if
	// we have output parameter or return values
	// we can only get these values 
	smart_ptr_bind<MYBIND> autobindptr0(numParams);
    MYBIND *cmdParams = autobindptr0.get();

	ATLENSURE(cmdParams != NULL);
	if (cmdParams == NULL)
	{
		// can't do anything except get out of here
		return E_OUTOFMEMORY;
	}

	if (numParams > 0)
	{
		POSITION			inputParamPos = NULL;
		CStringA			paramValue;
		//ULONG*				ordinals;
		DBORDINAL*				ordinals;

		smart_ptr_bind<DBPARAMBINDINFO> autobindptr1(numParams);
		DBPARAMBINDINFO *bindInfo = autobindptr1.get();

		ATLENSURE(bindInfo != NULL);
		if (bindInfo == NULL)
		{
			return E_OUTOFMEMORY;
		}

		//ordinals = new ULONG[numParams];
		ordinals = new DBORDINAL[numParams];
		ATLENSURE(ordinals != NULL);
		if (ordinals == NULL)
		{	
			// can't do anything except get out of here

			// bindInfo will automatically get
			// deleted because its pointer was
			// copied from autobindptr1
			return E_OUTOFMEMORY;
		}

		// get the position of the first input parameter
		// we'll check for null later, inputParamPos can be null
		// if the stored proc doesn't have any input params, only 
		// a return value
		inputParamPos = inputParams.GetHeadPosition();
		
		// make the bindings for the accessor based on the DBTYPE_STR data type
		hr = command.CreateParameterAccessor((int)numParams, &cmdParams[0], sizeof(MYBIND)*numParams);

		ATLENSURE(SUCCEEDED(hr));
		if (SUCCEEDED(hr))
		{
						
			for (size_t l = 0; l < numParams; l++)
			{
				// Set up information for the SetParameterInfo call
				ordinals[l] = l+1;
				bindInfo[l].pwszDataSourceType = CT2OLE(_T("DBTYPE_CHAR"));
				bindInfo[l].pwszName = NULL;					
				bindInfo[l].ulParamSize = MAX_SIZE;
				bindInfo[l].bPrecision = 0;
				bindInfo[l].bScale = 0;

				DWORD dwFlags = 0;
			
				if (cmdInfo->paramTypes[l] == DBPARAMTYPE_INPUT || 
				    cmdInfo->paramTypes[l] == DBPARAMTYPE_INPUTOUTPUT)
					dwFlags |= DBPARAMFLAGS_ISINPUT;

				if (cmdInfo->paramTypes[l] == DBPARAMTYPE_OUTPUT ||
				    cmdInfo->paramTypes[l] == DBPARAMTYPE_INPUTOUTPUT ||
				    cmdInfo->paramTypes[l] == DBPARAMTYPE_RETURNVALUE)
					dwFlags |= DBPARAMFLAGS_ISOUTPUT;
			
				bindInfo[l].dwFlags = dwFlags;
				
				// Set up information for bindings
				if (cmdInfo->paramTypes[l] == DBPARAMTYPE_RETURNVALUE)
				{				
					command.AddParameterEntry(	1, 
									DBTYPE_STR, 
									sizeof(TCHAR) * MAX_SIZE,
									&cmdParams[l].szValue, 
									NULL, 
									&cmdParams[l].dwStatus,
									DBPARAMIO_OUTPUT);
				}
				else
				{
					// bind
					command.AddParameterEntry(	l+1, 
									DBTYPE_STR, 
									sizeof(TCHAR) * MAX_SIZE,
									&cmdParams[l].szValue, 
									NULL, 
									&cmdParams[l].dwStatus,
									cmdInfo->paramTypes[l]);

					// fill in the value
					if (cmdInfo->paramTypes[l] == DBPARAMTYPE_INPUT || cmdInfo->paramTypes[l] == DBPARAMTYPE_INPUTOUTPUT)
					{					
						if (inputParamPos != NULL)
						{
							CStringA temp = inputParams.GetNext(inputParamPos);
							int iLen = temp.GetLength();
							ATLENSURE(iLen < MAX_SIZE);
							if (iLen >= MAX_SIZE)
							{
								hr = E_FAIL;
								break;
							}
							//_tcsncpy(cmdParams[l].szValue, temp, iLen);
							_tcsncpy_s(cmdParams[l].szValue, sizeof(cmdParams[l].szValue)/sizeof(TCHAR), temp, iLen);
						}
					}
				}
			}	

			// call ICommandWithParameters::SetParameterInfo so that we can
			// tell the provider to convert the parameter types to DBTYPE_STR
			if (SUCCEEDED(hr))
				command.SetParameterInfo(numParams, ordinals, bindInfo);		
		}

		// cleanup
		if (ordinals) {
			delete[] ordinals;
			ordinals = NULL;
		}

		// bindInfo will automatically get
		// deleted because its pointer was
		// copied from autobindptr1
	}

	if (SUCCEEDED(hr))
	{
		// execute our command and get back a rowset		
		hr = command.Open(NULL, NULL, false);	
		ASSERT(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = GetResults(command, numParams);
			ASSERT(SUCCEEDED(hr));

			if (SUCCEEDED(hr) && numParams > 0)
			{
				// get the return value and out parameters
				for (size_t i = 0; i < numParams; i++)
				{
					if (i == 0)
					{		
						m_resultBuilder->SetReturnValue(CStringA(cmdParams[i].szValue));
					}
					else if (cmdParams[i].dwStatus == DBPARAMTYPE_RETURNVALUE)
					{
						m_resultBuilder->AddOutputParamValue(CStringA(cmdParams[i].szValue));
					}		
				}				
			}
		}
	}
	return hr;
}

HRESULT DBCommand::Execute(CStringA&	cmdName, 
						   CStringA&	connectionString,
						   StringList&	inputParams)
{
	bool dsFromCache = false;
	HRESULT hr = S_OK;
		
	// get a dataconnection
	ASSERT(m_dataConnectionFactory != NULL);
	if (m_dataConnectionFactory == NULL)
	{
		return E_FAIL;
	}

	CDataConnection datasource;	
	hr = m_dataConnectionFactory->GetDataConnection(connectionString,													
													datasource, 
													dsFromCache);
	
	ASSERT(!FAILED(hr));

	if (FAILED(hr))
	{
		// nothing we can do, we can't get a data connection, get out of here
		return hr;
	}

	// if we got here, then we got a data connection somehow, either using the cache,
	// or opening one on our own

	// get information about this stored procedure's parameters						
	CmdInfo *cmdInfo = NULL;
	bool infoFromCache = false;
	
	ASSERT(m_cmdInfoFactory != NULL);
	if (m_cmdInfoFactory == NULL)
	{
		return E_FAIL;
	}

	hr = m_cmdInfoFactory->GetCmdInfo(	cmdName, 
										connectionString, 										
										datasource, 
										&cmdInfo, 
										infoFromCache);

	ASSERT(!FAILED(hr));
	if (FAILED(hr))
	{
		// if we failed, there is a chance that it is because our data connection was 
		// stale, try opening it ourselves
		hr = datasource.Open(CComBSTR(connectionString));
		
		if (SUCCEEDED(hr))
		{	
			// make sure we know to close this connection ourselves
			dsFromCache = false;

			// re-opening the connection worked, try again to get our cmd information
			hr = m_cmdInfoFactory->GetCmdInfo(	cmdName, 
												connectionString, 											
												datasource, 
												&cmdInfo, 
												infoFromCache);
		}
	}
	
	// check to see if we could get our command info
	ASSERT(!FAILED(hr));
	if (FAILED(hr))
	{
		// nothing we can do, just get out
		return hr;
	}

	// if we got here, the we were able to get information about our stored procedure
	// so now execute that stored proc
	hr = ExecuteCommand(datasource,									
						cmdInfo,
						inputParams);				
	ASSERT(!FAILED(hr));

	// not a whole lot of error checking we can do on the returned hr, if the execute
	// command failed, we are going to return the bad hr

	// close the data connection if we didn't get one from the cache
	if (!dsFromCache)
	{
		// datasource.Close();
		datasource.m_session.Close();
		datasource.m_source.Close();
	}

	// delete our command info structure if we didn't get it from the cache
	if (!infoFromCache)
	{
		cmdInfo->paramTypes.RemoveAll();
		delete cmdInfo;
	}

	return hr;
}
