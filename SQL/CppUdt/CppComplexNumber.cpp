/*=====================================================================

  File:      CppComplexNumber.cpp
  Summary:   A Format.Native UDT that represents a complex number
  Date:	     April 30, 2004

---------------------------------------------------------------------

  This file is part of the Microsoft Visual Studio 2005 Code Samples.
  Copyright (c) Microsoft Corporation.  All rights reserved.

This source code is intended only as a supplement to Microsoft
Development Tools and/or on-line documentation.  See these other
materials for detailed information regarding Microsoft code samples.

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

======================================================= */
#include "stdafx.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;
using namespace System::Text::RegularExpressions;

[Serializable]
[Microsoft::SqlServer::Server::SqlUserDefinedType(Format::Native, IsByteOrdered = true)]
public value struct CppComplexNumber : public SqlTypes::INullable, IComparable
{
private:
	//Regular expression used to parse values that are of the form (1,2i)
	static initonly Regex ^_parser = gcnew Regex("\\(\\s*(?<real>\\-?\\d+(\\.\\d+)?)\\s*,\\s*(?<img>\\-?\\d+(\\.\\d+)?)\\s*i\\s*\\)", RegexOptions::Compiled | RegexOptions::ExplicitCapture);
	literal const String ^NULL = "<<null complex>>";
	static initonly CppComplexNumber NULL_INSTANCE = CppComplexNumber(true);
	double _r;
	double _i;
	bool _isnull;

private:
	__declspec(noinline) CppComplexNumber(bool isnull)
	{
		_isnull = isnull;
		_r = _i = 0;
	}

public:
	__declspec(noinline) CppComplexNumber(double r, double i) 
	{
		_r = r;
		_i = i;
		_isnull = false;
	}

	property double Real 
	{
		double get()
		{
			if (_isnull)
				throw gcnew InvalidOperationException();
			return _r;
		}
		void set(double value) {_r = value;}
	}

	property double Imaginary
	{
		double get()
		{
			if (_isnull)
				throw gcnew InvalidOperationException();
			return _i;
		}
		void set(double value) {_i = value;}
	}

	property double Modulus 
	{
		double get()
		{
			if (_isnull)
				throw gcnew InvalidOperationException();
			return System::Math::Sqrt(_r*_r+_i*_i);
		}
	}
#pragma region "value-based equality"
public:
	__declspec(noinline) virtual int CompareTo(Object ^o)
	{
		if (safe_cast<CppComplexNumber^>(o) == nullptr)
			return -1;
	    
		CppComplexNumber c = *(safe_cast<CppComplexNumber^>(o));
		
		if (_isnull && c._isnull)
			return 0;

		if (_isnull || c._isnull)
			return -1;

		if (_r == c._r && _i == c._i)
			return 0;

		if (Modulus == c.Modulus) // same modulus but different r/i, force diff
			return -1;

		// arbitrary comparison...semantics for complex numbers not necessarily correct
		return Modulus.CompareTo(c.Modulus);
	}

	__declspec(noinline) virtual bool Equals(Object ^o) override
	{
		return (this->CompareTo(o) == 0);
	}
	__declspec(noinline) virtual int GetHashCode() override
	{
		return Modulus.GetHashCode();
	}

	static SqlTypes::SqlBoolean operator==(CppComplexNumber c1, CppComplexNumber c2)
	{
		CppComplexNumber ^c = c2;
		return (c1.Equals(c));
	}
	static SqlTypes::SqlBoolean operator!=(CppComplexNumber c1, CppComplexNumber c2)
	{
		CppComplexNumber ^c = c2;
		return (!c1.Equals(c));
	}
	static SqlTypes::SqlBoolean operator<(CppComplexNumber c1, CppComplexNumber c2)
	{
		CppComplexNumber ^c = c2;
		return (c1.CompareTo(c) < 0);
	}
	static SqlTypes::SqlBoolean operator>(CppComplexNumber c1, CppComplexNumber c2)
	{
		CppComplexNumber ^c = c2;
		return (c1.CompareTo(c) > 0);
	}

#pragma endregion

	__declspec(noinline) virtual String ^ToString() override
	{
		return _isnull ? (String ^)NULL : String::Concat("(", _r.ToString(), ",", _i.ToString(), ")");
	}

	property bool IsNull 
	{
		virtual bool get() {return _isnull;}
	}

	static CppComplexNumber Parse(SqlTypes::SqlString s)
	{
		String ^value = s.ToString();

		if (NULL == value)
		{
			return CppComplexNumber(true);
		}

		Match ^m = _parser->Match(value);

		if (!m->Success)
			throw gcnew ArgumentException("Invalid format for complex number. Format is ( n, mi ) where n and m are floating point numbers");

		return CppComplexNumber(Convert::ToDouble(m->Groups[1]->Value), Convert::ToDouble(m->Groups[2]->Value));
	}

	property static CppComplexNumber Null
	{
		CppComplexNumber get()
		{
			return NULL_INSTANCE;
		}
	}
};

