#ifndef MESHLINK_H
#define MESHLINK_H

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define MESHLINK_HEADER_SIZE 1
#define MESHLINK_MAX_PAYLOAD 31
#define MESHLINK_MAX_PACKET_SIZE (MESHLINK_HEADER_SIZE + MESHLINK_MAX_PAYLOAD)

class MeshLink {
public:
    typedef void (*DataCallback)(uint8_t senderId, const uint8_t *data, uint8_t len);

    MeshLink(uint16_t cePin, uint16_t csnPin, uint8_t nodeId);

    bool begin(DataCallback callback);
    bool broadcast(const uint8_t *data, uint8_t len);
    void update();

private:
    RF24 _radio;
    uint8_t _nodeId;
    DataCallback _userCallback;
};

#endif
