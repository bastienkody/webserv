import requests

var1= "a=AAR\n"
var2= "b=BAR\n"
var3= "c=CAR\n"
var4= "d=DAR\n"
var5= "e=EAR\n"

def gen():
    yield var1.encode()
    yield var2.encode()
    yield var3.encode()
    yield var4.encode()
    yield var5.encode()

# without connection:close the script keeps on running, but the server is not blocked 
r = requests.post('http://127.0.0.1:8080/chunk.html', data=gen(), headers={"Connection":"close"})

print(f"Response code: {r.status_code} {r.reason}")
print(f"Response headers: {r.headers}")
print("Response body:")
print(r.text)
