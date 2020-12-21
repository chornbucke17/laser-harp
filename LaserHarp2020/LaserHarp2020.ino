/* Laser harp  
 * 
 * Author: Christopher Hornbuckle
 * Date: Sept.-Dec. 2020
 * 
 * Description: code for a self-constructed laser harp built using an arduino mega 2560. Photocells are used to
 * read breaks in light between a laser diode and a LDR (photocell). Output from arduino is printed using serial print
 * to create strings containing byte information which can be interpreted by MAX 8. The format for the print statement 
 * is as follows:
 *          instrument status dial dial dial dial
 *          3           0     344  453  1023 17
 * 
 * Arduino Mega 2560
 * 
 */
const int laserPins [] = {13,12,11,10,9,8,7,6,5,4,3,2}; // 10 pins for current implementation

int photocell_readings [12]; // 12 photocells for current implementation

int potentiometers [4] = {12,13,14,15}; // analog ports A12-A15. Supports 4 potentiometer dials.
int potentiometer_readings [4];//

/*---set-up----*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // changed from 9600 
  for (int i = 0; i < 12; i++) { // turns on each of the 12 pins individually
    pinMode(laserPins[i], OUTPUT);
  }
  /*--turn-lasers-on--*/
  activate_lasers();
} // end setup

/*---main-loop---*/
void loop() {
  // put your main code here, to run repeatedly:
  handle_photocell_readings(photocell_readings);
  handle_potentiometer_readings(potentiometer_readings, potentiometers);
  play_string(photocell_readings, potentiometer_readings);
  
} // end loop


/*---program-functions*/
// loops through and stores analog readings from photocells starting from A0
void handle_photocell_readings(int readings[]) {
  int pin = 12; 
  for (int i = 0; i < pin; i++) {
    photocell_readings[i] = analogRead(i); // pins on the Arduino are read from A0-A11
  }
}

// loops through four potentiometers and stores their values in a new array
void handle_potentiometer_readings(int dials[], int potentiometers[]) {
  for (int i = 0; i < 4; i++) { 
     potentiometer_readings[i] = analogRead(potentiometers[i]);
  }
}

// activates harp lasers in sequence
void activate_lasers() {
  for (int i = 0; i < 12; i++) { // turn on lasers in sequence
    delay(200);
    digitalWrite(laserPins[i], HIGH);
  }
}

// checks each stored value in the array of photocell readings. Light target value of 550 allows LDRs and laser diodes
// to work in both bright and dim room conditions. 'printLine' is printed to serial port during each loop once. 
void play_string(int readings[], int dials[]) {
  for (int i = 0; i < 12; i++) {
    
    if (readings[i] > 550) { // target LDR reading is 550
      
      String printLine = String(i + 1); // MAX unpacks strings from 1-12
      printLine += " 0 "; // status is off
      for (int j = 0; j < 4; j++) {
        printLine += dials[j];
        printLine += " "; // serial print formatting
      }
      Serial.println(printLine);  
      //delay(1); // added for stability
      
      } else {
      String printLine = String(i + 1);
      printLine += " 1 "; // status is on
      for (int j = 0; j < 4; j++) {
        printLine += dials[j];
        printLine += " ";
      }
      Serial.println(printLine); 
      //delay(1); // added for stability
      }
  }
}
