'''
 ----------------------------------------------------------------------
 		Home Security System
 -----------------------------------------------------------------------
 @ Author 		: Anesh Kumar
 @ Email		: anesh_kumar@yahoo.com
 @ Version		:
 @ Date			: April, 2017
 @ Brief		: Once PIR sensor detects, RaspberryPi camera takes  
 			  picture and upload to dropbox.			
'''

import RPi.GPIO as GPIO
import time
import urllib2
import json
import picamera
import dropbox
import io

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(10,GPIO.OUT)
GPIO.setup(21,GPIO.IN)

temp1 = 0
temp2 = 0
while True:
        input_state = GPIO.input(21)
        temp1 = input_state

        if temp1 != temp2:
                GPIO.output(10,1)
                print ('Motion Detected')
                time.sleep(2)
                GPIO.output(10,0)
                time.sleep(2)

                dbx = dropbox.Dropbox('API_KEY')
                stream = io.BytesIO()

                with picamera.PiCamera() as camera:
                    camera.resolution = (1280,960)
                    time.sleep(2)
                    camera.capture(stream,format='jpeg')
                stream.seek(0)
                from PIL import Image
                im = Image.open(stream)
                dbx.files_upload(stream.getvalue(),"/test.jpg",mode=dropbox.files.WriteMode.overwrite)
                temp2 = temp1
