// File: RowValues.cpp
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "Stdafx.h"
#include "RowValues.h"

RowValues::RowValues() : CAtlArray<ColumnValues>()	
{
	Trace::TraceMsg("RowValues constructor");
}

RowValues::RowValues(const RowValues& c)
{
	// do a deep copy
	Trace::TraceMsg("RowValues copy constructor");
			
	size_t num = c.GetCount();
	
	for (size_t i = 0; i < num; i++)
	{						
		Add(c[i]);
	}		
}

RowValues::~RowValues()
{
	Trace::TraceMsg("RowValues destructor");		
	RemoveAll();
}

const ColumnValues& RowValues::GetRowValue(size_t index) const
{				
	return GetAt(index);		
}

void RowValues::AddRowValue(const ColumnValues& columnValues)
{		
	Add(columnValues);		
}

size_t RowValues::GetNumRows() const
{
	return GetCount();
}

void RowValues::Reset()
{
	RemoveAll();
}

// need to override = operator to make a deep copy
RowValues& RowValues::operator=(const RowValues& p)
{
	Trace::TraceMsg("RowValue = operator");

	RemoveAll();
	size_t num = p.GetCount();
	for (size_t i = 0; i < num; i++)
	{
		Add(p[i]);
	}

	return (*this);
}	