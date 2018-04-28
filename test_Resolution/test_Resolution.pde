import processing.video.*;
Movie myMovie1, myMovie2;
int MoviePlaying;

void setup()
{
  fullScreen();
  //size(600, 350);
  myMovie1 = new Movie(this, "chaos.mov");
  myMovie2 = new Movie(this, "sad_face.mov");
}

void draw() {
  image(myMovie1, 0, 0, width, height);   
  myMovie1.loop();
}

void movieEvent(Movie m) {
  m.read();
}
