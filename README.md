# CodeCellBleJoypad
Reads Pitch, Roll, Yaw from the Codecell's On-board IMU - Maps it to axes on a bluetooth joypad.

Also reads an analog sensor on GPIO2 or GPIO3 and maps that to joypad "throttle"

Analog sensors can be noisy, so the sketch can take multiple samples to average out the readings.

## Libraries 
[ESP32-BLE-Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad)
[CodeCell](https://github.com/microbotsio/CodeCell) 

## Board 
[CodeCell](https://github.com/microbotsio/CodeCell) C3 

### Codecell pinout
<img src="https://moore.dk/codecellPinout.webp" width="50%">


