#include <Wire.h>

#define SLAVE_ADDRESS 0x04
#define FLOATS_SENT 7

float data[FLOATS_SENT];
int status;
int state[4];
void setup() {
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
    
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  

  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

void loop() {
  delay(1000);
}

/* callback for received data */
void receiveData(int byteCount) {
  while (Wire.available()) {
    status = Wire.read();
    if (status > 0 ) {
      Serial.print("data received: ");
      Serial.println(status);
      if (status < 5 ) {
        if (state[status] == 0) {
          digitalWrite(status, HIGH); // set the LED on
          state[status] = 1;
        } else {
          digitalWrite(status, LOW); // set the LED off
          state[status] = 0;
        }
        Wire.write(state[status-200]);
      }
      if (status > 200 ){
         Serial.print("pin value: ");
         Serial.println(state[status-200]);
         Wire.write(state[status-200]);
         data[6] = state[status-200];
      }
      if (status < 14 && status > 5 ) {
         data[6] = digitalRead(status);
      }
    }
  }
}

// callback for sending data
void sendData() {
  data[0] = analogRead(A0);
  data[1] = analogRead(A1);
  data[2] = analogRead(A2);
  data[3] = analogRead(A3);
  data[4] = analogRead(A6);
  data[5] = analogRead(A7);
  Wire.write((byte*) &data, FLOATS_SENT*sizeof(float));
}

