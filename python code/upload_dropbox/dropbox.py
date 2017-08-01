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

def sendNotification(token, channel, message):
        data = {
                "body" : message,
                "message_type" : "text/plain"
        }

        req = urllib2.Request('http://api.pushetta.com/api/pushes/{0}/'.format(channel))
        req.add_header('Content-Type', 'application/json')
        req.add_header('Authorization', 'Token {0}'.format(token))
        response = urllib2.urlopen(req, json.dumps(data))

temp1 = 0
temp2 = 0
while True:
        input_state = GPIO.input(21)
        temp1 = input_state

        if temp1 != temp2:
                GPIO.output(10,1)
                print ('Motion Detected')
                sendNotification("f62aa951224bd27b6f4a4259ad68f9c661e14efb","Secure Home", "Intruder Detected")
                time.sleep(2)
                GPIO.output(10,0)
                time.sleep(2)

                dbx = dropbox.Dropbox('5MWqwKTnIiAAAAAAAAAACeKNZEKg_P6hsLCldsHpB0uL2AdjcUf2w4M2h9IT3uts')

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