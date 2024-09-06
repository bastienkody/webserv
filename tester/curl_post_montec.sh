# post a whole file (-d vs --data-binary --> preserve escaped character)
curl -X "POST" --data-binary @montec.html 127.0.0.1:8080/res

# post some data with -d
curl -d "yooooo" 127.0.0.1:8080/file
