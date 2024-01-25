#include "motors.h"
#include "PS2_controller.h"

void setup()
{

  Serial.begin(115200);
  initMotors();
  setupPS2controller();
  Serial.println("Done setup!");
}

void loop()
{
  ps2x.read_gamepad(0, 0);
  PS2control();
  PS2dieukhien();
  ps2Controlservo();
  delay(50);

}