# CodeCellBleJoypad
Reads Pitch, Roll, Yaw from the Codecell's On-board IMU - Maps it to axes on a bluetooth joypad.

Also reads an analog sensor on GPIO2 or GPIO3 and maps that to joypad "throttle"

Analog sensors can be noisy, so the sketch can take multiple samples to average out the readings.

Beware of gimbal lock - Use quaternions if you wanna "do it right" (TM) - But good luck mapping those to joypad axes... Actually, that would be kinda fun to see. Please do it!

# Testing gamepad in windows:

```
%SystemRoot%\System32\joy.cpl
```

## Libraries 
[ESP32-BLE-Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad)
[CodeCell](https://github.com/microbotsio/CodeCell) 

## Board 
[CodeCell](https://github.com/microbotsio/CodeCell) C3 

### Codecell pinout
<img src="https://moore.dk/codecellPinout.webp" width="50%">



