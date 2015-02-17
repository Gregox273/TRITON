/* Triton ROV
Created 29/6/2014
Digital potentiometer control program (run by onboard Arduino Mega)
CRC8 checksum from: http://www.leonardomiliani.com/en/2013/un-semplice-crc8-per-arduino/
Authors: Gregory Brooks
*/

#include <SPI.h>

#define rs485 35   
#define rs485Transmit    HIGH
#define rs485Receive     LOW 



//CS pins for each potentiometer (CS low to communicate with that potentiometer)
int pots[5] = {2,3,4,5,6};
int packet[5] = {127,127,127,127,127};//motor speeds, 127 = stop, more = forward, less = backward
int revs[5] = {23,25,27,29,31};
int fors[5] = {22,24,26,28,30};
int lightpin = 32; //turn lights on/off
int vpin = 7;//pin for supplying 5V to potentiomers themselves (not Vdd)




/*int pot1 = 2;
int pot2 = 3;
int pot3 = 4;
int pot4 = 5;
int pot5 = 6;
int vpin = 7; //pin for supplying 5V to potentiomers themselves (not Vdd)
int rev1 = 8;//reversers
int rev2 = 9;
int rev3 = 10;
int rev4 = 11;
int rev5 = 12;
int for1 = 22;//forward direction pins
int for2 = 24;
int for3 = 26;
int for4 = 28;
int for5 = 30;
int lightpin = 32;//pin for turning bools[5] on/off
*/


//wiper settings for each potentiometer (0 to 128)


void setup(){
   pinMode(rs485, OUTPUT);//setup rs485 comms
  digitalWrite(rs485, rs485Receive);
  for (int x = 0; x<=4; x++){
    pinMode(pots[x],OUTPUT); //set all output pins to outputs
    pinMode(revs[x],OUTPUT);
    pinMode(fors[x],OUTPUT);
    digitalWrite(fors[x], HIGH); //start motors in forward position
  }
  pinMode(vpin,OUTPUT);
  pinMode(lightpin, OUTPUT);
  pinMode(13,OUTPUT); //LED
  /*set CS + control pins as outputs
  pinMode(pot1,OUTPUT);
  pinMode(pot2,OUTPUT);
  pinMode(pot3,OUTPUT);
  pinMode(pot4,OUTPUT);
  pinMode(pot5,OUTPUT);
  pinMode(vpin,OUTPUT);
  pinMode(lightpin, OUTPUT);
  pinMode(13,OUTPUT); //LED
  pinMode(rev1,OUTPUT);
  pinMode(rev2,OUTPUT);
  pinMode(rev3,OUTPUT);
  pinMode(rev4,OUTPUT);
  pinMode(rev5,OUTPUT);
  pinMode(for1,OUTPUT);
  pinMode(for2,OUTPUT);
  pinMode(for3,OUTPUT);
  pinMode(for4,OUTPUT);
  pinMode(for5,OUTPUT);
  */
  
  //start motors in forward position
  
  /*digitalWrite(for1,HIGH);
  digitalWrite(for2,HIGH);
  digitalWrite(for3,HIGH);
  digitalWrite(for4,HIGH);
  digitalWrite(for5,HIGH);
  */
  
  
  SPI.begin();
  //stop all motors
  for (int x = 0; x<=4; x++){
   digitalWrite(pots[x], LOW); 
  }
  
  SPI.transfer(0);
  SPI.transfer(0);
  
  for (int x = 0; x<=4; x++){
   digitalWrite(pots[x], HIGH); 
  }
  digitalWrite(vpin,HIGH);//now power the potentiometers
  /*digitalWrite(pot1, LOW);
  digitalWrite(pot2, LOW);
  digitalWrite(pot3, LOW);
  digitalWrite(pot4, LOW);
  digitalWrite(pot5, LOW);
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(pot1, HIGH);
  digitalWrite(pot2, HIGH);
  digitalWrite(pot3, HIGH);
  digitalWrite(pot4, HIGH);
  digitalWrite(pot5, HIGH);
  digitalWrite(vpin,HIGH);//now power the potentiometers
  */
  

  
    //Serial1.begin(9600);
    //DEBUG
    Serial.begin(9600);
    Serial1.begin(9600);
  
}

void loop(){
  delay(50);
  parse(packet);
  for (int x = 0; x<=4; x++){
    update(x, packet[x]);
  
  }
}


void parse(int pckt[]){
  if (Serial1.available() > 9){
    if (Serial1.find("U")){ //skip ahead to start of packet
      if (Serial1.available() >= 5){ //if there is at least one complete packet
        for (int x = 0; x<= 4; x++){
          pckt[x] = Serial1.read();
           
        }
      }
    }
  }
}    

void update(int motor, int spd){    //update pot 'X' to wiper position 'X'
  int vX; //scalar motor speed (0 to 127)
  if (spd < 127){
    vX = 127 - spd;
    digitalWrite(fors[motor],LOW);
    digitalWrite(revs[motor],HIGH);
  }
  else if (spd >= 127){
    vX = spd - 127;
    digitalWrite(revs[motor],LOW);
    digitalWrite(fors[motor],HIGH);
  }
  digitalWrite(pots[motor],LOW);
  SPI.transfer(0);
  SPI.transfer(vX);
  digitalWrite(pots[motor],HIGH);
  //Serial.println(vX);


}

