import requests
import time

url1 = "http://127.0.0.1:8080/testhtml/simple.html"
url2 = "http://127.0.0.1:8080/test.html"
for i in range(0, 200):
    url = url2
    if i % 2:
        url = url1
    r = requests.get(url, headers={"Connection":"Keep-Alive"})

    print(f"Response code: {r.status_code} {r.reason}")
    print(f"Response headers: {r.headers}")
    print("Response body:")
    print(r.text)
    print(f"{i}###################################")
    # time.sleep(0.2)
