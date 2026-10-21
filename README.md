# MeshLink

MeshLink is a lightweight Arduino library for exchanging short peer-to-peer packets over NRF24L01+ radios.

## Key features

1. Simple initialization: one `begin()` call configures the radio.
2. Dynamic payload support: handles variable-length packets up to the NRF24 hardware limit.
3. Sender identification: every transmitted packet includes the sender node ID.
4. Non-blocking updates: use the `update()` loop to process incoming packets without freezing the sketch.

## Important protocol limit

Each NRF24 packet can carry a maximum of 32 bytes total.

This library reserves 1 byte for the sender ID, so the maximum user payload is 31 bytes.

- Maximum payload size: `31` bytes
- Total packet size: `32` bytes

## Installation

1. Download or clone this repository.
2. Move the folder into your Arduino `libraries` directory.
3. Restart the Arduino IDE.
4. Include `#include <MeshLink.h>` in your sketch.

## Example

```cpp
#include <MeshLink.h>

MeshLink mesh(9, 10, 1);

void onReceive(uint8_t senderId, const uint8_t *data, uint8_t len) {
    Serial.print("Received from node ");
    Serial.println(senderId);
}

void setup() {
    Serial.begin(115200);
    mesh.begin(onReceive);
}

void loop() {
    mesh.update();
    const char *msg = "Hello";
    mesh.broadcast((const uint8_t *)msg, strlen(msg));
    delay(5000);
}
```

## License

This project is provided as-is for educational and personal use.
