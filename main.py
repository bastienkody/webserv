import requests
import time

url1 = "http://127.0.0.1:8080/truc"
url2 = "http://127.0.0.1:8081/machin"
for i in range(0, 200):
    url = url2
    if i % 2:
        url = url1
    r = requests.get(url)
    print(f"Response code: {r.status_code} {r.reason}")
    print(f"Response headers: {r.headers}")
    print("Response body:")
    print(r.text)
    print("###################################")
    #time.sleep(0.2)
