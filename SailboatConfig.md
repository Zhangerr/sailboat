The sailboat config file is in xml format, and contains some variables that alter the running of the server. The config file _**must**_ be named "config.xml". The current root node is "settings".

# Settings #
## DocumentRoot ##
Sets the root of the website, all pages will be loaded from this location. A root of www will cause a GET / to attempt loading the current working directory(location of sailboat)/www/index.html.

## FileExtensions ##
Sets a list of allowed extensions when attempting to load pages without a file extension present (example GET /home). List is in format extension1|extension2|extensioN and supports any number of extensions.

## Port ##
The port the web server should listen on, specified as an integer. Typically web servers listen on port 80; if you set it as that, you may need to run the server with `sudo`

## LogLevel ##
The verbosity level to run the server at; basically, the higher the number, the more stuff will be printed out to console

# Sample config.xml #

```
<?xml version="1.0" encoding="utf-8"?>
<settings version="1.0">
	<DocumentRoot>sailboat/www</DocumentRoot>
	<FileExtensions>html|htm</FileExtensions>
        <Port>7878</Port>
        <LogLevel>2</LogLevel>
</settings>
```