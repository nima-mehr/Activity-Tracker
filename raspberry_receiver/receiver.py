import time
import spidev
import RPi.GPIO as GPIO
from RF24 import RF24, RF24_PA_LOW

# CE and CSN pins
CE_PIN = 25
CSN_PIN = 0

# Initialize radio
radio = RF24(CE_PIN, CSN_PIN)
pipes = [b"00001"]

radio.begin()
radio.setPALevel(RF24_PA_LOW)
radio.setDataRate(RF24.RF24_1MBPS)
radio.openReadingPipe(1, pipes[0])
radio.startListening()

print("Listening for messages...")

try:
    while True:
        if radio.available():
            received_payload = radio.read(radio.getDynamicPayloadSize())
            try:
                message = received_payload.decode('utf-8')
                print("Received:", message)
            except Exception as e:
                print("Decode error:", e)
        time.sleep(0.1)

except KeyboardInterrupt:
    print("\nReceiver stopped.")
    GPIO.cleanup()
