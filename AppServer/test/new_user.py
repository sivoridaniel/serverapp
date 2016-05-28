import requests

r = requests.post("http://localhost:3000/user/newuser", data = {"name":"user","password":"1234","firstName":"Pablo","lastName":"Sivori","email":"sivori.daniel@gmail.com"})

if r.status_code == 200:
    print r.text
