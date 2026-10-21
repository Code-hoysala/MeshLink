#include "MeshLink.h"

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
    _radio.enableDynamicPayloads(); // Crucial: Enables variable-length packets
    _radio.openReadingPipe(1, 0xF0F0F0F0E1LL);
    _radio.openWritingPipe(0xF0F0F0F0E1LL);
    
    _radio.startListening();
    return true;
}

bool MeshLink::broadcast(const uint8_t *data, uint8_t len) {
    if (len > MESHLINK_MAX_PAYLOAD) return false;
    
    _radio.stopListening();
    
    uint8_t buffer[MESHLINK_MAX_PAYLOAD + 1];
    buffer[0] = _nodeId;
    memcpy(buffer + 1, data, len);

    bool success = _radio.write(buffer, len + 1);
    
    _radio.startListening();
    return success;
}

void MeshLink::update() {
    if (_radio.available()) {
        uint8_t buffer[MESHLINK_MAX_PAYLOAD + 1];
        uint8_t len = _radio.getDynamicPayloadSize();
        
        if (len > 0 && len <= sizeof(buffer)) {
            _radio.read(buffer, len);
            uint8_t senderId = buffer[0];
            
            if (senderId != _nodeId && _userCallback) {
                _userCallback(senderId, buffer + 1, len - 1);
            }
        }
    }
}
