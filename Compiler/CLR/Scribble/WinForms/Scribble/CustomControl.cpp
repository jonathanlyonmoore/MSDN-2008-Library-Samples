#ifndef _M_CEE_SAFE

#include "CustomControl.h"
#include "UnmanagedStringProvider.h"

using namespace System;
using namespace System::Drawing;

template<typename _To_Type> _To_Type marshal_as(char* input_string) { return nullptr; }
template<> System::String^ marshal_as<System::String^>(char* input_string) 
{ 
	return gcnew String(input_string); 
}

void ScribbleCustomControl::OnPaint(PaintEventArgs ^e)
{
	UnmanagedStringProvider *usp = new UnmanagedStringProvider;
	String ^Text = marshal_as<String ^>(usp->GetUnmanagedString());
	e->Graphics->DrawString(Text, Font, gcnew SolidBrush(ForeColor), ClientRectangle);
	delete usp;
}

#endif