void setup(){
  Serial.begin(9600);
}


void loop(){
  for (int x = 0; x<129; x++){
    Serial.print('U');
    for (int y = 0; y<=4; y++){
      Serial.print(char(x)); //prints the ascii character corresponding to the integer x, this is decoded back into an integer by the mega
    }
    delay (1000);
  }
}
