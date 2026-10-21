# MeshLink
MeshLink is a lightweight, zero-configuration peer-to-peer communication library designed for the Arduino UNO R3 and AVR microcontrollers using NRF24L01+ 2.4GHz wireless modules.
Key Features :

1. Simplified Initialization: Hides the boilerplate complexity of configuring NRF24 radio pipes, power amplifier levels, and dynamic payloads into a single .begin() call.
2. Dynamic Payload Handling: Automatically manages variable packet lengths up to the 32-byte NRF24 hardware limit.
3. Automatic Sender Identification: Prepends a node ID byte to every transmitted packet so receiving devices instantly know who sent the message without manual routing tables.
4. Non-Blocking Architecture: Uses an update() loop combined with clean C++ user callbacks to handle incoming radio packets smoothly without freezing the main program.
