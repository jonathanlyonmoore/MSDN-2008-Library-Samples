/***********************************************************************
Copyright (c) Microsoft Corporation
All Rights Reserved.
***********************************************************************/
// This is an example of an SQL/CLR. The functions within the DLL can 
// be invoked by using the SQL/CLR support in SQL Server.  This example
// shows a conversion between extended stored procedure and SQL/CLR
// while maintaining the orignial business function and build /CLR:pure
//
// CLR_CHECKSUMSPROC -- Used to show the creation of a new stored 
// procedure to SQL Server
#include "stdafx.h"
#include "stdlib.h"
#include "vcclr.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlClient;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;


public ref class clr_checksumsproc
{
public:
// clr_checksum
//    Returns the checksum value from SQL statement
//
// Parameters:
//    tableName - table to do checksum calculation
//
// Returns:
//    checksum value
	[SqlProcedure]
	static void clr_checksum(String ^tableName)
	{
		SqlConnection ^conn = gcnew SqlConnection("context connection=true");
		conn->Open();
		SqlCommand ^cmd = gcnew SqlCommand(String::Concat("SELECT * FROM ", tableName), conn);
		SqlDataReader ^rdr = cmd->ExecuteReader();

		// query table and read each record/column to calculate checksum
		int CheckSumValue = 0;
		while (rdr->Read())
		{
			String ^str;
			for(int i=0; i < rdr->FieldCount; i++)
			{
				str = rdr->GetValue(i)->ToString();

				// convert to native string before calling checksum
				pin_ptr<const wchar_t> wch = PtrToStringChars(str);

				CheckSumValue += checksum(wch, str->Length);
			}
		}
		rdr->Close();

		//send results 
		cmd->CommandText = "CREATE TABLE ##T (CheckSum INT)";
		cmd->ExecuteNonQuery();
		
		cmd->CommandText = String::Concat("INSERT INTO ##T (CheckSum) VALUES (", CheckSumValue.ToString(), ")");;
		cmd->ExecuteNonQuery();

		cmd->CommandText = "SELECT * FROM ##T";
		rdr = cmd->ExecuteReader();
		SqlContext::Pipe->Send(rdr);
		rdr->Close();
	}

// checksum
//    C++ function to calculate checksum
//
// Parameters:
//    buffer - pointer to char string to calculate checksum
//    length - length of buffer
//
// Returns:
//    checksum value
	static unsigned int checksum(const wchar_t *buffer, int length)
	{
		unsigned int cs = 0;
		for (int i = 0; i < length; i++)
		{
			cs += (unsigned int)buffer[i] & 0xFFFF;
		}
		return cs & 0xFFFF;
	}
};
