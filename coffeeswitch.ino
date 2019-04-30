#include <ESP8266WiFi.h>
#include "secrets.h"
#include <WiFiUdp.h>
#include <Servo.h>

Servo servo;
WiFiUDP Udp;
int angleActive = 70;
int angleResting = 120;
int angleCurrent = 120;
char incomingPacket[33];

void setup() {
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    servo.attach(D1);

    Udp.begin(8000);

    servo.write(angleCurrent);
}

void loop() {
    int packetSize = Udp.parsePacket();
    if (packetSize == 33) {
        Udp.read(incomingPacket, 33);
        if (strncmp(incomingPacket, secret, 32) == 0) {
            if (incomingPacket[32] == '+') {
                angleCurrent += 2;
                angleActive += 2;
                angleResting += 2;
            } else if (incomingPacket[32] == '-') {
                angleCurrent -= 2;
                angleActive -= 2;
                angleResting -= 2;
            } else if (incomingPacket[32] == '1') {
                angleCurrent = angleActive;
            }
            servo.write(angleCurrent);
            if (angleCurrent == angleActive) {
                delay(250);
                angleCurrent = angleResting;
                servo.write(angleCurrent);
            }
        }
    }
}