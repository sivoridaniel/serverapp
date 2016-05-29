import argparse
import requests
import json
import sys

class Message:

	 def __init__(self, user1, user2, idMessage):
            	self.user1 = user1
               	self.user2 = user2
        	self.idMessage = idMessage

  	 def reprJSON(self):
        	return dict(idFrom=self.user1,idTo = self.user2,idMessage = self.idMessage) 

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de update last message seen http://localhost:3000/chat/last')
        parser.add_argument('-u1','--user1',type=str,help='Nombre de usuario1', required = True)
        parser.add_argument('-u2','--user2',type=str,help='Nombre de usuario2', required = True)
        parser.add_argument('-m','--idmessage',type=str,help='Id ultimo mensaje visto', required = True)

        args = parser.parse_args()
        user1 = args.user1
        user2 = args.user2
	idmessage = args.idmessage

        return user1,user2,idmessage 

user1, user2, idmessage = get_args()

data = Message(user1,user2,idmessage)

data = json.dumps(data.reprJSON())

headers = {"content-type": "application/json"}

r = requests.put("http://localhost:3000/chat/last", data = data, headers = headers)

try: 
    assert( r.status_code == 200 ),"ERROR LLAMANDO A LAST MESSAGE SEEN"
    print r.json()
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = json.loads(r.text)
    print data


