# importation de la bibliothèque GPIO
import RPi.GPIO as GPIO
import mysql.connector
import time
from datetime import datetime

# définition des broches
bouton = 5
led = 3
buzzer = 11

# configuration de la connexion MySQL
db = mysql.connector.connect(
   host="localhost",
   user="root",
   password="root",
   database="myBDD"
)

try:
    while True:
        cursor = db.cursor()

        # configuration de la bibliothèque GPIO
        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(bouton, GPIO.IN)
        GPIO.setup(led, GPIO.OUT)
        GPIO.setup(buzzer, GPIO.OUT)

        # initialisation de l'état du bouton
        etatbouton = GPIO.input(bouton)

        if etatbouton == GPIO.LOW:  # Vérifiez si le bouton est enfoncé (LOW)
            # mesure 1
            temp1 = time.clock()
            time.sleep(2)

            while etatbouton != GPIO.LOW:  # Vérifiez si le bouton est enfoncé (LOW)
                # le bouton est enfoncé
                GPIO.output(led, GPIO.HIGH)  # LED allumée
                GPIO.output(buzzer, GPIO.HIGH)  # buzzer allumé

                # mesure 2
                temp2 = time.clock()
                Duration = temp2 - temp1
                Type = 1

                # Ajout de données dans la base de données
                req = "INSERT INTO TEST_BOARD (DATE,DUARATION) VALUES (NOW(),%s,%s);"%(Duration,Type)
                cursor.execute(req)
                db.commit()  # Commit pour sauvegarder les changements

                # l'alarme est acquittée
                GPIO.output(led, GPIO.LOW)  # LED off
                GPIO.output(buzzer, GPIO.LOW)  # buzzer off

    # attente courte pour éviter la surcharge du processeur
    time.sleep(0.1)
except KeyboardInterrupt:
    GPIO.cleanup()