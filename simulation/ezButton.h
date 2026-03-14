class ezButton
{
public:
  ezButton(int pin);
  ezButton(int pin, int mode);
  void setDebounceTime(unsigned long time);
  int getState(void);
  //int getStateRaw(void);
  //bool isPressed(void);
  //bool isReleased(void);
  void loop(void);
};
