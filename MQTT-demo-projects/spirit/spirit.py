#!/usr/bin/env python3
import paho.mqtt.client as mqtt
import time
from perlin_noise import PerlinNoise
from random import randrange
from urllib.request import urlopen
import json

try:
    from settings import settings
except ImportError:
    print("WiFi settings are kept in settings.py, please add or change them there!")
    raise

def on_connect(client, userdata, flags, rc):
    # This will be called once the client connects
    print(f"Connected with result code {rc}")
    # Subscribe here!
    client.subscribe("my-topic")

def on_message(client, userdata, msg):
    print(f"Message received [{msg.topic}]: {msg.payload}")

client = mqtt.Client(settings["client_id"]) # client ID "mqtt-test"
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(settings["username"], settings["password"])
client.connect(settings["broker"], 1883)
client.loop_start()

def perlin_noise_step ():
	perlin_noise = PerlinNoise()
	perlin_broker_topic = "perlin"
	perlin = "0.5";
	noiseScale=0.02
	offset = 0.0
	speed = 0.8
	noise = perlin_noise(randrange(100)/100)
	return noise
	
import math

def iss_step():
	iss_url = "https://api.wheretheiss.at/v1/satellites/25544"
	ide_coords = [52.00207425711345, 4.369903741537897]
	ide_dist = "-1"
	iss_broker_topic = "iss"
	long = ""
	lat = ""
	
	def calculate_distance(lat1, lon1, lat2, lon2):
		def degree_2_radial(degree):
			return(degree * math.pi/180)
			
		r = 6372 # Radius of the earth in km
		d_lat = degree_2_radial(lat2 - lat1)
		d_lon = degree_2_radial(lon2 - lon1)
		a = math.sin(d_lat/2) * math.sin(d_lat/2) + math.cos(degree_2_radial(lat1)) * math.cos(degree_2_radial(lat2)) * math.sin(d_lon/2) * math.sin(d_lon/2)
		c = 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))
		d = r * c
		return d
		
	
	response = urlopen(iss_url)
	response_json = json.loads(response.read())
	
	distance = calculate_distance(ide_coords[0], ide_coords[1], response_json["latitude"], response_json["longitude"])
	return distance, response_json["latitude"], response_json["longitude"]
	

while True:
	distance, latitude, longitude = iss_step()
	client.publish("iss/distance", str(distance))
	client.publish("iss/location", str(latitude) + " " + str(longitude))
	
	noise = perlin_noise_step()
	client.publish("perlin", noise)
	time.sleep(30)

