var oArgs = WScript.Arguments;
if (oArgs.length != 1)
{
	WScript.Echo("must give word length as argument");
	WScript.Quit();
}
var oFS = new ActiveXObject("Scripting.FileSystemObject");
ForReading = 1;
var strLength = oArgs(0);
var oWords = oFS.OpenTextFile("words", ForReading);
var oNoDups = oFS.CreateTextFile("nodups." + strLength, true);
var oDict = oFS.CreateTextFile("dict." + strLength, true);
var strLine;
oMatcher = new RegExp("^[a-z]{" + strLength + "}$");
while (!oWords.AtEndOfStream)
{
	strLine = oWords.ReadLine();
	var oLetters = new Array(26);
	if (oMatcher.test(strLine))
	{
		for (i=0; i<26; i++)
			oLetters[i] = false;
		var bDup = false;
		for (i=0; i<strLine.length; i++)
		{
			var nChar = strLine.charCodeAt(i) - "a".charCodeAt(0);
			if (oLetters[nChar])
				bDup = true;
			oLetters[nChar] = true;
		}
		if (!bDup)
			oNoDups.WriteLine(strLine);
		oDict.WriteLine(strLine);
	}
}
