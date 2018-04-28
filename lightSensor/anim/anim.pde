 import processing.serial.*;
Serial myPort;
String val; 

boolean firstContact = false;

import processing.video.*;
Movie myMovie1, myMovie2;
int MoviePlaying;


void setup()
{
  //fullScreen();
  size(600, 500);
  myMovie1 = new Movie(this, "anxious.mov");
  myMovie2 = new Movie(this, "sad_face.mov"); 
  // Open port
  String portName = Serial.list()[3]; 
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
  //buffer; make sure contact established between Arduino and Processing before start
}


void draw() {
  //put incoming data into String Val
  //'\n' is the end of each complete line of message
  val = myPort.readStringUntil('\n');
  
  //make sure our data isn't empty before continuing
  if (val != null) {
    //trim whitespace and formatting characters (like carriage return)
    val = trim(val);
    println(val);

    //look for an 'A' Arduino gave us
    //if it's there, start our conversation
    //clear the buffer 
    //first contact is made
    //send a "C" request for data
    if (firstContact == false) {
      if (val.equals("A")) {
        myPort.clear();
        firstContact = true;
        myPort.write("C");
        println("Contact Made");
      }
    } else { //if we've already established contact, let's get into REAL business!
     //when button is pressed, received message "Y" from Arduino, do following:
      if (val.equals("Y")) {
        myMovie2.stop();
        background(0);
        image(myMovie1, 0, 0, width, height);   
        myMovie1.loop();
      } else if (val.equals("P")) {
        //when button is not pressed, received message "P" from Arduino, do following:
        myMovie1.stop();
        background(0);
        image(myMovie2, 0, 0, width, height);   
        myMovie2.loop();
      }
    }
  }
}

//always send an M to continute the conversation
void serialEvent(Serial myPort) {
  myPort.write("M");
}


void movieEvent(Movie m) {
  m.read();
}
