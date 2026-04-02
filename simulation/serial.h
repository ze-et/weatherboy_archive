#pragma once
// Header made for the simulation;
// communicates with the Processing program via shared memory.


struct joyStruct;

class Serial{
private:
  char* shm_map;
  int write_inc;
public:
  Serial();
  ~Serial();

  void begin(int); // Does nothing
  void write(char);
  joyStruct get_input();
};
