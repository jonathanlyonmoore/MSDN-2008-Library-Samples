<%@ Page Inherits="Nile.Account" %>

<!DOCTYPE html public="-//W3C//DTD HTML 3.2 Final//EN">
<html>
	<head>
		<title>
			New User
		</title>
		<meta http-equiv="DESCRIPTION" content="This is the page where new users are created and added to the user database.">
		<META HTTP-EQUIV="Expires" CONTENT="Tues, 04 December 1993 21:29:02 GMT">
	</head>
	<body background="graphics/cream.gif">
		<center>
			<IMG height=85 src="graphics/account.gif" width=188>
		</center>
		<form runat="server" method="POST" action="account.aspx">
			<table width="550" border="0" cellspacing="1" cellpadding="1" align="center">
				<tr>
					<td>
						First Name:
					</td>
					<td>
						<input runat="server" id="FirstNameField" size="20">
					</td>
					<td colspan="2" align="right">
						Last Name:
						<input runat="server" id="LastNameField" size="20">
					</td>
				</tr>
				<tr>
					<td>
						Address:
					</td>
					<td colspan="3">
						<input runat="server" id="AddressField" size="40">
					</td>
				</tr>
				<tr>
					<td>
						City:
					</td>
					<td>
						<input runat="server" id="CityField" size="20">
					</td>
					<td nowrap>
						State:
						<input runat="server" id="StateField" size="4">
					</td>
					<td align="right">
						Zip Code:
						<input runat="server" id="ZipField" size="12" value="1">
					</td>
				</tr>
				<tr>
					<td>
						&nbsp;
					</td>
				</tr>
				<tr>
					<td>
						Email:
					</td>
					<td>
						<input runat="server" id="EmailField" size="20">
					</td>
					<td colspan="2" align="right">
						Phone:
						<input runat="server" id="PhoneField" size="20">
					</td>
				</tr>
				<tr>
					<td nowrap>
						Credit Card:
					</td>
					<td colspan="2">
						<input runat="server" id="CreditField" size="20">
							Expiration Date:
						<input runat="server" id="DateField" size="8">
					</td>
				</tr>
				<tr>
					<td height="50">
						UserName:
					</td>
					<td>
						<input runat="server" id="UserField" size="20">
					</td>
					<td colspan="2" align="right">
						Password:
						<input type="password" runat="server" id="PasswordField" size="20">
					</td>
				</tr>
				<tr>
					<td>
					</td>
					<td>
						<input type="submit" runat="server" id="SubmitButton" value="Submit">
						<input type="reset" name="cmdReset" value="Reset">
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
