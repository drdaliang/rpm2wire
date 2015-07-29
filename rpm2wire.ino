#include <Wire.h>

int tachoPin1 = 4;
int tachoPin2 = 5;
int tachoPin3 = 6;
int tachoPin4 = 7;

// The i2c pins for arduino pro micro is Pin2 and Pin 3.
const int recordBufferSize = 6;
int tacho1[recordBufferSize]={0,0,0,0,0,0};
int tacho2[recordBufferSize]={0,0,0,0,0,0};
int tacho3[recordBufferSize]={0,0,0,0,0,0} ;
int tacho4[recordBufferSize]={0,0,0,0,0,0};

unsigned long tachoBuffer1 = 0;
unsigned long tachoBuffer2 = 0;
unsigned long tachoBuffer3 = 0;
unsigned long tachoBuffer4 = 0;
unsigned long current = 0;

boolean tachoFlag1 = LOW;
boolean tachoFlag2 = LOW;
boolean tachoFlag3 = LOW;
boolean tachoFlag4 = LOW;


byte buf [8];


void setup() {
  // put your setup code here, to run once:
  pinMode(tachoPin1, INPUT_PULLUP);
  pinMode(tachoPin2, INPUT_PULLUP);
  pinMode(tachoPin3, INPUT_PULLUP);
  pinMode(tachoPin4, INPUT_PULLUP);

  tachoBuffer1 = micros();
  tachoBuffer2 = micros();
  tachoBuffer3 = micros();
  tachoBuffer4 = micros();
  //initialize wire protocol
  //Address #2
  Wire.begin(2);                // join i2c bus with address #2
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  // register current loop time
  
  //for blower
  current = micros();
  if (tachoFlag1 == LOW && digitalRead(tachoPin1) == HIGH) {
    unsigned long temp1 = tachoBuffer1;
    tachoFlag1 = HIGH;
    tachoBuffer1 = current;
    if (tachoBuffer1 - temp1 > 1500 ) {
        fifo(tacho1,int(15000000 / (tachoBuffer1 - temp1)));//60/(microseconds*4/1000000)
    }
  }

 if (tachoFlag1 == HIGH && digitalRead(tachoPin1) == LOW) {
    unsigned long temp1 = tachoBuffer1;
    tachoFlag1 = LOW;
    tachoBuffer1 = current;
    if (tachoBuffer1 - temp1 > 1500 ) {
      fifo(tacho1,int(15000000 / (tachoBuffer1 - temp1))); //60/(microseconds*4/1000000)
    }
  }

//for motor hall 1
    current = micros();
  if (tachoFlag2 == LOW && digitalRead(tachoPin2) == HIGH) {
    unsigned long temp2 = tachoBuffer2;
    tachoFlag2 = HIGH;
    tachoBuffer2 = current;
    if (tachoBuffer2 - temp2 > 1500 ) {
        fifo(tacho2,int(7500000 / (tachoBuffer2 - temp2)));//60/(microseconds*4/1000000)
    }
  }

 if (tachoFlag2 == HIGH && digitalRead(tachoPin2) == LOW) {
    unsigned long temp2 = tachoBuffer2;
    tachoFlag2 = LOW;
    tachoBuffer2 = current;
    if (tachoBuffer2 - temp2 > 1500 ) {
      fifo(tacho2,int(7500000 / (tachoBuffer2 - temp2))); //60/(microseconds*4/1000000)
    }
  }
  
  //for motor hall 2
    current = micros();
  if (tachoFlag3 == LOW && digitalRead(tachoPin3) == HIGH) {
    unsigned long temp3 = tachoBuffer3;
    tachoFlag3 = HIGH;
    tachoBuffer3 = current;
    if (tachoBuffer3 - temp3 > 1500 ) {
        fifo(tacho3,int(7500000 / (tachoBuffer3 - temp3)));//60/(microseconds*4/1000000)
    }
  }

 if (tachoFlag3 == HIGH && digitalRead(tachoPin3) == LOW) {
    unsigned long temp3 = tachoBuffer3;
    tachoFlag3 = LOW;
    tachoBuffer3 = current;
    if (tachoBuffer3 - temp3 > 1500 ) {
      fifo(tacho3,int(7500000 / (tachoBuffer3 - temp3))); //60/(microseconds*4/1000000)
    }
  }
  //for motor hall 3
    current = micros();
  if (tachoFlag4 == LOW && digitalRead(tachoPin4) == HIGH) {
    unsigned long temp4 = tachoBuffer4;
    tachoFlag4 = HIGH;
    tachoBuffer4 = current;
    if (tachoBuffer4 - temp4 > 1500 ) {
        fifo(tacho4,int(7500000 / (tachoBuffer4 - temp4)));//60/(microseconds*4/1000000)
    }
  }

 if (tachoFlag4 == HIGH && digitalRead(tachoPin4) == LOW) {
    unsigned long temp4 = tachoBuffer4;
    tachoFlag4 = LOW;
    tachoBuffer4 = current;
    if (tachoBuffer4 - temp4 > 1500 ) {
      fifo(tacho4,int(7500000 / (tachoBuffer4 - temp4))); //60/(microseconds*4/1000000)
    }
  }
  
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
//  buf [0] = tacho1 >> 24;
//  buf [1] = (tacho1 >> 16) & 0xFF;
//  buf [2] = (tacho1 >> 8) & 0xFF;
//  buf [3] = tacho1 & 0xFF;
  tacho1[0]=arrayavg(tacho1);
  tacho2[0]=arrayavg(tacho2);
  tacho3[0]=arrayavg(tacho3);
  tacho4[0]=arrayavg(tacho4);
  buf [0] = tacho1[0] >> 8;
  buf [1] = (tacho1[0]) & 0xFF;
  buf [2] = tacho2[0] >> 8;
  buf [3] = (tacho2[0]) & 0xFF;
  buf [4] = tacho3[0] >> 8;
  buf [5] = (tacho3[0]) & 0xFF;
  buf [6] = tacho4[0] >> 8;
  buf [7] = (tacho4[0]) & 0xFF; 
  arrayclear(tacho1);
  arrayclear(tacho2);
  arrayclear(tacho3);
  arrayclear(tacho4);
  // Serial.println(buf[1]);
  Wire.write(buf, 8); // respond with message of 6 bytes
  //Wire.write(tacho1>>8);
  // as expected by master
}

void fifo(int* arr, int newvalue){
  for (int i=1;i<recordBufferSize;i++){
    arr[i-1] = arr[i];
    }
    arr[recordBufferSize-1] = newvalue;
  }

int arrayavg(int* arr){
  long sum=0;
  for (int i=0;i<recordBufferSize;i++){
    sum += arr[i];
    }
    return int(sum/recordBufferSize);
  }
  
  int arrayclear(int* arr){
  for (int i=0;i<recordBufferSize;i++){
    arr[i]=0;
    }
  }

