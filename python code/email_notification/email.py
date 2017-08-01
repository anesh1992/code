#!/usr/local/bin/python
import smtplib
import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BOARD)

#define the pin that goes to the circuit
powerPin = 37
inputPin = 35
prev_input = 0
mailNum = 0
GPIO.setup(powerPin, GPIO.OUT)
GPIO.output(powerPin, GPIO.HIGH)

GPIO.setup(inputPin, GPIO.IN)

mail = smtplib.SMTP('smtp.gmail.com', 587)
mail.ehlo()
mail.starttls()
mail.login('sherryaujla3@gmail.com',2269781603)

try:
        while True:
                input = GPIO.input(inputPin)
                if ((not prev_input) and input):
                        if input == 1:
                                print("open")
                                mailNum += 1
                                content = 'Hello Maninder, \nYou have got a new mail ' + str (mailNum)
                                mail.sendmail('sherryaujla3@gmail.com','mdhanoa550@gmail.com', content)
                #update previous input
                prev_input = input
                sleep (2)
except:
        print "Error or exception occurred!"
finally:
        GPIO.cleanup()