void read_eeprom(){
  
      analog_servo=EEPROM.read (30);
      move_mode=EEPROM.read (36);
      delay_standby=(EEPROM.read(32))*1000;
      battery_limit=(float)(EEPROM.read(34))/10;
      if (battery_limit>8){battery_limit=8;}
      if (delay_standby<10){delay_standby=10;}


    for (int i=0;i<6;i++){ 
         
          Serial.print(F("servo "));Serial.print(i+1);
          for(int y=0;y<3;y++){
            servos[i][y]=EEPROM.read (i*5+y);
  
            if (y==0){Serial.print(F(" Angle (F)inal: "));Serial.print(int((float(servos[i][y])*360)/250));}
            if (y==1){Serial.print(F(" Angle (R)epos: "));Serial.print(int((float(servos[i][y])*360)/250));}
            if (y==2){Serial.print(F(" (A)ccélération: "));Serial.print(int((float(servos[i][y])*360)/250));}
          
          }
          byte mode_info=0;
          if(bitRead(move_mode,i)){bitSet(mode_info,1);}
          if(bitRead(analog_servo,i)){bitSet(mode_info,0);}
          Serial.print(F(" (M)ode: "));Serial.println(mode_info);
    }
    Serial.print(F(" DELAY de mise en veille: "));Serial.println(delay_standby/1000);
    Serial.print(F(" VOLTS Batterie déchargée: "));Serial.println(battery_limit);
}
