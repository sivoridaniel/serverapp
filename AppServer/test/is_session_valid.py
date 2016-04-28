import self
import json
import requests

r = requests.get("http://localhost:3000/user/newuser", data = {"name":"user","password":"1234","firstName":"Pablo","lastName":"Sivori","email":"sivori.daniel@gmail.com"})

assert (r.status_code == 200),"LLAMADA AL NEW USER NOK"

if(r.status_code == 200):
     r = requests.get("http://localhost:3000/login_user", data = {"name":"user","password":"1234"})

assert(r.status_code == 200),"LLAMADA AL LOGIN NOK"

if(r.status_code == 200): 
         
       data = json.loads(r.text)

       if(data['result'] == 'status ok'):
           r = requests.get("http://localhost:3000/valid_session", data = {"token":data['token']})
           data = json.loads(r.text) 
           print(data['result'])
       else:
            print("ERROR EN LA LLAMADA AL VALID SESSION")
else:
	print("ERROR EN LLAMADA AL LOGIN")

