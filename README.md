INTERNET OF THINGS

## Introduction
This is a project for the Internet of Things course at the University of Pisa. The project consists in the development of a system that allows the user to monitor the temperature and humidity of a room and to control the lighting system of the same room. The system is composed of a Raspberry Pi 3 Model B, a DHT11 sensor and a relay module. The Raspberry Pi is connected to the sensor and the relay module through the GPIO pins. The Raspberry Pi is also connected to the Internet through an Ethernet cable. The user can interact with the system through a web application that allows him to monitor the temperature and humidity of the room and to turn on and off the light. The web application is hosted on a server that is also connected to the Internet. The Raspberry Pi communicates with the server through a REST API. The web application is developed using the React framework. The server is developed using the Node.js framework. The communication between the Raspberry Pi and the server is implemented using the MQTT protocol. The communication between the web application and the server is implemented using the HTTP protocol.