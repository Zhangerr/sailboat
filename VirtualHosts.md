[Name based virtual hosts](http://en.wikipedia.org/wiki/Virtual_hosting) are somewhat supported by sailboat. `hosts.xml` is the file that contains definitions of virtual hosts and is located in the working directory of sailboat, along with `config.xml`. The root node is "hosts"

# VirtualHost #

## Name ##

The name of the virtual host. Using the domain sail.boat.com, the name would be defined as "sail.boat.com". The name matches the domain typed in by a user in their web browser.

## DocumentRoot ##

The directory to serve files from for the given virtual host. Works the same as the document root in config.xml, except specific to a given host name.

## NotFound ##

Allows a virtual host to have its own 404 page to serve when the request page is not found. The selected page has to be within the document root of that virtual host.

## ResourceMoved ##

Specifying a resource as resource moved allows the server to send a 410 permanently moved header. Uses relative path name. Multiple ResourceMoved elements can be specified.

# Sample hosts.xml #
```
<?xml version="1.0" encoding="utf-8"?>
<hosts version="1.0">
	<VirtualHost>
		<Name>localhost</Name>
		<DocumentRoot>www</DocumentRoot>
                <ResourceMoved>test.html</ResourceMoved>
                <ResourceMoved>a/image.jpg</ResourceMoved>
	</VirtualHost>
	<VirtualHost>
		<Name>127.0.0.1</Name>
		<DocumentRoot>www2</DocumentRoot>
		<NotFound>404.html</NotFound>
	</VirtualHost>
</hosts>
```