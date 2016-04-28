import requests

r = requests.get("http://localhost:3000/login_user", data = {"name":"user","password":"1234"})

if r.status_code == 200:
    print r.text
