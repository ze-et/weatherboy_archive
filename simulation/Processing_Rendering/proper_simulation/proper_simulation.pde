import java.nio.MappedByteBuffer;
import java.nio.ByteBuffer;
import java.io.RandomAccessFile;
import java.nio.channels.FileChannel;
import java.io.IOException;

byte[] image_array = new byte[1024];

byte input_byte;
final String FILE_NAME = "/dev/shm/shm_weatherboy";
final byte marked_byte = (byte) 0b10101010;

RandomAccessFile file;
MappedByteBuffer serial;

final int scale = 8;
int image_saved_count = 0;

void settings()
{
  size(128*scale, 64*scale);
}

void setup()
{
  try{
    file = new RandomAccessFile(FILE_NAME, "rwd");
    serial = file.getChannel().map(FileChannel.MapMode.READ_WRITE, 0, 1025);
  }
  catch(IOException e){
    println("Error in setup: " + e);
  }
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
  println("Image saved as image"+str(image_saved_count)+".png");
  pg.save("image"+str(image_saved_count)+".png");
  image_saved_count += 1;
}

void draw()
{
  try{
    ByteBuffer duplicate_buffer = serial.duplicate();
    duplicate_buffer.get(image_array, 0, 1024);
  }
  catch(Exception e){
    println("Error in draw: " + e);
    exit();
  }

  if(image_array.length == 1024)
  {
    //print("Oldms:"); print(millis());
    render_image();
    //print("Newms:"); println(millis());
  }
  delay(33);
}

void keyPressed()
{
  if (key == CODED) {
    switch(keyCode){
      case UP:
        input_byte |= 0b00000001;
        break;
      case DOWN:
        input_byte |= 0b00000010;
        break;
      case LEFT:
        input_byte |= 0b00000100;
        break;
      case RIGHT:
        input_byte |= 0b00001000;
        break;
    }
  }
  switch(key){
    case ESC: // pressing ESC implicitly exits the program
      input_byte |= 0b10000000;
      break;
    case TAB:
      save_image();
      break;
    case ENTER:
      input_byte |= 0b00010000;
      break;
  }
  serial.put(1024, input_byte);

}

void keyReleased()
{
  if (key == CODED) {
    switch(keyCode){
      case UP:
        input_byte &= 0b11111110;
        break;
      case DOWN:
        input_byte &= 0b11111101;
        break;
      case LEFT:
        input_byte &= 0b11111011;
        break;
      case RIGHT:
        input_byte &= 0b11110111;
        break;
    }
  }
  if(key == ENTER){
    input_byte &= 0b11101111;
  }
  serial.put(1024, input_byte);
}


public void dispose() {
  try{
    file.close();
  }
  catch(IOException e){
    println("Error in dispose: " + e);
  }
  println("Sketch closed regularly.");
  super.dispose();
}
