#!/usr/bin/python3

# import cgi

content = '<html>'
content += '<head>'
content += '<title>Hello World - First CGI Program</title>'
content += '</head>'
content += '<body>'
content += '<h2>Hello World! This is my first CGI program</h2>'
content += '</body>'
content += '</html>'
print ("Content-type: text/html\n")
print ("Content-length: ", len(content), "\n\n")
print (content)