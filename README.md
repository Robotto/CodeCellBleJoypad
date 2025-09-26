Board: Codecell C3. 

<img src="https://moore.dk/codecellPinout.webp" width="50%">


Reads Pitch, Roll, Yaw from the Codecell's On-board IMU - Maps it to axes on a bluetooth joypad

Also reads an analog sensor on GPIO2 or GPIO3 and maps that to joypad "throttle"

Analog sensors can be noisy, so the sketch can take multiple samples to average out the readings
