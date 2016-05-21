#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

class User:

	 def __init__(self, name, alias, password, photo, interests, location, mail):
            	self.name = name
            	self.alias = alias
        	self.password = password
        	self.photo = photo
	        self.interests = interests
        	self.location = location
		self.mail = mail

  	 def reprJSON(self):
        	return dict(name=self.name,alias=self.alias,password = self.password,photo = self.photo, interests = self.interests,
                            location=self.location, mail = self.mail) 

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
        parser.add_argument('-a','--alias',type=str,help='Alias del usuario',required= True)
	parser.add_argument('-i','--interests',type=str,help='Intereses. Categoria1,Valor1;Categoria2,Valor2;...;CategoriaN,ValorN')
	parser.add_argument('-p','--password',type=str,help='Password', required = True)
	parser.add_argument('-l','--location',type=str,help='Latitud;Longitud', required = True)
	parser.add_argument('-f','--foto',type=str,help='Url de la foto', required = False)
	parser.add_argument('-e','--mail',type=str,help='Correo electronico', required = True)
	
	args = parser.parse_args()
	name = args.name
	alias = args.alias
	password = args.password
	photo = args.foto
	interests = args.interests
	location = args.location
	mail = args.mail
	
	return name, alias, password, photo, interests, location, mail

name, alias, password, photo, interests, location, mail = get_args()

locations = location.split(';')

location = Location(locations[0],locations[1])

intereses = interests.split(';')

vec_interests = []

for value in intereses:
	valores = value.split(',')
	interes = Interest(valores[0],valores[1])
	vec_interests.append(interes)

data = User(name,alias,password,photo,vec_interests,location,mail)

data = '{\"user\":'+json.dumps(data.reprJSON(), cls=ComplexEncoder)+'}'

headers = {"content-type": "application/json"}

r = requests.get("http://localhost:3000/user/newuser", data = data, headers = headers)

try:
    assert( r.status_code == 200 ),"ERROR LLAMADA CREANDO NUEVO USUARIO"
    data = json.loads(r.text)
    result = data['result']
    assert( result == '200'),"ERROR CREANDO EL USUARIO %s"%result    
    print result

except AssertionError, e:
    print 'NOK: %s'%e
