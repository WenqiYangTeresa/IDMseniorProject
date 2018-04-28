//char val; //data received from the serial port

#define trigPin 10
#define echoPin 13
bool CHAOS = false;
int threshold = 40;
int oldDistance = 300;
int newDistance;


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  establishContact();
}

void loop() {

  // if (Serial.available() > 0) { //if there is data available to read
  //   val = Serial.read(); //read and store it at val

  //read sensor input
  float duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  //constraint
  if (distance >= 200 || distance <= 0) {
    //if out of range, don't use the jumpy reading and use the last normal reading
    //Serial.println("Out of range");
    newDistance = oldDistance;
  } else {
    //if in range, assign the input reading to newDistance
    newDistance = distance;
    //Serial.println(newDistance);
  }
//  Serial.print("old Distance is ");
//  Serial.println(oldDistance);
//  Serial.print("new Distance is ");
//  Serial.println(newDistance);
  
  if (newDistance < threshold) {
    if (oldDistance >= threshold) {
      CHAOS = true;
    }
  } else if (newDistance > threshold) {
    if (oldDistance <= threshold) {
      CHAOS = false;
    }
  }

  if (CHAOS == true) {
    Serial.println("Y"); //play yellow video; chaotic Mov2
  } else {
    Serial.println("P"); //play pink video; calm Mov1
  }

  oldDistance = newDistance;
  delay(200);

}


// send an A to to initiate first contact
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("A");
    delay(200);
  }
}




