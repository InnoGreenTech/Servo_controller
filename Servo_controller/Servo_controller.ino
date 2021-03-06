
//#include <avr/interrupt.h>
#include <Arduino.h>
#include <registers.h>


 /*    EEPROM  ADDRESS   */


    #include <EEPROM.h>

    

 /*    Servos memory setting    */


    #define OUTPUT_SERVO_1 7                  // define output
    #define OUTPUT_SERVO_2 8
    #define OUTPUT_SERVO_3 9
    #define OUTPUT_SERVO_4 10
    #define OUTPUT_SERVO_5 11
    #define OUTPUT_SERVO_6 12
    #define POWER_SERVOS   6


    byte  analog_servo;                       // use to active or desactivate servo
    byte  move_mode;                          // use to active move mode
    byte  edge_push_buttom;                   // use mode push bottom


    int  servos[6][7];                       //use to set and control servos

      #define SERVO_1 0
      #define SERVO_2 1
      #define SERVO_3 2
      #define SERVO_4 3
      #define SERVO_5 4
      #define SERVO_6 5

      #define POWER_SERVOS 6
  
      #define END     0       // End position
      #define ZERO    1       // Zero position  
      #define ACC_MAX 2       // acceleration or deceleration of servo 0 to 250 per period see bellow
      #define CUR     3       // current postion
      #define DEMAND  4       // demand postion
      #define MIDLE   5       // Midle position 
      #define ACC     6       // Midle position 
      
     byte nb_20ms;
     
     #define PERIOD_ACCELERATION 2  // To adjust the number of cycles bettween to increase servo position. exemple= 4x20ms = 80 ms 

 /* Message user traitement */

    byte    statut_message=0;
    #define MESSAGE_IN_READ 0
    #define MESSAGE_READY   1
    #define MESSAGE_ERROR   2
    #define DATA_READY      3
    
    
    String message="";
    String label="";
    String donnee="";
    char   c;
    int    position_string;  

 /* Input setting */

   #define CONTROL_SERVO_1    A0
   #define CONTROL_SERVO_2    A1
   #define CONTROL_SERVO_3    A2
   #define CONTROL_SERVO_4    A3
   #define CONTROL_SERVO_5    A4
   #define CONTROL_SERVO_6    A5

   byte current_servo=0;
   byte i=0;

/* Control Battery */

  #define V_BATTERY           A6
  #define R1                  10
  #define R2                  10
  #define R_SCALE           (R1+R2)/R2       //R2 

  float   v_battery;
  float   battery_limit;

/* Standby mode */

  unsigned long delay_standby;
  unsigned long start_delay_standby;
  
  #define DELAY_ENABLE_SERVOS   200
  unsigned long delay_enable_servos;
  
    
void setup() {
  
  pinMode(POWER_SERVOS,OUTPUT);
  digitalWrite(POWER_SERVOS,0);
  
  
  SMCR=0;
  
  Serial.begin(115200);

  /* Read eeprom memory */

    
  read_eeprom();


  /* Set timer 1 to control servo motors */

    TCCR1A = 0;//_BV(COM1A0)| _BV(COM1B0);     //Active match compare Toogle
    TCCR1B = _BV(CS10)|_BV(CS11);        //  _BV(WGM12)|set prescaler of  CTC mode
    TCNT1 = 0;                            // clear the timer count
    TIMSK1 =  _BV(OCIE1A)|_BV(OCIE1B);   // enable the output compare interrupt
    OCR1A  = 250;                           // 
    OCR1B  = 5000;                      // 20 ms


    
  Serial.println();
  Serial.println(F("InnoGreenTech vous êtes connecté à votre controleur."));
  Serial.println(F("Vous pouvez enregistrer de nouveaux paramétres en suivant cet exemple de synthaxe: S3A=200 "));

 /* Pin out control*/
 
  pinMode(OUTPUT_SERVO_1,OUTPUT);
  pinMode(OUTPUT_SERVO_2,OUTPUT);
  pinMode(OUTPUT_SERVO_3,OUTPUT);
  pinMode(OUTPUT_SERVO_4,OUTPUT);
  pinMode(OUTPUT_SERVO_5,OUTPUT);
  pinMode(OUTPUT_SERVO_6,OUTPUT);


  if(!bitRead(analog_servo,SERVO_1)){pinMode(CONTROL_SERVO_1,INPUT_PULLUP);}
  if(!bitRead(analog_servo,SERVO_2)){pinMode(CONTROL_SERVO_2,INPUT_PULLUP);}
  if(!bitRead(analog_servo,SERVO_3)){pinMode(CONTROL_SERVO_3,INPUT_PULLUP);}
  if(!bitRead(analog_servo,SERVO_4)){pinMode(CONTROL_SERVO_4,INPUT_PULLUP);}
  if(!bitRead(analog_servo,SERVO_5)){pinMode(CONTROL_SERVO_5,INPUT_PULLUP);}
  if(!bitRead(analog_servo,SERVO_6)){pinMode(CONTROL_SERVO_6,INPUT_PULLUP);}

/* auto adjust midle position */

  servos[SERVO_1][MIDLE]=analogRead(CONTROL_SERVO_1);
  servos[SERVO_2][MIDLE]=analogRead(CONTROL_SERVO_2);
  servos[SERVO_3][MIDLE]=analogRead(CONTROL_SERVO_3);
  servos[SERVO_4][MIDLE]=analogRead(CONTROL_SERVO_4);
  servos[SERVO_5][MIDLE]=analogRead(CONTROL_SERVO_5);
  servos[SERVO_6][MIDLE]=analogRead(CONTROL_SERVO_6);

/* Init ACC information */
  servos[SERVO_1][ACC]=servos[SERVO_1][ACC_MAX];
  servos[SERVO_2][ACC]=servos[SERVO_2][ACC_MAX];
  servos[SERVO_3][ACC]=servos[SERVO_3][ACC_MAX];
  servos[SERVO_4][ACC]=servos[SERVO_4][ACC_MAX];
  servos[SERVO_5][ACC]=servos[SERVO_5][ACC_MAX];
  servos[SERVO_6][ACC]=servos[SERVO_6][ACC_MAX];    

  delay_enable_servos=millis();

  /* External interupt */

 PCICR = _BV(PCIE1);  // Enable pin change interrupt on analog input

  //PCIFR flag interrupt

 PCMSK1= B00111111;  // Enable each input
  

}

void loop() {
  
  int analog_value; 
  
  analog_value=analogRead(CONTROL_SERVO_1);
  if(bitRead(analog_servo,SERVO_1)){
    if( bitRead(move_mode,SERVO_1)){
      if (analog_value>(servos[SERVO_1][MIDLE]+5)){servos[SERVO_1][DEMAND]=servos[SERVO_1][END];}
      else if (analog_value<(servos[SERVO_1][MIDLE]-5)){servos[SERVO_1][DEMAND]=servos[SERVO_1][ZERO];}
      else {servos[SERVO_1][DEMAND]=servos[SERVO_1][CUR];}
      analog_value=abs(servos[SERVO_1][MIDLE]-analog_value);
      servos[SERVO_1][ACC]=map(analog_value,0,512,servos[SERVO_1][ZERO],servos[SERVO_1][END]);
    }
    else{servos[SERVO_1][DEMAND]=map(analog_value,1024,0,0,servos[SERVO_1][ACC_MAX]);}
    }
  else if(bitRead(move_mode,SERVO_1)){  
      
    if (bitRead(edge_push_buttom,SERVO_1)){if (analog_value>700){bitClear(edge_push_buttom,SERVO_1);}}
    else if(analog_value<100 and servos[SERVO_1][DEMAND]==servos[SERVO_1][END]){servos[SERVO_1][DEMAND]=servos[SERVO_1][ZERO];bitSet(edge_push_buttom,SERVO_1);}
    else if(analog_value<100 and servos[SERVO_1][DEMAND]==servos[SERVO_1][ZERO]){servos[SERVO_1][DEMAND]=servos[SERVO_1][END];bitSet(edge_push_buttom,SERVO_1);}      
  }  
  else if(analog_value<100){servos[SERVO_1][DEMAND]=servos[SERVO_1][END];}else if(analog_value>700){servos[SERVO_1][DEMAND]=servos[SERVO_1][ZERO];}
  
  analog_value=analogRead(CONTROL_SERVO_2);
  if(bitRead(analog_servo,SERVO_2)){
      if( bitRead(move_mode,SERVO_2)){
        if (analog_value>(servos[SERVO_2][MIDLE]+5)){servos[SERVO_2][DEMAND]=servos[SERVO_2][END];}
        else if (analog_value<(servos[SERVO_2][MIDLE]-5)){servos[SERVO_2][DEMAND]=servos[SERVO_2][ZERO];}
        else {servos[SERVO_2][DEMAND]=servos[SERVO_2][CUR];}
        analog_value=abs(servos[SERVO_2][MIDLE]-analog_value);
        servos[SERVO_2][ACC]=map(analog_value,0,512,0,servos[SERVO_2][ACC_MAX]);
      }
      else{servos[SERVO_2][DEMAND]=map(analog_value,1024,0,servos[SERVO_2][ZERO],servos[SERVO_2][END]);}
  }
  else if(bitRead(move_mode,SERVO_2)){    
    if (bitRead(edge_push_buttom,SERVO_2)){if (analog_value>700){bitClear(edge_push_buttom,SERVO_2);}}
    else if(analog_value<100 and servos[SERVO_2][DEMAND]==servos[SERVO_2][END]){servos[SERVO_2][DEMAND]=servos[SERVO_2][ZERO];bitSet(edge_push_buttom,SERVO_2);}
    else if(analog_value<100 and servos[SERVO_2][DEMAND]==servos[SERVO_2][ZERO]){servos[SERVO_2][DEMAND]=servos[SERVO_2][END];bitSet(edge_push_buttom,SERVO_2);}      
  }
  else if(analog_value<100){servos[SERVO_2][DEMAND]=servos[SERVO_2][END];}else if(analog_value>700){servos[SERVO_2][DEMAND]=servos[SERVO_2][ZERO];}
  
  
  
    
  analog_value=analogRead(CONTROL_SERVO_3);
  if(bitRead(analog_servo,SERVO_3)){
      if( bitRead(move_mode,SERVO_3)){
        if (analog_value>(servos[SERVO_3][MIDLE]+5)){servos[SERVO_3][DEMAND]=servos[SERVO_3][END];}
        else if (analog_value<(servos[SERVO_3][MIDLE]-5)){servos[SERVO_3][DEMAND]=servos[SERVO_3][ZERO];}
        else {servos[SERVO_3][DEMAND]=servos[SERVO_3][CUR];}
        analog_value=abs(servos[SERVO_3][MIDLE]-analog_value);
        servos[SERVO_3][ACC]=map(analog_value,0,512,0,servos[SERVO_3][ACC_MAX]);
      }
      else{servos[SERVO_3][DEMAND]=map(analog_value,1024,0,servos[SERVO_3][ZERO],servos[SERVO_3][END]);}
  }
  else if(bitRead(move_mode,SERVO_3)){    
    if (bitRead(edge_push_buttom,SERVO_3)){if (analog_value>700){bitClear(edge_push_buttom,SERVO_3);}}
    else if(analog_value<100 and servos[SERVO_3][DEMAND]==servos[SERVO_3][END]){servos[SERVO_3][DEMAND]=servos[SERVO_3][ZERO];bitSet(edge_push_buttom,SERVO_3);}
    else if(analog_value<100 and servos[SERVO_3][DEMAND]==servos[SERVO_3][ZERO]){servos[SERVO_3][DEMAND]=servos[SERVO_3][END];bitSet(edge_push_buttom,SERVO_3);}      
  }
  else if(analog_value<100){servos[SERVO_3][DEMAND]=servos[SERVO_3][END];}else if(analog_value>700){servos[SERVO_3][DEMAND]=servos[SERVO_3][ZERO];}
    
  analog_value=analogRead(CONTROL_SERVO_4);
  if(bitRead(analog_servo,SERVO_4)){
      if( bitRead(move_mode,SERVO_4)){
        if (analog_value>(servos[SERVO_4][MIDLE]+5)){servos[SERVO_4][DEMAND]=servos[SERVO_4][END];}
        else if (analog_value<(servos[SERVO_4][MIDLE]-5)){servos[SERVO_4][DEMAND]=servos[SERVO_4][ZERO];}
        else {servos[SERVO_4][DEMAND]=servos[SERVO_4][CUR];}
        analog_value=abs(servos[SERVO_4][MIDLE]-analog_value);
        servos[SERVO_4][ACC]=map(analog_value,0,512,0,servos[SERVO_4][ACC_MAX]);
      }
      else{servos[SERVO_4][DEMAND]=map(analog_value,1024,0,servos[SERVO_4][ZERO],servos[SERVO_4][END]);}
  } 
  else if(bitRead(move_mode,SERVO_4)){    
    if (bitRead(edge_push_buttom,SERVO_4)){if (analog_value>700){bitClear(edge_push_buttom,SERVO_4);}}
    else if(analog_value<100 and servos[SERVO_4][DEMAND]==servos[SERVO_4][END]){servos[SERVO_4][DEMAND]=servos[SERVO_4][ZERO];bitSet(edge_push_buttom,SERVO_4);}
    else if(analog_value<100 and servos[SERVO_4][DEMAND]==servos[SERVO_4][ZERO]){servos[SERVO_4][DEMAND]=servos[SERVO_4][END];bitSet(edge_push_buttom,SERVO_4);}      
  }
  else if(analog_value<100){servos[SERVO_4][DEMAND]=servos[SERVO_4][END];}else if(analog_value>700){servos[SERVO_4][DEMAND]=servos[SERVO_4][ZERO];}
    
  analog_value=analogRead(CONTROL_SERVO_5);
  if(bitRead(analog_servo,SERVO_5)){
      if( bitRead(move_mode,SERVO_5)){
        if (analog_value>(servos[SERVO_5][MIDLE]+5)){servos[SERVO_5][DEMAND]=servos[SERVO_5][END];}
        else if (analog_value<(servos[SERVO_5][MIDLE]-5)){servos[SERVO_5][DEMAND]=servos[SERVO_5][ZERO];}
        else {servos[SERVO_5][DEMAND]=servos[SERVO_5][CUR];}
        analog_value=abs(servos[SERVO_5][MIDLE]-analog_value);
        servos[SERVO_5][ACC]=map(analog_value,0,512,0,servos[SERVO_5][ACC_MAX]);
      }
      else{servos[SERVO_5][DEMAND]=map(analog_value,1024,0,servos[SERVO_5][ZERO],servos[SERVO_5][END]);}
  }
  else if(bitRead(move_mode,SERVO_5)){    
    if (bitRead(edge_push_buttom,SERVO_5)){if (analog_value>700){bitClear(edge_push_buttom,SERVO_5);}}
    else if(analog_value<100 and servos[SERVO_5][DEMAND]==servos[SERVO_5][END]){servos[SERVO_5][DEMAND]=servos[SERVO_5][ZERO];bitSet(edge_push_buttom,SERVO_5);}
    else if(analog_value<100 and servos[SERVO_5][DEMAND]==servos[SERVO_5][ZERO]){servos[SERVO_5][DEMAND]=servos[SERVO_5][END];bitSet(edge_push_buttom,SERVO_5);}      
  }
  else if(analog_value<100){servos[SERVO_5][DEMAND]=servos[SERVO_5][END];}else if(analog_value>700){servos[SERVO_5][DEMAND]=servos[SERVO_5][ZERO];}

  analog_value=analogRead(CONTROL_SERVO_6);
  if(bitRead(analog_servo,SERVO_6)){
      if( bitRead(move_mode,SERVO_6)){
        if (analog_value>(servos[SERVO_6][MIDLE]+5)){servos[SERVO_6][DEMAND]=servos[SERVO_6][END];}
        else if (analog_value<(servos[SERVO_6][MIDLE]-5)){servos[SERVO_6][DEMAND]=servos[SERVO_6][ZERO];}
        else {servos[SERVO_6][DEMAND]=servos[SERVO_6][CUR];}
        analog_value=abs(servos[SERVO_6][MIDLE]-analog_value);
        servos[SERVO_6][ACC]=map(analog_value,0,512,0,servos[SERVO_6][ACC_MAX]);
      }
      else{servos[SERVO_6][DEMAND]=map(analog_value,1024,0,servos[SERVO_6][ZERO],servos[SERVO_6][END]);}
  }
  else if(bitRead(move_mode,SERVO_6)){    
    if (bitRead(edge_push_buttom,SERVO_6)){if (analog_value>700){bitClear(edge_push_buttom,SERVO_6);}}
    else if(analog_value<100 and servos[SERVO_6][DEMAND]==servos[SERVO_6][END]){servos[SERVO_6][DEMAND]=servos[SERVO_6][ZERO];bitSet(edge_push_buttom,SERVO_6);}
    else if(analog_value<100 and servos[SERVO_6][DEMAND]==servos[SERVO_6][ZERO]){servos[SERVO_6][DEMAND]=servos[SERVO_6][END];bitSet(edge_push_buttom,SERVO_6);}      
  }
  else if(analog_value<100){servos[SERVO_6][DEMAND]=servos[SERVO_6][END];}else if(analog_value>700){servos[SERVO_6][DEMAND]=servos[SERVO_6][ZERO];}
  
  serial_command();
  
  if (bitRead(statut_message,DATA_READY)){
    check_message();  
  }
 
  control_battery();

  if(millis()> start_delay_standby+delay_standby){
    active_sleep_mode();
  }

  if(!digitalRead(POWER_SERVOS)){
    if (millis()>(delay_enable_servos+DELAY_ENABLE_SERVOS)){digitalWrite(POWER_SERVOS,1);Serial.println("Servo enable");}  
    for (int i=0; i>6; i++){
      if (bitRead(move_mode,i)){
        servos[i][CUR]=servos[SERVO_1][ZERO]; 
      }
      else{servos[i][CUR]=servos[i][DEMAND]; }
    }

  }

}
