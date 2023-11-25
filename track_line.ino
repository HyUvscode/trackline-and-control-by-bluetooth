#include <SoftwareSerial.h>
SoftwareSerial hcSerial(10, 11);  // RX11 - TX10
//1-DenDo, 2-DoDen, 3-DoDen, 4-DenDo

// Connect Moto shield L298N to the sensor shield V 5.0
const int enAPin = 6;  // (V5) pin 6 - ENA (L298N)
const int in1Pin = 7;  // (V5) pin 7 - IN1 (L298N)
const int in2Pin = 5;  // (V5) pin 5 - IN2 (L298N)
const int in3Pin = 4;  // (V5) pin 4 - IN3 (L298N)
const int in4Pin = 2;  // (V5) pin 2 - IN4 (L298N)
const int enBPin = 3;  // (V5) pin 3 - ENB (L298N)

const int SensorLeft = 9;    //Left sensor input (A9)
const int SensorMiddle = 8;  //Midd sensor input (A8)
const int SensorRight = 0;   //Right sensor input(A0)
int SL;                      //Status of Left line track sensor
int SM;                      //Status of Midd line track sensor
int SR;                      //Status of Righ line track sensor

bool BluetoothModeOn = true; //Set trang thai dieu khien
bool hideLine = false;

int signal1 = 0;
int signal2 = 0;
int command;          // bien luu trang thai Bluetooth
int speedCar = 255;   // toc do 50 - 255.
int speedTurn = 50;


void setup() {

  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);

  Serial.begin(9600);  //Init Serial port rate set to 9600

  hcSerial.begin(9600);  // software serial Arduino to HC-06 (9600 is default)

  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
}

void goAhead(int speedCarFunc) {
  Serial.println("GoAhead");
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
 
  analogWrite(enAPin, speedCarFunc);
  analogWrite(enBPin, speedCarFunc);
}

void goBack(int speedCarFunc) {
  Serial.println("GoBack");
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW); 

  analogWrite(enAPin, speedCarFunc);
  analogWrite(enBPin, speedCarFunc);
}

void goRight(int speedCarFunc) {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);

  analogWrite(enAPin, speedCarFunc);
  analogWrite(enBPin, speedCarFunc);
}

void goLeft(int speedCarFunc) {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);

  analogWrite(enAPin, speedCarFunc);
  analogWrite(enBPin, speedCarFunc);
}


void goAheadRight(int speedCarFunc, int speedTurnFunc) {  
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);


  analogWrite(enAPin, speedTurnFunc);  //banh phai quay cham hon
  analogWrite(enBPin, speedCarFunc);
}

void goAheadLeft(int speedCarFunc, int speedTurnFunc) {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);


  analogWrite(enAPin, speedCarFunc);
  analogWrite(enBPin, speedTurnFunc);  //banh trai quay cham hon
}

void goBackRight(int speedCarFunc, int speedTurnFunc) {  
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);

  analogWrite(enAPin, speedTurnFunc);  //banh phai quay cham hon
  analogWrite(enBPin, speedCarFunc);
}

void goBackLeft(int speedCarFunc, int speedTurnFunc) {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW); 

  analogWrite(enAPin, speedCarFunc);
  analogWrite(enBPin, speedTurnFunc);  //banh trai quay cham hon
}

void stopRobot() {

  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  analogWrite(enAPin, 0);
  analogWrite(enBPin, 0);
}

void loop() {

  if (BluetoothModeOn && hcSerial.available() > 0) {
    speedCar = 255;
    speedTurn = 40;

    command = hcSerial.read();
    stopRobot();  //Initialize with motors stopped.
    hcSerial.println(command);

    switch (command) {
      case 'F': goAhead(speedCar); break;
      case 'B': goBack(speedCar); break;
      case 'L': goLeft(speedCar); break;
      case 'R': goRight(speedCar); break;
      case 'I': goAheadRight(speedCar,speedTurn); break;
      case 'G': goAheadLeft(speedCar,speedTurn); break;
      case 'J': goBackRight(speedCar,speedTurn); break;
      case 'H': goBackLeft(speedCar,speedTurn); break;
      case 'X': BluetoothModeOn = false;break;
      case 'W': hideLine = true;break;
      case 'w': hideLine = false;break;
    }
  } else if (!BluetoothModeOn) {
    speedCar = 160;
    speedTurn = 70;

    if (hcSerial.available() > 0) {
      char inputRemote = hcSerial.read();
      if(inputRemote == 'x'){
        BluetoothModeOn = true;
      }
      else if(inputRemote == 'w'){
        hideLine = false;
      }
      else if(inputRemote == 'W'){
        hideLine = true;
      }
    } else {
      // // 1 black, 0 other
      SL = digitalRead(SensorLeft);
      SM = digitalRead(SensorMiddle);
      SR = digitalRead(SensorRight);

      if (SM != 0 && SL == 0 && SR == 0) {
        goAhead(speedCar-10);
        delay(20);
      } else if (SM != 0 && SL != 0 && SR == 0) {
        goAheadLeft(speedCar,speedTurn);
        delay(20);
      } else if (SM == 0 && SL != 0 && SR == 0) {
        goLeft(speedCar);
        delay(20);
      } else if (SM != 0 && SL == 0 && SR != 0) {
        goAheadRight(speedCar,speedTurn);
        delay(20);
      } else if (SM == 0 && SL == 0 && SR != 0) { 
        goRight(speedCar);
        delay(20);
      } else if (SM == 0 && SL == 0 && SR == 0) {
        if(hideLine == true){
          goAheadRight(speedCar-20,speedTurn);
        }else{
          stopRobot();
        }
        delay(20);
      }
    }
  }

  if (Serial.available()) {
    delay(10);  // The DELAY!
    hcSerial.write(Serial.read());
  }
}
