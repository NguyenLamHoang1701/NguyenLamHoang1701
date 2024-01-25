#include <PS2X_lib.h>

PS2X ps2x; 


#define PS2_DAT 12 
#define PS2_CMD 13 
#define PS2_SEL 15
#define PS2_CLK 14 

#define TOP_SPEED 4095
#define NORM_SPEED 3900
#define TURNING_FACTOR 1

#define SINGLE_HAND_DRIVING 1
#define TWO_HAND_DRIVING 0
bool driving_mode = SINGLE_HAND_DRIVING;
void setupPS2controller()
{
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }

}
bool PS2control()
{
  int speed = TOP_SPEED;
  if (ps2x.ButtonPressed(PSB_L1))
    driving_mode =!driving_mode;
  
  int nJoyX = 127 - ps2x.Analog(PSS_RX); 
  int nJoyY = 128 - (driving_mode ? ps2x.Analog(PSS_RY) :ps2x.Analog(PSS_LY)); 
  int nMotMixL;                         
  int nMotMixR;                       

  if(nJoyX == -1 && nJoyY == 0) 
  {
    setPWMMotors(0, 0, 0, 0);
    return 0;
  }

  bool temp = (nJoyY * nJoyX > 0);
  if (nJoyX) 
  {
    nMotMixL = -nJoyX + (nJoyY * temp);
    nMotMixR = nJoyX + (nJoyY * !temp);
  }
  else 
  {
    nMotMixL = nJoyY;
    nMotMixR = nJoyY;
  }
 
  Serial.print(F("Calculated value from joystick: "));
  Serial.print(nMotMixL);
  Serial.print("\t");
  Serial.println(nMotMixR);
 
  int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  if (nMotMixR > 0) //tiến
  {
    c3 = nMotMixR;
    Serial.print("tiến");
    c3 = map(c3, 0, 128, 0, speed);
  }

  else if (nMotMixR < 0)
  {
    c4 = abs(nMotMixR) ;
    c4 = map(c4, 0, 128, 0, speed);
  }

  if (nMotMixL > 0)
  {
    c1 = nMotMixL;
    c1 = map(c1, 0, 128, 0, speed);
  }
  else if (nMotMixL < 0)
  {
    c2 = abs(nMotMixL);
    c2 = map(c2, 0, 128, 0, speed);
  }
  setPWMMotors(c1, c2, c3, c4);
  return 1;
}
// ban bong 
void shooter_On(){
  pwm.setPWM(12,0,4095);
  pwm.setPWM(13,0,0);
}
void shooter_Off(){
  pwm.setPWM(12,0,0);
  pwm.setPWM(13,0,0);
}
// thu bong va sap xep bong
void thubong_on(){
  pwm.setPWM(14,0,3900);
  pwm.setPWM(15,0,0);
}
void thubong_off(){
  pwm.setPWM(14,0,0);
  pwm.setPWM(15,0,0);
}
// dieu khien ps2
void PS2dieukhien(){
  if (ps2x.Button(PSB_PAD_UP)){
    shooter_On();
  } 
  if (ps2x.Button(PSB_PAD_DOWN)){
    shooter_Off();
  } 
  if (ps2x.Button(PSB_GREEN)){
    thubong_on();
  }
  if (ps2x.Button(PSB_BLUE)){
    thubong_off();
  }
 }
// dieu khien servo
void ps2Controlservo(){
  //intake white tower - servo 360
  if (ps2x.ButtonPressed(PSB_R2)){
    pwm.setPWM(4, 0, 404);
  }
  if (ps2x.Button(PSB_R1)){
    
    pwm.setPWM(4, 0, 0);
  
  }
  //open shooter - servo 180
  if (ps2x.ButtonPressed(PSB_RED)){
    pwm.setPWM(5, 0,404);
  }
  if (ps2x.Button(PSB_PINK)){
    
    pwm.setPWM(5, 0, 0);
  }
  //intake - servo 360 
  if (ps2x.ButtonPressed(PSB_PINK)){
    pwm.setPWM(6, 0, 404);
  }
  if (ps2x.Button(PSB_BLUE)){
    
    pwm.setPWM(6, 0, 0);
  }
}
