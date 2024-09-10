#!/usr/bin/python3

import cgi
import os

# Définir le dossier où les fichiers seront stockés
UPLOAD_FOLDER = '/mnt/nfs/homes/mmuesser/Cursus42/webserv/www/cgi-bin/tmp'

# Activer la gestion des erreurs CGI (utile pour le développement)
import cgitb
cgitb.enable()

def save_uploaded_file():
    # Récupérer les données du formulaire
    form = cgi.FieldStorage()

    # Vérifier si le champ 'file' est présent dans le formulaire
    if 'file' not in form:
        print("<h1>Error</h1>")
        print("<p>No file field found in the form!</p>")
        return

    # Récupérer le fichier envoyé
    fileitem = form['file']
    print(fileitem)

    # Vérifier si un fichier a été sélectionné
    if not fileitem.filename:
        print("<h1>Error</h1>")
        print("<p>No file was selected for upload!</p>")
        return

    # Nettoyer le nom du fichier (supprimer les chemins éventuels)
    filename = os.path.basename(fileitem.filename)

    # Définir le chemin complet du fichier à sauvegarder
    filepath = os.path.join(UPLOAD_FOLDER, filename)
    # Sauvegarder le fichier
    try:
        file_data = fileitem.file.read()
        data_length = len(file_data)
        print(f"<p>Debug : Nombre d'octets lus : {data_length}</p>")

        # Écrire le contenu dans le fichier
        with open(filepath, 'wb') as f:
            bytes_written = f.write(file_data)
        print(f"<p>Debug : Nombre d'octets ecrits : {bytes_written}</p>")
        print("<h1>Success</h1>")
        print(f"<p>File '{filename}' successfully uploaded to {UPLOAD_FOLDER}.</p>")
    except Exception as e:
        print("<h1>Error</h1>")
        print(f"<p>Failed to upload file: {str(e)}</p>")

# Exécuter la fonction pour sauvegarder le fichier
save_uploaded_file()
