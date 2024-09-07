#!/usr/bin/python3

# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

html = '''
<html>
<head>
<title>Reponse Calcul</title>
</head>
<body>
<h2>Error Calcul</h2>
</body>
</html>'''

# Get data from fields
calcul = form.getvalue('calcul')
# print(calcul)
# if len(calcul == 0) :
# 	print (html)



print ("<html>")
print ('<head>')
print ("<title>Reponse Calcul</title>")
print ('</head>')
print ('<body>')
print ("<h2>%s = ...</h2>" % (calcul))
print ('</body>')
print ('</html>')