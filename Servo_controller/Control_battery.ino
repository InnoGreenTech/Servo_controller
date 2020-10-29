void control_battery(){

  v_battery= R_SCALE*(float(map(analogRead(V_BATTERY),0,1024,0,500)))/100;

  if (v_battery<battery_limit){
    Serial.print("Batterie faible mise en veille: ");Serial.println(v_battery);
    digitalWrite(POWER_SERVOS,0);
    PORTB=0;
    PORTD=0; 
    delay(5);
    
    PCICR = 0;          //deasable input interrupt


    SMCR=_BV(SM1);        //Powerdown mode
    bitSet(SMCR,SE);
    sei();
    asm("SLEEP");
  }
}
