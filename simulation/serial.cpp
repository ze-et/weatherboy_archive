#include "serial.h"
#include "customtypes.h"
// shm_open()
#include <sys/mman.h>
// O_ constants
#include <fcntl.h>
// mode constants
#include <sys/stat.h>
// reading/writing
#include <unistd.h>

#define SHM_NAME "shm_weatherboy"
#define SHM_SIZE 1024 + 1 // screen pixel size + input

Serial::Serial() : write_inc(0)
{
  int fd;
  fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);
  ftruncate(fd, SHM_SIZE);
  shm_map = static_cast<char*>(mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
}
Serial::~Serial()
{
  shm_unlink(SHM_NAME);
}

void Serial::begin(int)
{
}

void Serial::write(char symbol)
{
  shm_map[write_inc] = symbol;
  write_inc = (write_inc + 1) & 1023;
}

joyStruct Serial::get_input()
{
  joyStruct joyResult;
  // NOTE: I considered casting directly to joyResult, but the standard does not
  //       guarantee the order of bitfields, making it rather risky.
  if(shm_map[1024] & 0b0000'0001){
    joyResult.up      = 1;
  }
  if(shm_map[1024] & 0b0000'0010){
    joyResult.down    = 1;
  }
  if(shm_map[1024] & 0b0000'0100){
    joyResult.left    = 1;
  }
  if(shm_map[1024] & 0b0000'1000){
    joyResult.right   = 1;
  }
  if(shm_map[1024] & 0b0001'0000){
    joyResult.pressed = 1;
  }
  if(shm_map[1024] & 0b1000'0000){
        exit(0);
  }
  return joyResult;
}
