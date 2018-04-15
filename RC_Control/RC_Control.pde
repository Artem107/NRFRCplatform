import processing.serial.*;
Serial myPort;
String v = "0";
byte buf[] = new byte[6];
byte keyB[] = new byte[4];

void sendMsg() {
  for (;; ) {
    if (myPort.available() > 0) {
      v = myPort.readString();
    }
    buf[2]=byte(constrain(
      (keyB[0]==1?127:0)+
      (keyB[1]==1?-127:0)+
      (keyB[2]==1?-127:0)+
      (keyB[3]==1?127:0), -127, 127));

    buf[3]=byte(constrain(
      (keyB[0]==1?127:0)+
      (keyB[1]==1?127:0)+
      (keyB[2]==1?-127:0)+
      (keyB[3]==1?-127:0), -110, 110));
    myPort.write(buf);
    delay(50);
  }
}

void setup() {
  fullScreen();
  printArray(Serial.list());
  if (Serial.list().length>0)
    myPort = new Serial(this, Serial.list()[0], 500000);
  print(myPort);
  buf[0]='s';
  buf[1]='t';
  buf[4]='d';
  buf[5]='o';
  thread("sendMsg");
}

void draw() {
  background(20);
  if (myPort==null)
    text("Err", 2, 12);
  else
    text("Ok", 2, 12);
  text(v, 2, 24);
  //text(buf[2], 2, 36);
  //text(buf[3], 2, 48);
}

void keyPressed() {
  if (key=='w' || key=='W')keyB[0]=1;
  if (key=='a' || key=='A')keyB[1]=1;
  if (key=='s' || key=='S')keyB[2]=1;
  if (key=='d' || key=='D')keyB[3]=1;
}

void keyReleased() {
  if (key=='w' || key=='W')keyB[0]=0;
  if (key=='a' || key=='A')keyB[1]=0;
  if (key=='s' || key=='S')keyB[2]=0;
  if (key=='d' || key=='D')keyB[3]=0;
}
