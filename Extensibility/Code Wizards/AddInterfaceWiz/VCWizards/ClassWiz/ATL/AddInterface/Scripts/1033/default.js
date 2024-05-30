// Copyright (c) Microsoft Corporation.  All rights reserved.
// Script for Add Interface

function OnFinish(selProj, selObj)
{
	var oCM	= selProj.CodeModel;

	try
	{		
		var strInterfaceName = wizard.FindSymbol("INTERFACE_NAME");

		oCM.StartTransaction("Add Interface " + strInterfaceName);

		var strInterfaceFile = wizard.FindSymbol("INTERFACE_FILE");
		var strComment = wizard.FindSymbol("COMMENT");
		var strHelpstring = wizard.FindSymbol("HELPSTRING");
		var strGuid = wizard.CreateGuid();
		strGuid = wizard.FormatGuid(strGuid, 0);
		var bDual = wizard.FindSymbol("DUAL");
		var bAutomation = wizard.FindSymbol("AUTOMATION");
		var bAttributed = wizard.FindSymbol("ATTRIBUTED");
		
		var oInt = oCM.AddInterface(strInterfaceName, strInterfaceFile);
		var oAtt;
		if (bDual)
		{
			var oBase = oInt.AddBase("IDispatch");
			oAtt = oInt.AddAttribute("dual", "");
			if (!bAttributed)
			{
				oAtt = oInt.AddAttribute("nonextensible", "");
			}
		}
		else
		{
			var oBase = oInt.AddBase("IUnknown");			
			if (bAutomation)
			{
				oAtt = oInt.AddAttribute("oleautomation", "");
				if (!bAttributed)
				{
					oAtt = oInt.AddAttribute("nonextensible", "");
				}
			}
		}
		oAtt = oInt.AddAttribute("object", "");
		oAtt = oInt.AddAttribute("uuid", strGuid);
		oAtt = oInt.AddAttribute("helpstring", "\"" + strHelpstring + "\"");
		oAtt = oInt.AddAttribute("pointer_default", "unique");
		oInt.Comment = strComment;
		oCM.CommitTransaction();
	}
	catch(e)
	{
		oCM.AbortTransaction();
		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}
