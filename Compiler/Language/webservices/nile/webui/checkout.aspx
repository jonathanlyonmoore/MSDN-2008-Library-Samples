<%@ Page Inherits="Nile.Checkout"%>

<HTML>
<HEAD>
<TITLE>ShoppingCart</TITLE>
</HEAD>
<BODY background="graphics/cream.gif">
<CENTER><IMG src="graphics/checkout.gif">
<FORM method="post" action="checkout.aspx" runat="server">
<table border="0" width="80%">
<tr>
    <td>
    
    <table cellpadding="2" cellspacing="2" width="100%" border="2" align="center">
    <tr>
    <td colspan="2" align="middle"><font color="#000000">Shopping Cart</font>
    </td>
    </tr>
    </table>

    <table border="0" width="100%">
    <span id="ShoppingCartPlaceHolder" runat="server"/>
    <span id="TotalsPlaceHolder" runat="server"/>
    </table>

    <table cellpadding="2" cellspacing="2" width="100%" border="2" align="center">
    <tr>
    <td colspan="2" align="middle"><font color="#000000">Credit Card Info</font>
    </td>
    </tr>
    </table>

    <span id="CreditCardInfoPlaceHolder" runat="server"/>

    </td>
    </tr>
</table>
    <INPUT type="submit" value="Submit" id="SubmitButton" runat="server">
</FORM>
</center>
</BODY>
</HTML> 
