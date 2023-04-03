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

String sendMail(){

  LW.read(0x3A, present_SPD_L);         //Read vel of left wheel (0x38 - позиция / 0x3A - скорость)
  RW.read(0x3A, present_SPD_R);         //Read vel of right wheel (0x38 - позиция / 0x3A - скорость)
  present_BODY_ANGLE = BODY.read();     //Read angle of body servo
  present_HAND_ANGLE = HAND.read();     //Read andle of hand servo
  
  //Формат сообщения (+XXXX,+XXXX,XXX,XXX.)
  //                 (LW,RW,BODY,HAND.)
  return String(present_SPD_L) + ',' + String(present_SPD_R) + ',' 
         + String(present_BODY_ANGLE) + ',' + String(present_HAND_ANGLE) + '.';
}

void readMail(){
  char mail[20] = {};                           //Буфер для чтения сообщения (+XXXX,+XXXX,XXX,XXX.)
  if (Serial3.available()){                     //Проверка почтового ящика по Rx Tx
    Serial3.readBytesUntil('.', mail, 20);      //Чтение пакета до символа '.'
    Serial.println(mail);                       //Дублирование полученного пакета в отладочный Serial 
    
    //Обнуление всех переменных перед записью. Пока не уверен, нужно ли...
    SPD_L = 0;
    SPD_R = 0;
    BODY_ANGLE = 0;
    HAND_ANGLE = 0;                     

    String num;
    short int counter = 0;
    for(int i = 0; i < sizeof(mail); i++){
      if((mail[i] != ',')&&(mail[i] != ' ')){
        num += mail[i];}
      else{ 
        switch(counter){
          case 0:
            SPD_L = num.toInt();
            break;
          case 1:
            SPD_R = num.toInt();
            break;
          case 2:
            BODY_ANGLE = num.toInt();
            break;
          /*case 3:
            HAND_ANGLE = num.toInt();
            Serial.print(HAND_ANGLE);
            break;*/}
        counter++;
        num = "";    
      }
    }
    HAND_ANGLE = num.toInt();

    //Прямая передача данных на моторы возможна тут
    //Прямая передача данных на сервы возможна тут
    
  }
}



void setup() 
{ Serial.begin(9600);         //Отладочный Serial
  Serial3.begin(9600);        //Общение с NanoPi
  Serial3.setTimeout(5);      //Частота синхронизации по Rx Tx
  DxlMaster.begin(baudrate);  //Канал общения с сервами
  delay(10);

  LW.init();
  RW.init();
  BODY.attach(12);            //PWM pin for BODY servo
  HAND.attach(13);            //PWM pin for HAND servo
  
  LW.write(0x28, 1);          //ENABLE_TORQUE
  RW.write(0x28, 1);          //ENABLE_TORQUE
  
  LW.write(0x1A, 0);          //DYN_ADDRESS_CW_LIMIT
  RW.write(0x1A, 0);          //DYN_ADDRESS_CW_LIMIT
  
  LW.write(0x1B, 0);          //DYN_ADDRESS_CCW_LIMIT
  RW.write(0x1B, 0);          //DYN_ADDRESS_CCW_LIMIT
  
  LW.write(0x21, 1);          //wheel mode
  RW.write(0x21, 1);          //wheel mode

  //Исходные положения и значения скоростей
  LW.write(0x2E, SPD_L);         //
  RW.write(0x2E, SPD_R);         //
  BODY.write(BODY_ANGLE);        //Важно првильно закрепить сервы (сервы ведут отсчёт от нуля)
  HAND.write(HAND_ANGLE);        //Важно првильно закрепить сервы (сервы ведут отсчёт от нуля) 
}

void loop() {  
  
  //Serial.println(sendMail());    //Отправка пакета информации в отлидочный Serial
  Serial3.println(sendMail());   //Отправка пакета информации на NanoPi 

  readMail();                    //Приём пакета данных от NanoPi (и пряма передача на моторы и сервы)
  
}
