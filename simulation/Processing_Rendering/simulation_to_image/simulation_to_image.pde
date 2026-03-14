byte[] image_array = new byte[1]; //[1024]
byte[] text_array = new byte[0];

final char empty_char = '.';
final char fill_char = '#';
final char newline_char = '\n';

final int scale = 15;
int image_saved_count = 0;

void settings()
{
  size(128*scale, 64*scale);
}

void setup()
{
  image_array[0] = 0;
  int byte_idx = 0;
  byte bit_offset = 0;
  byte new_bit;
  text_array = loadBytes("input.txt");
  for(byte character : text_array){
    new_bit = -1;
    if(character == empty_char){
      new_bit = 0;
    }
    else if(character == fill_char){
      new_bit = 1;
    }

    if(new_bit != -1){
      if(bit_offset == 8){
        byte_idx += 1;
        bit_offset = 0;
        image_array = append(image_array, byte(0));
      }
      image_array[byte_idx] |= new_bit << (7-bit_offset);
      bit_offset += 1;
    }
  }
  while(image_array.length >= 1024){
    save_image();
    image_array = subset(image_array, 1024);
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
