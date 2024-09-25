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

# while (1):
# 	a = 1

if calcul == None :
	print(html)
	exit(0)

size = len(calcul)
for i in range (1, size) :
	if (calcul[i] == '+' or calcul[i] == '-' or calcul[i] == '*' or calcul[i] == '/') :
		break
operator = calcul[i]
nb1 = int(calcul[0:i], base=10)
nb2 = int(calcul[i + 1:size], base=10)

if (operator == '+'):
	res = nb1 + nb2
elif (operator == '-'):
	res = nb1 - nb2
elif (operator == '*'):
	res = nb1 * nb2
elif (operator == '/'):
	if (nb2 == 0):
		print(html)
		exit(0)
	else:
		res = nb1 / nb2

print ("<html>")
print ('<head>')
print ("<title>Reponse Calcul</title>")
print ('</head>')
print ('<body>')
print ("<h2> " + calcul + " = %s </h2>" % (res))
print ('</body>')
print ('</html>')