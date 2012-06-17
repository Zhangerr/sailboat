function this:main() 
	color=string.format("background-color:rgb(%s,%s,%s);",math.random(0,255),math.random(0,255),math.random(0,255))
	this:echo([[
	<!DOCTYPE html>
	
	<html>
	<head>
	<title>
	!~LUA~!
	</title>
	<style>
	html {
	]]
	.. color ..
	[[
	
	font-family:"Myriad Pro", sans-serif, Arial;
	}
	body {
	
		background-color:rgb(200,200,200);
		border-radius:5px;
		padding:5px;
		font-size:20px;
		text-align:center;
	}
	</style>
	</head>
	<body>
	~ hi ~ lua test ~ try refreshing me ~
	</body>
	</html>
	]])
	--this:echo(math.random(1,100))	
	
	trace("printing~")
	table.foreach(this,print)
end