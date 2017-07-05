// ParticleBots
// Karolina Ladino Puerto
// Thesis of Automation Engineering
// June 2017 V1.0
// Utadeo

// Note: Based in an public example of light sensor TSL2561
// Libraries
#include "WEMOS_Motor.h"

int pwm;


//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30, _MOTOR_A, 1000); //Motor A
Motor M2(0x30,_MOTOR_B, 1000);//Motor B



void setup() {
  Serial.begin(19200);
}
int i=0;
void loop() {

  for (pwm = 40; pwm <= 100; pwm++)
  {
    if(i==0){
    i=1;
        M1.setmotor( _CW, pwm);
        M2.setmotor(_CCW, pwm);
    }
    else{
    i=0;
        M1.setmotor( _CCW, pwm);
        M2.setmotor(_CW, pwm);
    }

    Serial.printf("A:%d%, B:%d%, DIR:CW\r\n", pwm, 100 - pwm);
    delay(500);
    M1.setmotor(_STANDBY);
    M2.setmotor( _STANDBY);
    Serial.println("Motor A STANDBY");
    delay(100);
  }

}

void car_front(int speed){
  M1.setmotor( _CW, speed);
  M2.setmotor(_CW, speed);
  }

  void car_reverse(int speed){
  M1.setmotor( _CCW, speed);
  M2.setmotor(_CCW, speed);
  }
  void car_CW(int speed){
  M1.setmotor( _CCW, speed);
  M2.setmotor(_CW, speed);
  }
  void car_CCW(int speed){
  M1.setmotor( _CW, speed);
  M2.setmotor(_CCW, speed);
  }
