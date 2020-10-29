ISR(TIMER1_COMPA_vect) {
   

    if(current_servo==0){OCR1A=600;bitClear(PORTD,7);}
    if(current_servo==1){OCR1A=servos[SERVO_2][CUR]+850;bitSet(PORTB,0);}
    if(current_servo==2){OCR1A=1200;bitClear(PORTB,0);}    
    if(current_servo==3){OCR1A=servos[SERVO_3][CUR]+1450;bitSet(PORTB,1);}
    if(current_servo==4){OCR1A=1800;bitClear(PORTB,1);}
    if(current_servo==5){OCR1A=servos[SERVO_4][CUR]+2050;bitSet(PORTB,2);}
    if(current_servo==6){OCR1A=2400;bitClear(PORTB,2);}
    if(current_servo==7){OCR1A=servos[SERVO_5][CUR]+2650;bitSet(PORTB,3);}
    if(current_servo==8){OCR1A=3000;bitClear(PORTB,3);}
    if(current_servo==9){OCR1A=servos[SERVO_6][CUR]+3250;bitSet(PORTB,4);}
    if(current_servo==10){bitClear(PORTB,4);}
    
    current_servo++;


}

ISR(TIMER1_COMPB_vect) {

  nb_20ms++; 
  if(nb_20ms==PERIOD_ACCELERATION){                                                        
    for (int i=0; i<6; i++){
    if (servos[i][CUR]<(servos[i][DEMAND]-servos[i][ACC])){servos[i][CUR]=servos[i][CUR]+servos[i][ACC];}            // change values to manage acceleration or decceleration
    else if(servos[i][CUR]>(servos[i][DEMAND]+servos[i][ACC])){servos[i][CUR]=servos[i][CUR]-servos[i][ACC];}
    else {servos[i][CUR]=servos[i][DEMAND];}
    nb_20ms=0;
    }
  }
    current_servo=0;
    OCR1A=servos[SERVO_1][CUR]+250;
    bitSet(PORTD,7);
    TCNT1 = 0;  
  }
