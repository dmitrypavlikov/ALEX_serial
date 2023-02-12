#include <Servo.h>
#include "DxlMaster.h"


const uint8_t LW_id = 1;      //Left Wheel id
const uint8_t RW_id = 2;      //Right Wheel id
//Servos does not have ID

int SPD_L = 0;                //Initial speed of left wheel
int SPD_R = 0;                //Initial speed of right wheel
int BODY_ANGLE = 0;           //Initial angle of body
int HAND_ANGLE = 0;           //Initial angle of hand

int present_SPD_L = 0;        //Present speed of left wheel
int present_SPD_R = 0;        //Present speed of right wheel
int present_BODY_ANGLE = 0;   //Present angle of body
int present_HAND_ANGLE = 0;   //Present angle of hand

const long unsigned int baudrate = 1000000;


DynamixelDevice LW(LW_id);
DynamixelDevice RW(RW_id);
Servo BODY;
Servo HAND;

void setup() 
{ Serial.begin(9600);         //Отладочный Serial
  Serial3.begin(9600);        //Общение с NanoPi
  Serial3.setTimeout(5);      //Частота синхронизации по Rx Tx
  DxlMaster.begin(baudrate);  //Канал общения с сервами
  delay(10);

  LW.init();
  RW.init();
  BODY.attach(12);          //PWM pin for BODY servo
  HAND.attach(13);          //PWM pin for HAND servo
  
  LW.write(0x28, 1);        //ENABLE_TORQUE
  RW.write(0x28, 1);        //ENABLE_TORQUE
  
  LW.write(0x1A, 0);        //DYN_ADDRESS_CW_LIMIT
  RW.write(0x1A, 0);        //DYN_ADDRESS_CW_LIMIT
  
  LW.write(0x1B, 0);        //DYN_ADDRESS_CCW_LIMIT
  RW.write(0x1B, 0);        //DYN_ADDRESS_CCW_LIMIT
  
  LW.write(0x31, 1);        //wheel mode
  RW.write(0x31, 1);        //wheel mode
}

void loop() {
  LW.read(0x3A, present_SPD_L);         //Read vel of left wheel 
  RW.read(0x3A, present_SPD_R);         //Read vel of right wheel
  // Чтение данных с серв тела и рук не происходит. 
  //Отправляем то, что и получили, или самое начальное положение.
  
  char mail[20] = {};                   //Буфер для чтения сообщения 
  if (Serial3.available()){             //Проверка соединения по Rx Tx
    Serial3.readBytesUntil('\n', mail, 20);     //Чтение пакета до символа '\n'
    Serial.println(mail);                       //Дублирование пакета в отладочный Serial 
    
  }

  //Дальше дробим сообщение на числа и задаём их в сервы
  /* 
   *  
   * Дробим сообщение на числа  
   *  
   */

  //Запись положений в сервы
  /*
   * 
   * Запись положений в сервы
   * 
   */



  //Напоминалка о работе со скоростью (wheelMode) и чтению
  /*device.write(0x2E, makeSpeed(5000)); //DYN_ADDRESS_GOAL_SPEED
  device.read(0x38, pos);
  device.read(0x3A, vel);
  Serial.print(pos);
  Serial.print("   ");
  Serial.println(vel);
  delay(500);*/
}
