//Count Time & Calibration
unsigned long start_time;
unsigned long current_time;
unsigned long elapsed_time;
unsigned long start_calibrate;
unsigned long current_calibrate;
unsigned long elapsed_calibrate;

//Input & Output
const int sensorPin = A0;
const int sensorPinB = A1;
const int ledPin = 9;
int sensorValue = 0;
int sensorMin = 1023;
int sensorMax = 0;
int sensorValueB = 0;
int sensorMinB = 1023;
int sensorMaxB = 0;

//Average
int New_regAverage = 255;
int Old_regAverage;
int list[20];
int numReadings = 20;
//int sensorReading = 0;
int total = 0;
int regAverage = 0;

int New_regAverageB = 255;
int Old_regAverageB;
int listB[20];
int totalB = 0;
int regAverageB = 0;

//Trigger
bool CHAOS = false;
int threshold = 25;


void setup() {
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 255);
  while (millis() < 5000) {
    sensorValue = analogRead(sensorPin);
    sensorValueB = analogRead(sensorPinB);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }

    // record the maximum sensor value
    if (sensorValueB > sensorMaxB) {
      sensorMaxB = sensorValueB;
    }

    // record the minimum sensor value
    if (sensorValueB < sensorMinB) {
      sensorMinB = sensorValueB;
    }
  }
  analogWrite(ledPin, 0);
  // put your setup code here, to run once:
  Serial.begin(9600);
//  Serial.print("sensorMin:");
//  Serial.println(sensorMin);
//  Serial.print("sensorMax:");
//  Serial.println(sensorMax);
//  Serial.print("sensorMinB:");
//  Serial.println(sensorMinB);
//  Serial.print("sensorMaxB:");
//  Serial.println(sensorMaxB);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    list[thisReading] = 0;
    listB[thisReading] = 0;
  }
  establishContact();
}

void loop() {

  //Count Time
  start_time = millis();
  //Serial.println(start_time);

  //Main Action
  do {
    current_time = millis();
    elapsed_time = current_time - start_time;

    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      list[thisReading] = analogRead(sensorPin);
      list[thisReading] = map(list[thisReading], sensorMin, sensorMax, 0, 255);
      list[thisReading] = constrain(list[thisReading], 0, 255);
      listB[thisReading] = analogRead(sensorPinB);
      listB[thisReading] = map(listB[thisReading], sensorMinB, sensorMaxB, 0, 255);
      listB[thisReading] = constrain(listB[thisReading], 0, 255);
      // Update Total
      // add the reading to the total:
      total = total + list[thisReading];
      totalB = totalB + listB[thisReading];
      delay(5);
    }
//    Serial.print("ArrayA: [");
//    for (byte j = 0; j < numReadings; j++) {
//      Serial.print(list[j], DEC);
//      Serial.print (", ");
//    }
//    Serial.print("]\r\n");
//    Serial.print("ArrayB: [");
//    for (byte j = 0; j < numReadings; j++) {
//      Serial.print(listB[j], DEC);
//      Serial.print (", ");
//    }
//    Serial.print("]\r\n");
    //average of all values
    regAverage = total / numReadings;
    New_regAverage = regAverage;
    regAverageB = totalB / numReadings;
    New_regAverageB = regAverageB;
//    Serial.print("New_regAverage:  ");
//    Serial.println(New_regAverage);
//    Serial.print("New_regAverageB:  ");
//    Serial.println(New_regAverageB);


    if (( New_regAverage > threshold && Old_regAverage <= threshold) || (New_regAverageB > threshold && Old_regAverageB <= threshold)) {
      analogWrite(ledPin, 255);
      CHAOS = false;
    } else if (( New_regAverage < threshold && Old_regAverage >= threshold) || (New_regAverageB < threshold && Old_regAverageB >= threshold)) {
      analogWrite(ledPin, 0);
      CHAOS = true;
    }
    if (CHAOS == true) {
      Serial.println("Y"); //play yellow video; chaotic Mov2
    } else {
      Serial.println("P"); //play pink video; calm Mov1
    }
    total = 0;
    Old_regAverage = New_regAverage;
    totalB = 0;
    Old_regAverageB = New_regAverageB;
    delay(5);
  }
  while (elapsed_time < 1200000);



  //Recalibrate every 20min
  if (elapsed_time = 1200000) {
    sensorMin = 1023;
    sensorMax = 0;
    sensorMinB = 1023;
    sensorMaxB = 0;
    // Serial.println("recalibrate");
    start_calibrate = millis();
    do {

      current_calibrate = millis();
      elapsed_calibrate = current_calibrate - start_calibrate;
      sensorValue = analogRead(sensorPin);

      // record the maximum sensor value
      if (sensorValue > sensorMax) {
        sensorMax = sensorValue;
      }

      // record the minimum sensor value
      if (sensorValue < sensorMin) {
        sensorMin = sensorValue;
      }

      sensorValueB = analogRead(sensorPinB);

      // record the maximum sensor value
      if (sensorValueB > sensorMaxB) {
        sensorMaxB = sensorValueB;
      }

      // record the minimum sensor value
      if (sensorValueB < sensorMinB) {
        sensorMinB = sensorValueB;
      }
    }
    while (elapsed_calibrate < 5000);
    Serial.print("sensorMin:");
    Serial.println(sensorMin);
    Serial.print("sensorMax:");
    Serial.println(sensorMax);
    Serial.print("sensorMinB:");
    Serial.println(sensorMinB);
    Serial.print("sensorMaxB:");
    Serial.println(sensorMaxB);
  }



}

// send an A to to initiate first contact
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("A");
    delay(200);
  }
}

