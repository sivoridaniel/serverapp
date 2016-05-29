import argparse
import requests
import json
import sys

class Message:

	 def __init__(self, user1, user2, message):
            	self.user1 = user1
               	self.user2 = user2
        	self.message = message

  	 def reprJSON(self):
        	return dict(idFrom=self.user1,idTo = self.user2,message = self.message) 

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de post message http://localhost:3000/chat/message')
        parser.add_argument('-u1','--user1',type=str,help='Nombre de usuario1', required = True)
        parser.add_argument('-u2','--user2',type=str,help='Nombre de usuario2', required = True)
        parser.add_argument('-m','--message',type=str,help='Mensaje a enviar', required = True)

        args = parser.parse_args()
        user1 = args.user1
        user2 = args.user2
	message = args.message

        return user1,user2,message 

user1, user2, message = get_args()

data = Message(user1,user2,message)

data = json.dumps(data.reprJSON())

headers = {"content-type": "application/json"}

r = requests.post("http://localhost:3000/chat/message", data = data, headers = headers)

try: 
    assert( r.status_code == 200 ),"ERROR LLAMANDO A POST MESSAGE"
    print r.json()
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = json.loads(r.text)
    print data


