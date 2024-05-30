<%@ Page Inherits="Nile.Results" %>

<html>
<head>
</head>
<body background="graphics/cream.gif">
<p align="CENTER"><FONT size+="1"><span id="NumResultsPlaceHolder" runat="server"></span> Books found</FONT></p>

<TABLE cellpadding="5" cellspacing="2" width="100%" border>
    <TR>
        <TD>
            <span id="PreviousLinkPlaceHolder" runat="server"/>
        </TD>            
        <TD align="CENTER">
            Page <span id="CurrentPagePlaceHolder" runat="server"/> of <span id="NumPagesPlaceHolder" runat="server"/>
        </TD>            
        <TD>
            <span id="NextLinkPlaceHolder" runat="server"/>
        </TD>
    </TR>
</TABLE>
<BR>

<TABLE id="tableBooks" width="100%" cellpadding="5" cellspacing="2" border=1>
<TR>      
    <TH align="CENTER"></TH>          
    <TH align="CENTER">Book</TH>
    <TH align="CENTER">Author</TH>
</TR>
<span id="ResultsPlaceHolder" runat="server"/>
</TABLE>

</body>
</html>
