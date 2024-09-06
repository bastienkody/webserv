#!/usr/bin/python3

# Import modules for CGI handling
import cgi, cgitb


import os
# print("Content-type: text/plain\n")
# print(os.environ)


# Create instance of FieldStorage
form = cgi.FieldStorage()

# Afficher toutes les clés et valeurs contenues dans 'form'
print("<html><body>")
print("<h1>Donnees recues</h1>")
print("<ul>")

# Parcourir toutes les clés et afficher les valeurs associées
for key in form.keys():
    value = form.getvalue(key)
    print("<p>test</p>")
    print(f"<li><strong>{key}</strong>: {value}</li>")

print("</ul>")
print("</body></html>")

# Get data from fields
calcul = form.getvalue('calcul')


print ("<html>")
print ('<head>')
print ("<title>Reponse Calcul</title>")
print ('</head>')
print ('<body>')
print ("<h2>%s = ...</h2>" % (calcul))
print ('</body>')
print ('</html>')