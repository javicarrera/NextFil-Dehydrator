/*
This sketch shows examples on how to read and write the RTC and EEPROM included on the Nextion display enhanced models.

I could not find a way to read it directly, but using variables as the middle men we can read them.

I am going to use the official library to receive data, but I am not going to use it to send data to the display
because it can create problems with touch events when we send data in the loop.
I think it's easier to send data to the display without the library, anyway.

Connection with Arduino Uno/Nano:
* +5V = 5V
* TX  = pin 0 (RX)
* RX  = pin 1 (TX)
* GND = GND

If you are going to use an Arduino Mega, you have to edit everything on this sketch that says "Serial"
and replace it with "Serial1" (or whatever number you are using). Also define the Serial port on NexConfig.h
inside the nextion library.


Nextion library: https://github.com/itead/ITEADLIB_Arduino_Nextion


This sketch was made for my 3rd video tutorial shown here: https://www.youtube.com/edit?o=U&video_id=7Dkn3t_jjN8

Made by InterlinkKnight
Last update: 02/20/2018
*/




#include <Nextion.h>  // Include the nextion library (the official one) https://github.com/itead/ITEADLIB_Arduino_Nextion
                      // Make sure you edit the NexConfig.h file on the library folder to set the correct serial port for the display.
                      // By default it's set to Serial1, which most arduino boards don't have.
                      // Change "#define nexSerial Serial1" to "#define nexSerial Serial" if you are using arduino uno, nano, etc.



// Declare objects that we are going to read from the display. This includes buttons, sliders, text boxes, etc:
// Format: <type of object> <object name> = <type of object>(<page id>, <object id>, "<object name>");
/* ***** Types of objects:
 * NexButton - Button
 * NexDSButton - Dual-state Button
 * NexHotspot - Hotspot, that is like an invisible button
 * NexCheckbox - Checkbox
 * NexRadio - "Radio" checkbox, that it's exactly like the checkbox but with a rounded shape
 * NexSlider - Slider
 * NexGauge - Gauge
 * NexProgressBar - Progress Bar
 * NexText - Text box
 * NexScrolltext - Scroll text box
 * NexNumber - Number box
 * NexVariable - Variable inside the nextion display
 * NexPage - Page touch event
 * NexGpio - To use the Expansion Board add-on for Enhanced Nextion displays
 * NexRtc - To use the real time clock for Enhanced Nextion displays
 * *****
 */
NexButton b0 = NexButton(2, 1, "b0");  // Button added
NexButton b1 = NexButton(2, 3, "b1");  // Button added
NexVariable va33 = NexVariable(2, 5, "va33");  // Variable added so we can read it
NexVariable vah = NexVariable(2, 6, "vah");  // Variable added so we can read it
NexVariable vam = NexVariable(2, 7, "vam");  // Variable added so we can read it
NexVariable vas = NexVariable(2, 8, "vas");  // Variable added so we can read it


// End of declaring objects








// Declare touch event objects to the touch event list: 
// You just need to add the names of the objects that send a touch event.
// Format: &<object name>,

NexTouch *nex_listen_list[] = 
{
  &b0,  // Button added
  &b1,  // Button added
  NULL  // String terminated
};  // End of touch event list









////////////////////////// Touch events:
// Each of the following sections are going to run everytime the touch event happens:
// It's going to run the code inside each section only ones for each touch event.

void b0PushCallback(void *ptr)  // Press event for button b0
{
  uint32_t number4 = 0;  // Create variable to store value we are going to get
  vah.getValue(&number4);    // Read variable (inside the display) called vah and stored on variable number4

  // Now is going to send the value we received to object n1:
  Serial.print("n1.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(number4);  // This is the value you want to send to that object and atribute mentioned before.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);


  uint32_t number3 = 0;  // Create variable to store value we are going to get
  vam.getValue(&number3);  // Read variable (inside the display) called vam and stored on variable number3

  // Now is going to send the value we received to object n2:
  Serial.print("n2.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(number3);  // This is the value you want to send to that object and atribute mentioned before.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);


  uint32_t number2 = 0;  // Create variable to store value we are going to get
  vas.getValue(&number2);  // Read variable (inside the display) called vas and stored on variable number2

  // Now is going to send the value we received to object n3:
  Serial.print("n3.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(number2);  // This is the value you want to send to that object and atribute mentioned before.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);

}  // End of press event





void b1PushCallback(void *ptr)  // Press event for button b1
{
  uint32_t number5 = 0;  // Create variable to store value we are going to get
  va33.getValue(&number5);  // Read variable (inside the display) called va33 and stored on variable number5

  // Now is going to send the value we received to object n0:
  Serial.print("n0.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(number5);  // This is the value you want to send to that object and atribute mentioned before.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
}  // End of press event


////////////////////////// End of touch events











void setup() {  // Put your setup code here, to run once:
  
  Serial.begin(9600);  // Start serial comunication at baud=9600



  // Register the event callback functions of each touch event:
  // You need to register press events and release events seperatly.
  // Format for press events: <object name>.attachPush(<object name>PushCallback);
  // Format for release events: <object name>.attachPop(<object name>PopCallback);
  b0.attachPush(b0PushCallback);  // Button press
  b1.attachPush(b1PushCallback);  // Button press

  // End of registering the event callback functions




  
/*
  // Set RTC time and data:
  // This is an example of how to set the RTC from arduino.
  // It's on the setup section because I want to run it only ones.
  // rtc0 is year 2000 to 2099,
  // rtc1 is month 1 to 12,
  // rtc2 is day 1 to 31,
  // rtc3 is hour 0 to 23,
  // rtc4 is minute 0 to 59,
  // rtc5 is second 0 to 59,
  // rtc6 is day of the week 0 to 6 (Sunday=0, Saturday=6). It's not really needed to set this because it's calculated automaticly.

  Serial.print("rtc0=2000");  // This is sent the year.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("rtc1=12");  // This is sent the month.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("rtc2=6");  // This is sent the day.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
  
  Serial.print("rtc3=3");  // This is sent the hour.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("rtc4=24");  // This is sent the minute.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("rtc5=0");  // This is sent the second.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);


  

  
  // Write to the EEPROM of the Nextion display:
  // The following it's going to set the brightness to 55 by writing into the EEPROM.
  // It's on the setup section because I want to run it only ones.
  // You are going to need to load the page 0 after arduino sends the instruction (after booting), so the display
  // can read the EEPROM and set the brightness to whatever value the EEPROM has.

  Serial.print("wepo 55,10");  // Write a value of 55 on the EEPROM in addresses 10 to 13
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
*/






}  // End of setup





void loop() {  // Put your main code here, to run repeatedly:


  delay(10);  // This is the only delay on this loop.
              // I put this delay because without it, the timer on the display would stop running.



  
  // We are going to check the list of touch events we enter previously to
  // know if any touch event just happened, and excecute the corresponding instructions:

  nexLoop(nex_listen_list);  // Check for any touch event






}  // End of loop


