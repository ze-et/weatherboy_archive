import processing.serial.*;

Serial myPort;
byte[] image_array = new byte[0]; //[1024]

final byte marked_byte = (byte) 0b10101010;

final int scale = 15;
int image_saved_count = 0;

void settings()
{
  size(128*scale, 64*scale);
}

void setup(){
  String port = Serial.list()[0]; // NOTE: Abhängig vom Betriebssystem/angesteckter Peripherie
  myPort = new Serial(this, port, 115200);
  print("Waiting for Serial");
  int i = 0;
  while(myPort.readBytesUntil(marked_byte) == null)
  {
    if((i % 4096) == 0)
    {
      println(myPort.available());;
    }
    i += 1;
  }
  println("Setup finished");
}

void render_image()
{
  noStroke();
  background(0);
  fill(255);
  for(int y = 0; y < 64; y += 1)
  {
    for(int byte_x = 0; byte_x < 16; byte_x += 1)
    {
      for(int bit = 0; bit < 8; bit += 1)
      {
        if(((image_array[byte_x + y * 16] >> (7-bit)) & 1) == 1)
        {
          rect((byte_x * 8 + bit) * scale, y * scale, scale, scale);
        }
      }
    }
  }
}
void save_image()
{
  final int scale = 2;
  PGraphics pg = createGraphics(128*scale, 64*scale);
  pg.beginDraw();
  pg.noStroke();
  pg.background(0);
  pg.fill(255);
  for(int y = 0; y < 64; y += 1)
  {
    for(int byte_x = 0; byte_x < 16; byte_x += 1)
    {
      for(int bit = 0; bit < 8; bit += 1)
      {
        if(((image_array[byte_x + y * 16] >> (7-bit)) & 1) == 1)
        {
          pg.rect((byte_x * 8 + bit) * scale, y * scale, scale, scale);
        }
      }
    }
  }
  pg.endDraw();
  pg.save("image"+str(image_saved_count++)+".png");
}

void draw()
{
  byte[] buffer = null;
  if (myPort.available() > 1024)
  {
    //buffer = myPort.readBytesUntil(marked_byte);
    buffer = myPort.readBytes(1025);
    if (buffer.length == 1025)
    {
      image_array = shorten(buffer);
    }
  }
  if(image_array.length == 1024)
  {
    //print("Oldms:"); print(millis());
    render_image();
    //print("Newms:"); println(millis());
  }
}

void keyPressed() {
  if (key == CODED) {
    if (keyCode == UP) {
      save_image();
      println("Image saved");
    }
  }
}
