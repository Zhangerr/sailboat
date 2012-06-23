function main() 
	--in the future, perhaps there should be COOKIES, POST, and GET declared as globals (or passed as arguments)
	color=string.format("background-color:rgb(%s,%s,%s);",math.random(0,255),math.random(0,255),math.random(0,255))		
	sb:echo([[
	<!DOCTYPE html>
	<html>
	<head>
	<title>
	!~LUA~!
	</title>
	<style>
	html {
	]] .. color ..
	[[
	
	font-family:"Myriad Pro", sans-serif, Arial;
	}
	body {
	
		background-color:rgb(240,240,240);
	
		width:400px;
		margin:auto;
		padding:5px;
		font-size:20px;
		text-align:center;
		box-shadow: 0px 0px 40px 0px #444;
	}
	</style>
	</head>
	<body>
	<div id="content">
	<form name="input" action="" method="post">
	Username: <input type="text" name="user" />
	<input type="submit" value="Submit" />
	</form>
	~ hi ~ lua test ~ try refreshing me ~ <br /><br />
	]])
	for i,v in pairs(GET) do
	sb:echo(i .. '=' .. v .. '<br />');
	end
	sb:echo([[<table
	style="
	border-color:grey;
	border-width:0px;
	border-collapse:collapse;
	width:300px;
	margin:auto;
	font-size:15px;
	" border=1
	>]])
	counter = 0
	
	for i,v in pairs(_ENV) do
	counter = counter + 1
	if counter % 2 == 0 then 
		sb:echo("<tr>")
	else
		sb:echo([[<tr style="background-color:lightgrey;">]])
	end	
	sb:echo("<td>")
	sb:echo(tostring(i)) --find way to make it so auto converts to string
	sb:echo("</td>")
	sb:echo("<td>")
	sb:echo(tostring(v))
	sb:echo("</td>")
	sb:echo("</tr>")
	end
	sb:echo("</table>")
	sb:echo([[</div>
	</body>
	</html>
	]])
end