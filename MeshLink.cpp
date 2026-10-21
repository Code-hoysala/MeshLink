#include "MeshLink.h"

#include <string.h>

MeshLink::MeshLink(uint16_t cePin, uint16_t csnPin, uint8_t nodeId) : _radio(cePin, csnPin) {
    _nodeId = nodeId;
    _userCallback = nullptr;
}

bool MeshLink::begin(DataCallback callback) {
    _userCallback = callback;

    if (!_radio.begin()) {
        return false;
    }

    _radio.setPALevel(RF24_PA_LOW);
    _radio.enableDynamicPayloads();
    _radio.setAutoAck(true);
    _radio.setRetries(15, 15);
    _radio.openReadingPipe(1, 0xF0F0F0F0E1LL);
    _radio.openWritingPipe(0xF0F0F0F0E1LL);
    _radio.startListening();

    return true;
}

bool MeshLink::broadcast(const uint8_t *data, uint8_t len) {
    if (data == nullptr || len == 0 || len > MESHLINK_MAX_PAYLOAD) {
        return false;
    }

    _radio.stopListening();

    uint8_t buffer[MESHLINK_MAX_PACKET_SIZE];
    buffer[0] = _nodeId;
    memcpy(buffer + MESHLINK_HEADER_SIZE, data, len);

    bool success = _radio.write(buffer, len + MESHLINK_HEADER_SIZE);

    _radio.startListening();
    return success;
}

void MeshLink::update() {
    if (_radio.available()) {
        uint8_t buffer[MESHLINK_MAX_PACKET_SIZE];
        uint8_t len = _radio.getDynamicPayloadSize();

        if (len >= MESHLINK_HEADER_SIZE && len <= sizeof(buffer)) {
            _radio.read(buffer, len);

            uint8_t senderId = buffer[0];
            uint8_t payloadLen = len - MESHLINK_HEADER_SIZE;

            if (senderId != _nodeId && payloadLen <= MESHLINK_MAX_PAYLOAD && _userCallback != nullptr) {
                _userCallback(senderId, buffer + MESHLINK_HEADER_SIZE, payloadLen);
            }
        }
    }
}
