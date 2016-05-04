import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de new matches http://localhost:3000/match/newmatches')
        parser.add_argument('-u','--user',type=str,help='Nombre de usuario', required = True)
        
        args = parser.parse_args()
        user = args.user
        
        return user

user = get_args()

r = requests.get("http://localhost:3000/match/newmatches", data = {"idUser":user})

try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO AL NEW MATCHES"
    data = json.loads(r.text)
    result = data['result']
    assert( result != 'failed'),"ERROR DE NEW MATCHES %s"%result
    print 'status ok %s'%result
except AssertionError, e:
    print 'NOK: %s'%e

