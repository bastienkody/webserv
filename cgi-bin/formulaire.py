#!/usr/bin/env python3

import os
import sys
import cgi

# L'en-tête HTTP pour spécifier le type de contenu de la réponse
# print("Content-type: text/html\n")

# Récupérer la longueur du contenu de la requête POST à partir de la variable d'environnement
content_length = os.environ.get('CONTENT_LENGTH')

if content_length:
    # Lire les données envoyées dans le corps de la requête
    content_length = int(content_length)
    post_data = sys.stdin.read(content_length)

    # Utiliser le module cgi pour extraire les données en tant que paires clé-valeur
    form = cgi.FieldStorage(fp=sys.stdin, environ=os.environ, keep_blank_values=True)

    # Récupérer les valeurs des champs du formulaire
    name = form.getvalue('name')
    age = form.getvalue('age')

    # Générer la réponse HTML
    print("<html><body>")
    print(f"<h1>Bonjour, {name}!</h1>")
    print(f"<p>Vous avez {age} ans.</p>")
    print("</body></html>")

else:
    # Si la requête POST ne contient pas de données
    print("<html><body><h1>Erreur</h1>")
    print("<p>Aucune donnée reçue.</p>")
    print("</body></html>")
