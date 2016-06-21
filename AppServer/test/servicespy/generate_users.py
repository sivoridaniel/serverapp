#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

class UserGenerate:

	 def __init__(self, name, password, email):
            	self.name = name
               	self.password = password
        	self.email = email

  	 def reprJSON(self):
        	return dict(name=self.name,password = self.password,email = self.email) 

class UserUpdate:

	 def __init__(self, id, name, alias, password, sex, age, photo, interests, location, email):
            	self.id = id
		self.name = name
		self.sex = sex
		self.age = age
            	self.alias = alias
        	self.password = password
        	self.photo = photo
	        self.interests = interests
        	self.location = location
		self.email = email

  	 def reprJSON(self):
        	return dict(id=self.id,name=self.name,alias=self.alias,password = self.password,sex=self.sex,age=self.age,photo = self.photo, 
                            interests = self.interests,location=self.location, email = self.email) 

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

#EMPIEZA EL SCRIPT 

#requests.delete("https://shared-server-match.herokuapp.com/users")

#GENERATE NEW USER 1

name='Pablo Sivori'
password='1234'
email='psivori12@gmail.com' 

headers = {"content-type": "application/json"}
data = UserGenerate(name,password,email)
data = "{\"user\":"+json.dumps(data.reprJSON())+"}"

r = requests.post("http://200.16.116.146:3000/user/newuser", data = data, headers = headers, stream=True)
print r.text
result = json.loads(r.text)
id1=result['data']
token = r.headers.get('Token')
#print token	

#UPDATE USER

location = "12.34;12.45"
locations = location.split(';')

location = Location(locations[0],locations[1])

vec_interests = []
interes1 = Interest('Bandas','Divididos')
interes2 = Interest('Clubs','River Plate')
vec_interests.append(interes1)
vec_interests.append(interes2)

data = UserUpdate(id1, name,"psivori",password,"Masculino","30","/users/"+id1+"/photo", vec_interests,location,email)
data = '{\"user\":'+json.dumps(data.reprJSON(), cls=ComplexEncoder)+'}'
print data
headers = {"content-type": "application/json", "token":token}
r = requests.put("http://200.16.116.146:3000/user/updateuser", data = data, headers = headers)
print r.text
#GENERATE NEW USER 2

name='Alejandra Diaz'
password='1234'
email='chachi@gmail.com' 

headers = {"content-type": "application/json"}
data = UserGenerate(name,password,email)

data = "{\"user\":"+json.dumps(data.reprJSON())+"}"

r = requests.post("http://200.16.116.146:3000/user/newuser", data = data, headers = headers, stream=True)
print r.text
result = json.loads(r.text)
id2=result['data']
token = r.headers.get('Token')	

#UPDATE USER

data = UserUpdate(id2, name,"chachi",password,"f","30","/users/"+id2+"/photo", vec_interests,location,email)

data = '{\"user\":'+json.dumps(data.reprJSON(), cls=ComplexEncoder)+'}'

headers = {"content-type": "application/json", "token":token}
r = requests.put("http://200.16.116.146:3000/user/updateuser", data = data, headers = headers)
print r.text

#MATCH

#python new_match -u id1
