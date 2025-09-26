/*
 * Board: Codecell C3
 * Reads Pitch, Roll, Yaw from the Codecell's On-board IMU - Maps it to axes on a bluetooth joypad
 * Also reads an analog sensor on GPIO2 or GPIO3 and maps that to joypad "throttle"
 * Analog sensors can be noisy, so the sketch can take multiple samples to average out the readings
 * 
 */

#include <Arduino.h>
#include <BleGamepad.h>
#include <CodeCell.h>

CodeCell myCodeCell; //using the codecell library to interface the on board IMU

float Roll = 0.0;
float Pitch = 0.0;
float Yaw = 0.0;


//BleGamepad bleGamepad;
BleGamepad bleGamepad("en form for gamepad", "orksat.me", 100); // Set custom device name, manufacturer and initial battery level
BleGamepadConfiguration bleGamepadConfig;                          // Create a BleGamepadConfiguration object to store all of the options

const int delayBetweenHIDReports = 5; // Additional delay in milliseconds between HID reports
const int analogSensorPin = 2;

void setup()
{
    Serial.begin(115200);
    myCodeCell.Init(MOTION_ROTATION); //Initializes Rotation Sensing
    Serial.println("Starting BLE work!");
    //bleGamepad.begin();
    bleGamepadConfig.setAutoReport(true);
    bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD); // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
    bleGamepadConfig.setButtonCount(0);
    bleGamepadConfig.setHatSwitchCount(0);
    bleGamepadConfig.setWhichSpecialButtons(false, false, false, false, false, false, false, false);
    bleGamepadConfig.setWhichAxes(false, false, false, true, true, true, false, false); //rotation around X,Y,Z used for 1
    bleGamepadConfig.setIncludeThrottle(true); //Throttle used for analog sensor

    bleGamepadConfig.setVid(0xe502);
    bleGamepadConfig.setPid(0xabcd);
    
    bleGamepadConfig.setModelNumber("1.0");
    bleGamepadConfig.setSoftwareRevision("Software Rev 1");
    bleGamepadConfig.setSerialNumber("24404972");
    bleGamepadConfig.setFirmwareRevision("2.1");
    bleGamepadConfig.setHardwareRevision("0.5");
    
    // Some non-Windows operating systems and web based gamepad testers don't like min axis set below 0, so 0 is set by default
    //bleGamepadConfig.setAxesMin(0x8001); // -32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
  	bleGamepadConfig.setAxesMin(0x0000); // 0 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal    
    bleGamepadConfig.setAxesMax(0x7FFF); // 32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal     
    
    bleGamepad.begin(&bleGamepadConfig); // Begin gamepad with configuration options
    
}
int16_t adjustedpitch=0;
int16_t adjustedroll=0;
int16_t adjustedyaw=0;
int16_t adjustedthrottle=0;

float filterAlpha = 10.0;
float filteredADC = 512.0;

void loop()
{
    if (bleGamepad.isConnected())
    {
        if (myCodeCell.Run(10)) {  //Run every 10Hz
        myCodeCell.Motion_RotationRead(Roll, Pitch, Yaw);
        Serial.printf("Roll: %.2f°, Pitch: %.2f°, Yaw: %.2f°\n", Roll, Pitch, Yaw);
        
        filteredADC = (filteredADC*filterAlpha+analogRead(analogSensorPin))/(filterAlpha+1.0);

        // Multiply angular measurements by 10 so we don't throw away precision, then add 0.5 for correct rounding when casting to integer.
        // This means the range we're mapping from goes from 0 to 3599 (representing 0-359.9 degrees)

        // example: 11.98 * 10 + 0.5 = 119.8 + 0.5 = 120.3 => int(120.3) = 120 (representing 12.0 degrees)
        // example: 11.94 * 10 + 0.5 = 119.4 + 0.5 = 119.9 => int(119.9) = 119 (representing 11.9 degrees)
        // example: 10.44 * 10 + 0.5 = 104.4 + 0.5 = 104.9 => int(104.9) = 104 (representing 10.4 degrees)

        adjustedpitch = map(int(Pitch*10+0.5), 0, 3599, 32737, 0); 
        adjustedroll = map(int(Roll*10+0.5), 0, 3599, 32737, 0);
        adjustedyaw = map(int(Yaw*10+0.5), 0, 3590, 32737, 0);
        // Map analog reading from 0 ~ 4095 to 32737 ~ 0 for use as an axis reading
        adjustedthrottle = map(int(filteredADC+0.5),0,4095,32737,0); 
        }
        
        // Update throttle axis and auto-send report
        bleGamepad.setAxes(0,0,0,adjustedpitch,adjustedroll,adjustedyaw);
        bleGamepad.setThrottle(adjustedthrottle);
        
        
        delay(delayBetweenHIDReports);


   }
}