#!/usr/bin/python3

# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
name = form.getvalue('name')
age = form.getvalue('age')


print ("<html>")
print ('<head>')
print ("<title>Reponse Formulaire</title>")
print ('</head>')
print ('<body>')
print (f"<h1>Bonjour, {name} !</h1>")
print (f"<p>Vous avez {age} ans.</p>")
print ('</body>')
print ('</html>')