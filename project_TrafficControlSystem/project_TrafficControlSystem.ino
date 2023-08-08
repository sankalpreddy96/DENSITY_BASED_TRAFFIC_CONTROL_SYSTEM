// Pin Numbers: Ultrasonic Sensor
#define US1_Trig_Pin 11   // (OUTPUT) Connect to Trig pin
#define US1_Echo_Pin 10   // (INPUT)  Connect to Echo pin

#define US2_Trig_Pin 7   // (OUTPUT) Connect to Trig pin
#define US2_Echo_Pin 6   // (INPUT)  Connect to Echo pin

#define US3_Trig_Pin 5   // (OUTPUT) Connect to Trig pin
#define US3_Echo_Pin 4   // (INPUT)  Connect to Echo pin

#define US4_Trig_Pin 3   // (OUTPUT) Connect to Trig pin
#define US4_Echo_Pin 2   // (INPUT)  Connect to Echo pin

// Variables:
int distance1;
int distance2;
int distance3;
int distance4;

int SignalLED_Pins[4][3] = {{23, 25, 27}, {29, 31, 33}, {35, 37, 39}, {41, 43, 45}};
int dist[4];
int order[4];
int prvSignalIdx = -1;
int curSignalIdx;
long prvSignalTime;
int signalIncr;


void setup() {
  // Define Input & Output Pins:
  pinMode(US1_Trig_Pin, OUTPUT);
  pinMode(US1_Echo_Pin, INPUT);

  pinMode(US2_Trig_Pin, OUTPUT);
  pinMode(US2_Echo_Pin, INPUT);

  pinMode(US3_Trig_Pin, OUTPUT);
  pinMode(US3_Echo_Pin, INPUT);

  pinMode(US4_Trig_Pin, OUTPUT);
  pinMode(US4_Echo_Pin, INPUT);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(SignalLED_Pins[i][j], OUTPUT);
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      if (j == 0) {
        digitalWrite(SignalLED_Pins[i][j], HIGH);
      } else {
        digitalWrite(SignalLED_Pins[i][j], LOW);
      }
    }
  }

  // Initialize serial communications with the PC:
  Serial.begin(9600);

}

void loop() {
  ReadDistance();
  curSignalIdx = order[signalIncr];

  Serial.print("signalIncr: ");
  Serial.println(signalIncr);

  Serial.print("curSignalIdx: ");
  Serial.println(curSignalIdx);

  Serial.print("Min Dist: ");
  Serial.println(dist[curSignalIdx]);
  
  if (curSignalIdx != prvSignalIdx) {
    Serial.println("Signal Change");
    digitalWrite(SignalLED_Pins[prvSignalIdx][2], LOW);   // Green Off
    digitalWrite(SignalLED_Pins[prvSignalIdx][0], HIGH);  // Red On

    digitalWrite(SignalLED_Pins[curSignalIdx][0], LOW);   // Red Off
    digitalWrite(SignalLED_Pins[curSignalIdx][1], HIGH);  // Yellow On
    delay(3000);
    digitalWrite(SignalLED_Pins[curSignalIdx][1], LOW);   // Yellow Off
    digitalWrite(SignalLED_Pins[curSignalIdx][2], HIGH);  // Green On
    prvSignalIdx = curSignalIdx;
    prvSignalTime = millis();
  }

  if (curSignalIdx == prvSignalIdx && millis() - prvSignalTime > 20000) {
    Serial.println("Timeout");
    signalIncr++;
    if(signalIncr == 4){
      signalIncr = 0;
    }
  }
  delay(2000);
}


void ReadDistance() {
  int prvMinValue;
  int curMinValue;
  int curMinIdx;
  int i, j;

  Serial.println();
  dist[0] = GetUltrasonicDist(US1_Trig_Pin, US1_Echo_Pin);
  Serial.print("Distance 0: ");
  Serial.println(dist[0]);
  delay(100);

  dist[1] = GetUltrasonicDist(US2_Trig_Pin, US2_Echo_Pin);
  Serial.print("Distance 1: ");
  Serial.println(dist[1]);
  delay(100);

  dist[2] = GetUltrasonicDist(US3_Trig_Pin, US3_Echo_Pin);
  Serial.print("Distance 2: ");
  Serial.println(dist[2]);
  delay(100);

  dist[3] = GetUltrasonicDist(US4_Trig_Pin, US4_Echo_Pin);
  Serial.print("Distance 3: ");
  Serial.println(dist[3]);
  
  curMinIdx = -1;
  prvMinValue = -1;
  for (i = 0; i < 4; i++) {
    curMinValue = 9999;
    for (j = 0; j < 4; j++) {
      if (dist[j] < curMinValue && dist[j] > prvMinValue) {
        curMinValue = dist[j];
        curMinIdx = j;
      }
    }
    prvMinValue = curMinValue;
    order[i] = curMinIdx;
    //Serial.println(curMinValue);
  }
}




int GetUltrasonicDist(int trigPin, int echoPin) {
  int distance;
  /* Clear the trig pin by setting it LOW */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);

  /* Trigger the sensor by setting the trig pin HIGH for 10 microseconds */
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  /* Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds */
  long duration = pulseIn(echoPin, HIGH);

  /* Calculate the distance: Distance = Speed * Time
     Speed of sound is 343 metres per second = 0.0343cm per micro second */
  distance = 0.0343 * (duration / 2);

  return distance;
}
