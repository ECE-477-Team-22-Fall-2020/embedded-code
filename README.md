# src
This is the embedded C code which runs on the "Social Distancing Chess" board. It does not include any board-specific libraries, making it easier to use
with any of the STM32F4 family of boards.

This code primarily covers:
* Piece detection and tracking by interfacing with our custom PCBs
* Bidirectional bluetooth communication
* Screen updates based on piece and bluetooth data
