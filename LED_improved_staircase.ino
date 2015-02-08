/*
Osam Javed


*/


// tell arduino where pins are connected
int dataPin = 2;
int latchPin = 4;
int clockPin = 3;
int buttonPin = 8;
int val = 0;
int seq[] = {0,0,0,0,0,0,0,0,1,3,7,15,31};
int seq2[] = {1, 3, 7, 15, 31, 63, 127, 255, 255, 255, 255, 255, 255};
int seqD[] = {31,31, 31, 31, 31,31,31,31,31, 30, 28, 24, 16, 0, 0, 0,0,0,0,0,0};
int seq2D[] = {255, 254, 252, 248, 240, 224, 192,128,0,0 ,0,0,0, 0};
int bseq[] = {16, 24, 28, 30, 31, 31, 31, 31, 31, 31, 31, 31 ,31};      // backwards sequence comparable to seq
int bseq2[] = {0,0,0,0,0,128, 192, 224, 240, 248, 252, 254, 255 };
int bseqD[] = {31,15,7,3,1,0,0,0,0,0,0,0,0,0,0,0};
int bseq2D[] = {255,255,255,255,255,255, 127,63,31,15,7,3,1,0};
int value = 0;
int value2 = 0;
int bvalue = 0;
int bvalue2 = 0;
int all_value2 = 255;
int all_value1 = 31;

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;          // CHANGE BACK TO 30 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!        
int calibrationTime2 = 10;        // This is to calibrate the second sensor that we have     
//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 5;    //the digital pin connected to the PIR sensor's output
// int ledPin = 13;   // DELETE LATER
int pirPin2 = 6;   // our second PIR sensor's output pin


void setup() 
{
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);  
  pinMode(pirPin, INPUT);
  pinMode(pirPin2, INPUT);    // our second sensor
  
  // button sensor
  pinMode(buttonPin,INPUT);
  
  // Let pirPin represent the IR sensor that will be at the bottom
  // of the stairs while pirPin2 represent the second IR sensor that 
  // is at the top of the stairs. 
  digitalWrite(pirPin, LOW);
  digitalWrite(pirPin2, LOW);  // second sensor
  
  
  
  
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
    
   // Calibrate the second sensor as well
    Serial.print("calibrating second sensor ");
    for(int i = 0; i < calibrationTime2; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
    
    
}


void loop()
{
  
      

  
  
  
      // Let's take of the scenario where someone walks from downstairs to upstairs
      if(digitalRead(pirPin) == HIGH) {    // if the first sensor downstairs is activated
        
        downUp();
      }
      
      if(digitalRead(pirPin2) == HIGH) {  // If someones walks from downstairs to upstairs and crosses the second sensor
      
        digitalWrite(latchPin, LOW);     // Turn all the LED's off
      
      }
      
      
     // If someone crossed downstairs sensor but did not cross the upstairs sensor....
     //  then start turning off the lights regardless
    if(digitalRead(pirPin) == LOW) {
      digitalWrite(latchPin, LOW);
    } 
    
    
    
    
    // Now let's cover the scenario where someone might walk from upstairs to downstairs
    if(digitalRead(pirPin2) == HIGH)  {
      upDown();
    }
    
    
    
    // if someone crosses the downstairs sensor, turn lights off
    if(digitalRead(pirPin) == HIGH) {
      digitalWrite(latchPin, LOW);
    }
    
    
    
    // if someone crosses the upstairs sensor but NOT the downstairs sensor, turn off the lights anyways...
    if(digitalRead(pirPin2) == LOW) {
      digitalWrite(latchPin, LOW);
    }
    
    else 
    {
      firstLast_LED_on();
    }

}  // ends loop function


void writeOutput()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  shiftOut(dataPin, clockPin, MSBFIRST, value2);
  digitalWrite(latchPin, HIGH);
}


void bwriteOutput()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, bvalue);
  shiftOut(dataPin, clockPin, MSBFIRST, bvalue2);
  digitalWrite(latchPin, HIGH);
}

void downUp()
{
  // downstairs to upstairs sequence turn on
  for (int n = 0; n < 13; n++)
  {
    value = seq[n];
    value2 = seq2[n];
    writeOutput();
    delay(750);
  }
  
  delay(2000);
  
   // backwards fade for downUP
  for (int s=0; s < 13; s++)
  {
    value = seqD[s];
    value2 = seq2D[s];
    writeOutput();
    delay(750);
  }
  delay(1000);
}


void upDown()
{
  // upstairs to downstairs sequence
  for (int k = 0; k < 13; k++)
  {
    bvalue = bseq[k];
    bvalue2 = bseq2[k];
    bwriteOutput();
    delay(750); 
  }
  
   // backwards fade for downUP
  for (int s=0; s < 13; s++)
  {
    value = bseqD[s];
    value2 = bseq2D[s];
    writeOutput();
    delay(750);
  }
}

void firstLast_LED_on()
{
  value = 16;
  value2 = 1;
  writeOutput();
  delay(750);
}























































