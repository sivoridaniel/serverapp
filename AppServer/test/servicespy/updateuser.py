#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

class User:

	 def __init__(self, id, name, alias, password, photo, interests, location, mail):
            	self.id = id
		self.name = name
            	self.alias = alias
        	self.password = password
        	self.photo = photo
	        self.interests = interests
        	self.location = location
		self.mail = mail

  	 def reprJSON(self):
        	return dict(id=self.id,name=self.name,alias=self.alias,password = self.password,photo = self.photo, 
                            interests = self.interests,location=self.location, mail = self.mail) 

class Interest:

	 def __init__(self, category, value):
		self.category = category
		self.value = value

	 def reprJSON(self):
                return dict(category=self.category,value=self.value)

class Location:

	 def __init__(self, latitude, longitude):
                self.latitude = latitude
                self.longitude = longitude

         def reprJSON(self):
                return dict(latitude=self.latitude,longitude=self.longitude)

class ComplexEncoder(json.JSONEncoder):
	 def default(self, obj):
        	if hasattr(obj,'reprJSON'):
            		return obj.reprJSON()
        	else:
            		return json.JSONEncoder.default(self, obj)


def get_args():

	parser = argparse.ArgumentParser('Llamada a creación de nuevo usuario http://localhost:3000/user/newuser')
      	parser.add_argument('-n','--name',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-a','--alias',type=str,help='Alias del usuario')
	parser.add_argument('-i','--interests',type=str,help='Intereses. Categoria1,Valor1;Categoria2,Valor2;...;CategoriaN,ValorN')
	parser.add_argument('-p','--password',type=str,help='Password')
	parser.add_argument('-l','--location',type=str,help='Latitud;Longitud', required = True)
        parser.add_argument('-id','--id',type=str,help='Id usuario', required = True)
	parser.add_argument('-f','--foto',type=str,help='Url de la foto')
	parser.add_argument('-e','--mail',type=str,help='Correo electronico')
	
	args = parser.parse_args()
	id_ = args.id
	name = args.name
	alias = args.alias
	password = args.password
	photo = args.foto
	interests = args.interests
	location = args.location
	mail = args.mail
	
	return id_, name, alias, password, photo, interests, location, mail

id_, name, alias, password, photo, interests, location, mail = get_args()

locations = location.split(';')

location = Location(locations[0],locations[1])

if(interests is not None):
	intereses = interests.split(';')

vec_interests = []

if(interests is not None):
	for value in intereses:
		valores = value.split(',')
		interes = Interest(valores[0],valores[1])
		vec_interests.append(interes)

data = User(id_, name,alias,password,photo, vec_interests,location,mail)

data = '{\"user\":'+json.dumps(data.reprJSON(), cls=ComplexEncoder)+'}'

headers = {"content-type": "application/json"}

r = requests.put("http://localhost:3000/user/updateuser", data = data, headers = headers)

try:
    print r.text 
    sys.exit(1)
    assert( r.success == 'true' ),"ERROR LLAMADA CREANDO NUEVO USUARIO"
    data = json.loads(r.text)
    print data

except AssertionError, e:
    print 'NOK: %s'%e
