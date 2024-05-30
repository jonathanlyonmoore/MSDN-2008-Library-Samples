<%@ Page Inherits="Nile.Cart" %>

<HTML>
<HEAD>
<TITLE>ShoppingCart</TITLE>
</HEAD>

<BODY background="graphics/cream.gif">
<P align="CENTER"><IMG src="graphics/dkcart.gif"><BR><B>Total Results - Books</B></P>
<CENTER>

<FORM runat="server" method="POST" action="cart.aspx">

<table>
<span id="CartItemsPlaceHolder" runat="server"/>
</table>

<input type="submit" id="CheckOutButton" runat="server" value="Check Out"/>
<input type="submit" id="ContinueShoppingButton" runat="server" value="Continue Shopping"/>

</FORM>
</CENTER>
</BODY>
</HTML> 
