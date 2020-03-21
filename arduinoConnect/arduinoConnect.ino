/** @author: Raymond Caramto III **/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9); // RX, TX  

/**  HM10 Pins       Arduino Uno
     Pin 1/RXD          Pin 0
     Pin 2/TXD          Pin 1
**/     

/** HM-10 Commands */
/** Command     Output
/**   AT          OK
 *  AT+OK         OK
 *  
 */

char c;
const int FSR_PIN1 = A0; // Pin connected to FSR/resistor divider
const int FSR_PIN2 = A1;
const int FSR_PIN3 = A2;
const int LED_PIN1 = 13;
const int LED_PIN2 = 12;
const double VCC = 4.99; // Measured voltage of Ardunio 5V line
const double R_DIV = 5020.0; // Measured resistance of 5k resistor
float fsrADC1 = 0, fsrADC2 = 0, fsrADC3= 0;
String state;
double fsrV1, fsrV2, fsrV3;
double fsrR1, fsrR2, fsrR3;
double fsrG1, fsrG2, fsrG3;
double force1, force2, force3;
double tol = 5.0;

void setup() {
   Serial.begin(9600);
  // If the baudrate of the HM-10 module has been updated,
  // you may need to change 9600 by another value
  // Once you have found the correct baudrate,
  // you can update it using AT+BAUDx command 
  // e.g. AT+BAUD0 for 9600 bauds 
  mySerial.begin(9600); //Starts bits per second from the HM-10 to Arduino (baudrate)
  pinMode(FSR_PIN1, INPUT); 
  pinMode(FSR_PIN2, INPUT); 
  pinMode(FSR_PIN3, INPUT); 
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN1, OUTPUT); 
  Serial.println("AT Enabled");
}

void loop() {
  initHM10();
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.readString(); // Reads the data from the serial port
 }
 if (state.equals("9")){
  checkPressure(); //Start detecting the baby
 }
 else if(state.equals("8")){
  fsrADC1 = 0;
  fsrADC2 = 0;
  fsrADC3 = 0;
 }
}

void initHM10(){ //TODO: Update this method from the loop() stuff up top^
  if (Serial.available()) {
    c = Serial.read();
    if(c != 0){
      mySerial.print(c);
      digitalWrite(LED_PIN2, HIGH);
    }
    mySerial.print(c);
  }
  else if(!Serial.available()){
    digitalWrite(LED_PIN2, LOW);
  }
  if (mySerial.available()) {
    c = mySerial.read(); //This picked up on the analog readings! NOOOOOOO
    if(c != 0){
      Serial.print(c);
      digitalWrite(LED_PIN2, HIGH);
    }
    Serial.print("d" + c);
  }
  else if(!mySerial.available()){
    digitalWrite(LED_PIN2, LOW);
  }
}

void checkPressure(){
  fsrADC1 = analogRead(FSR_PIN1);
  fsrADC2 = analogRead(FSR_PIN2);
  fsrADC3 = analogRead(FSR_PIN3);
  if (fsrADC1 > 20 and fsrADC2 > 40 and fsrADC3 > 30) // If the analog reading is non-zero
  {
    digitalWrite(LED_PIN1,HIGH);
    fsrV1 = fsrADC1 * VCC / 1023.0; //Calculates voltage from analog signal
    fsrV2 = fsrADC2 * VCC / 1023.0; //Calculates voltage from analog signal
    fsrV3 = fsrADC3 * VCC / 1023.0; //Calculates voltage from analog signal

    fsrR1 = R_DIV * (VCC / fsrV1 - 1.0); //Calculates resistance from voltage signal
    fsrR2 = R_DIV * (VCC / fsrV2 - 1.0); //Calculates resistance from voltage signal
    fsrR3 = R_DIV * (VCC / fsrV3 - 1.0); //Calculates resistance from voltage signal

    fsrG1 = 1.0 / fsrR1; // Calculate conductance from resistor
    fsrG2 = 1.0 / fsrR2; // Calculate conductance from resistor
    fsrG3 = 1.0 / fsrR3; // Calculate conductance from resistor

    if (fsrR1 <= 1000 and fsrR2 <= 1000 and fsrR3 <= 1000){ 
      force1 = (fsrG1 - 0.00075) / 0.00000032639;
      force2 = (fsrG2 - 0.00075) / 0.00000032639;
      force3 = (fsrG3 - 0.00075) / 0.00000032639;
        if(force1 > 2 && force2 > 4 && force3 > 3){ //Measures in kg
            Serial.println("Occupied"); 
      }
        else{
            Serial.println("Not Occupied");
      }
    }
    else{
      //Getting force readings in kg
      force1 =  (fsrG1 / 0.000000642857)/1000; 
      force2 =  (fsrG2 / 0.000000642857)/1000; 
      force3 =  (fsrG3 / 0.000000642857)/1000;
      if(force1 > 2 && force2 > 4 && force3 > 3){ //Measures in kg
        Serial.println("Occupied"); 
      }
      else{
        Serial.println("Not Occupied");
      }
    }
//      Serial.println("Force1: " + String(force1) + "g");
//      Serial.println("Force2: " + String(force2) + "g");
//      Serial.println("Force3: " + String(force3) + "g\n");
  }
  else{
    digitalWrite(LED_PIN1, LOW);
  }
}
