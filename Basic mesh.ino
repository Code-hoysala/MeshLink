#include <SPI.h>
#include <RF24.h>
#include <MeshLink.h>

MeshLink mesh(9, 10, 1);

void onDataReceived(uint8_t senderId, const uint8_t *data, uint8_t len) {
    Serial.print("Received from Node #");
    Serial.print(senderId);
    Serial.print(" | Payload: ");
    for (int i = 0; i < len; i++) {
        Serial.print((char)data[i]);
    }
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!mesh.begin(onDataReceived)) {
        Serial.println("NRF24 initialization failed!");
        while (1);
    }

    Serial.println("UNO MeshLink active!");
}

void loop() {
    mesh.update();

    static unsigned long lastSend = 0;
    if (millis() - lastSend > 5000) {
        lastSend = millis();
        const char *msg = "Hello from UNO!";
        mesh.broadcast((const uint8_t *)msg, strlen(msg));
        Serial.println("Broadcasted message.");
    }
}
