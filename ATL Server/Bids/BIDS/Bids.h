// File: Bids.h
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

// Convert a formatted currency string to a double
double atofc(LPCSTR szCurrency);

// Convert a double (DBDATE) style date to a string
void dbl2str(double dDate, LPSTR szDate, LPSTR szTime);

// Convert a string date to a double (DBDATE) style date
double str2dbl(LPCSTR szDate, LPCSTR szTime);