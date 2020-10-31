void serial_command(){
  


  while(Serial.available() and !bitRead(statut_message, MESSAGE_READY)){

    start_delay_standby=millis();    
    c=Serial.read();
    Serial.print(c);
   
    
    if(c==0x0D and bitRead(statut_message,MESSAGE_IN_READ))
    {
     message += c;
     position_string++;     
     bitSet(statut_message,MESSAGE_READY);  
    }
    if(c==0x08){message.remove(position_string); position_string--;}

    else if (!bitRead(statut_message,MESSAGE_IN_READ)){bitSet(statut_message,MESSAGE_IN_READ);message += c;}
    else if (c==0x7F&&bitRead(statut_message,MESSAGE_IN_READ)){message.remove(position_string); position_string--;}     // control back space putty
    else {message += c;position_string++;}
  }

   if (bitRead(statut_message, MESSAGE_READY)&&!bitRead(statut_message, DATA_READY)){   
    position_string=0;
    Serial.println();
    int index;    
    label="";
    donnee="";  
    for (index = 0; message[index]!=0x3D&&index<5; index++){
      label+= message[index];

    }
    
    for (index=index+1; message[index]!=0x0D&&index<10 ; index++){
      donnee += message[index];
    }
        
    bitSet(statut_message,DATA_READY);
   
  }
}

void check_message(){

  
        byte valeur = (byte)(ceil((donnee.toFloat()*250)/360));
        String   succes=F("Enregistrement réussi");
        
        if(valeur>250||donnee.length()<1){Serial.print(F("La valeur donnée n'est pas valide elle doit être comprise entre 0 et 360: ")); Serial.println(donnee);}
        
      
        else if  (label=="S1R"){servos[SERVO_1][ZERO]=valeur;EEPROM.write(ZERO,valeur);Serial.println(succes);}
        else if  (label=="S1F"){servos[SERVO_1][END]=valeur;EEPROM.write(END,valeur);Serial.println(succes);}
        else if  (label=="S1A"){servos[SERVO_1][ACC_MAX]=valeur;EEPROM.write(ACC_MAX,valeur);Serial.println(succes);}
        else if  (label=="S1P"){if(valeur==0){bitClear(analog_servo,0);bitClear(move_mode,0);pinMode(CONTROL_SERVO_1,INPUT_PULLUP);}
                                else if (valeur==1){bitSet(analog_servo,0);bitClear(move_mode,0);}
                                else {bitSet(analog_servo,0);bitSet(move_mode,0);}                            
                                EEPROM.write(30,analog_servo);
                                EEPROM.write(36,analog_servo);
                                Serial.println(succes);pinMode(CONTROL_SERVO_1,INPUT);}

        else if  (label=="S2R"){servos[SERVO_2][ZERO]=valeur;EEPROM.write(ZERO+5,valeur);Serial.println(succes);}
        else if  (label=="S2F"){servos[SERVO_2][END]=valeur;EEPROM.write(END+5,valeur);Serial.println(succes);}
        else if  (label=="S2A"){servos[SERVO_2][ACC_MAX]=valeur;EEPROM.write(ACC_MAX+5,valeur);Serial.println(succes);}
        else if  (label=="S2P"){if(valeur==0){bitClear(analog_servo,1);bitClear(move_mode,1);pinMode(CONTROL_SERVO_2,INPUT_PULLUP);}
                                else if (valeur==1){bitSet(analog_servo,1);bitClear(move_mode,1);}
                                else{bitSet(analog_servo,1);bitSet(move_mode,1);} 
                                EEPROM.write(30,analog_servo);
                                EEPROM.write(36,analog_servo);
                                Serial.println(succes);pinMode(CONTROL_SERVO_2,INPUT);}

        else if  (label=="S3R"){servos[SERVO_3][ZERO]=valeur;EEPROM.write(ZERO+10,valeur);Serial.println(succes);}
        else if  (label=="S3F"){servos[SERVO_3][END]=valeur;EEPROM.write(END+10,valeur);Serial.println(succes);}
        else if  (label=="S3A"){servos[SERVO_3][ACC_MAX]=valeur;EEPROM.write(ACC_MAX+10,valeur);Serial.println(succes);}
        else if  (label=="S3P"){if(valeur==0){bitClear(analog_servo,2);bitClear(move_mode,2);pinMode(CONTROL_SERVO_3,INPUT_PULLUP);}
                                else if (valeur==1){bitSet(analog_servo,2);bitClear(move_mode,2);}
                                else{bitSet(analog_servo,2);bitSet(move_mode,2);}
                                EEPROM.write(30,analog_servo);
                                EEPROM.write(36,analog_servo);
                                Serial.println(succes);pinMode(CONTROL_SERVO_3,INPUT);}

        else if  (label=="S4R"){servos[SERVO_4][ZERO]=valeur;EEPROM.write(ZERO+15,valeur);Serial.println(succes);}
        else if  (label=="S4F"){servos[SERVO_4][END]=valeur;EEPROM.write(END+15,valeur);Serial.println(succes);}
        else if  (label=="S4A"){servos[SERVO_4][ACC_MAX]=valeur;EEPROM.write(ACC_MAX+15,valeur);Serial.println(succes);}
        else if  (label=="S4P"){if(valeur==0){bitClear(analog_servo,3);bitClear(move_mode,3);pinMode(CONTROL_SERVO_4,INPUT_PULLUP);}
                                else if (valeur==1){bitSet(analog_servo,3);bitClear(move_mode,3);}
                                else{bitSet(analog_servo,3);bitSet(move_mode,3);}
                                EEPROM.write(30,analog_servo);
                                EEPROM.write(36,analog_servo);
                                Serial.println(succes);
                                pinMode(CONTROL_SERVO_4,INPUT);}

        else if  (label=="S5R"){servos[SERVO_5][ZERO]=valeur;EEPROM.write(ZERO+20,valeur);Serial.println(succes);}
        else if  (label=="S5F"){servos[SERVO_5][END]=valeur;EEPROM.write(END+20,valeur);Serial.println(succes);}
        else if  (label=="S5A"){servos[SERVO_5][ACC_MAX]=valeur;EEPROM.write(ACC_MAX+20,valeur);Serial.println(succes);}
        else if  (label=="S5P"){if(valeur==0){bitClear(analog_servo,4);bitClear(move_mode,4);pinMode(CONTROL_SERVO_5,INPUT_PULLUP);}
                                else if (valeur==1){bitSet(analog_servo,4);bitClear(move_mode,4);}
                                else{bitSet(analog_servo,4);bitSet(move_mode,4);}
                                EEPROM.write(30,analog_servo);
                                EEPROM.write(36,analog_servo);
                                Serial.println(succes);
                                pinMode(CONTROL_SERVO_5,INPUT);}  

        else if  (label=="S6R"){servos[SERVO_6][ZERO]=valeur;EEPROM.write(ZERO+25,valeur);Serial.println(succes);}
        else if  (label=="S6F"){servos[SERVO_6][END]=valeur;EEPROM.write(END+25,valeur);Serial.println(succes);}
        else if  (label=="S6A"){servos[SERVO_6][ACC_MAX]=valeur;EEPROM.write(ACC_MAX+25,valeur);Serial.println(succes);}
        else if  (label=="S6P"){if(valeur==0){bitClear(analog_servo,5);bitClear(move_mode,5);pinMode(CONTROL_SERVO_6,INPUT_PULLUP);}
                                else if (valeur==1){bitSet(analog_servo,5);bitClear(move_mode,5);}
                                else{bitSet(analog_servo,5);bitClear(move_mode,5);}
                                EEPROM.write(30,analog_servo);
                                EEPROM.write(36,analog_servo);
                                Serial.println(succes);
                                pinMode(CONTROL_SERVO_6,INPUT);}

        else if  (label=="DELAY"){EEPROM.write(32,(byte)(donnee.toInt()));}
        else if  (label=="VOLTS"){EEPROM.write(34,(byte)(donnee.toInt()));}

        else{Serial.println(F("Le label utilisé n'est pas reconnu."));
             Serial.println(F("La synthaxe est: S suivit du numéro du servo suivit du paramètre à ajuster"));
             Serial.println(F("R -Position Repos(0-->360), F -Position Final(0->360), A -Accélération(0->360), P -Entrée analogique(0 ou 1)"));
             Serial.println(F("Exemple S3A=160"));}
             Serial.println(F("Réglage en seconde du délais de mise en veille: DELAY=10 "));
             Serial.println(F("Réglage en décivolts du délais de mise en veille (6,2 volts: VOLTS=62 "));
             
       Serial.println();
       read_eeprom();
         statut_message=0;
         message="";


}
