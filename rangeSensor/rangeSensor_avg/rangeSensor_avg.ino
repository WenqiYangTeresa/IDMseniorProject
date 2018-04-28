//char val; //data received from the serial port

//#define trigPin 10
//#define echoPin 13
int trigPin = 10;
int echoPin = 13;
bool CHAOS = false;
int threshold = 60; //1m away
int oldDistance = 200;
int newDistance;

// an array to hold the values taken in
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 2000;                  // the running total
//int average = 0;                // the average
int newAverage;
int oldAverage = 200;


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 200;
  }
  establishContact();
}

void loop() {
  
  //READ SENSOR
  float duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  
  //FILTER
  if (distance > 200 || distance <= 0) {
    //if out of range, don't use the jumpy reading and use the last normal reading
    //Serial.println("Out of range");
    newDistance = oldDistance;
  } else {
    //if in range, assign the input reading to newDistance
    newDistance = distance;
    //Serial.println(newDistance);
  }
  
  //AVERAGE
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = newDistance;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  newAverage = total / numReadings;
  // send it to the computer as ASCII digits
//  Serial.print("old Avg is ");
//  Serial.println(oldAverage);
//  Serial.print("new Avg is ");
//  Serial.println(newAverage);
//  Serial.print("old Distance is ");
//  Serial.println(oldDistance);
//  Serial.print("new Distance is ");
//  Serial.println(newDistance);
  delay(1);        // delay in between reads for stability

  //use the Average as the guideline
  if (newAverage < threshold) {
    if (oldAverage >= threshold) {
      CHAOS = true;
    }
  } else if (newAverage > threshold) {
    if (oldAverage <= threshold) {
      CHAOS = false;
    }
  }
  //ACTION
  if (CHAOS == true) {
    Serial.println("Y"); //play yellow video; chaotic Mov2
  } else {
    Serial.println("P"); //play pink video; calm Mov1
  }

  //UPDATE
  oldDistance = newDistance;
  oldAverage = newAverage;
  delay(200);

}


// send an A to to initiate first contact
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("A");
    delay(200);
  }
}





