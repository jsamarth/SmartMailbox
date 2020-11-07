#include <driverlib/gpio.h>

const char keymap[4][4]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
 };
const int cpin[]={5,4,3,2};
const int rpin[]={9,8,7,6};
void setup()
{
  Serial.begin(115200);
  Serial.println("Program starting");//for debugging purpose and canbe removed later 
  for(int i=0;i<4;i++)
  {
//Should be INPUT_PULLUP and OUTPUT
    pinMode(rpin[i],OUTPUT);
    pinMode(cpin[i],INPUT_PULLDOWN);
  }
}
void loop()
{
  char key=getkey();
  if(key!=0)
  {
   Serial.println(key);
   delay(500);              // Also, consider debouncing. 
  }  
}
char getkey()
{
  char key=0;
  for(int i=0;i<4;i++)
  {
    digitalWrite(rpin[i],HIGH);
    for(int j=0;j<4;j++)
    {
      if (digitalRead(cpin[j])==HIGH)
        key=keymap[i][j];
    }
    digitalWrite(rpin[i],LOW);
  }
  return key;
}