# 📡 Minimal Activity Tracker (Arduino + Raspberry Pi + NRF24L01)

This open-source project is a lightweight activity tracker that detects motion using an MPU6050 accelerometer and sends data wirelessly via NRF24L01 modules to a Raspberry Pi receiver. The Raspberry Pi displays real-time activity status like `IDLE` or `MOVING` over serial.

> ❗️Cloud features like Blynk integration were tested but are not included in this version.

---

## 🔧 Features

- Motion detection using MPU6050
- Wireless transmission via NRF24L01
- Real-time monitoring on Raspberry Pi
- Expandable for cloud or notification systems

---

## 🧰 Hardware Used

| Component         | Qty |
|------------------|-----|
| Arduino Uno/Nano | 1   |
| MPU6050          | 1   |
| NRF24L01         | 2   |
| Raspberry Pi 4B  | 1   |
| Jumper Wires     | —   |
| Breadboard       | —   |

---

## ⚙️ Wiring

### 🔌 Arduino Sender (MPU6050 + NRF24L01)

| Component    | Pin Name   | Arduino Pin |
|--------------|------------|--------------|
| **MPU6050**  | VCC        | 3.3V         |
|              | GND        | GND          |
|              | SDA        | A4           |
|              | SCL        | A5           |
| **NRF24L01** | VCC        | 3.3V         |
|              | GND        | GND          |
|              | CE         | 9            |
|              | CSN        | 10           |
|              | SCK        | 13           |
|              | MOSI       | 11           |
|              | MISO       | 12           |

> ⚠️ Add a 10µF–47µF capacitor between VCC and GND near the NRF24L01 module.

---

### 🍓 Raspberry Pi Receiver (NRF24L01)

| NRF24L01 Pin | Raspberry Pi GPIO | Pi Physical Pin |
|--------------|-------------------|------------------|
| VCC          | 3.3V              | 1                |
| GND          | GND               | 6                |
| CE           | GPIO25            | 22               |
| CSN          | SPI CE0 (GPIO8)   | 24               |
| SCK          | SPI SCLK (GPIO11) | 23               |
| MOSI         | SPI MOSI (GPIO10) | 19               |
| MISO         | SPI MISO (GPIO9)  | 21               |

---

## 🚀 Getting Started

### 1. Arduino Setup

1. Wire up the MPU6050 and NRF24L01.
2. Upload the Arduino sketch in `/arduino_sender/sender.ino`.

### 2. Raspberry Pi Setup

1. Enable SPI:

```bash
sudo raspi-config
```
→ Go to Interface Options → Enable SPI → Reboot if prompted.

Install required libraries:
```bash
sudo apt-get update
sudo apt-get install -y python3-dev python3-pip libboost-python-dev libboost-thread-dev

git clone https://github.com/nRF24/RF24.git
cd RF24
sudo ./setup.py install
```
Wire the NRF24L01 to the Pi.

Run the Python script in /raspberry_receiver/receiver.py:
```bash
python3 receiver.py
```
📷 Example Output
```bash
Listening for messages...
Received: IDLE
Received: MOVING
Received: IDLE
```
🙌 Contributions
Feel free to fork, modify, and expand this project. Pull requests are welcome!
Once you're ready, upload the project to GitHub under a repository name like `activity-tracker`.
