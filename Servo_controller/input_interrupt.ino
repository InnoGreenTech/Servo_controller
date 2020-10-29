/* Input interupt to wake up the processor */

ISR(PCINT1_vect){
  
  if (bitRead(SMCR,SE)){                   // desable standby mode
    Serial.println("Wakeup!");
    SMCR=0;
    TCNT1 = 0;                            // clear the timer count
    TIMSK1 =  _BV(OCIE1A)|_BV(OCIE1B);   // enable the output compare interrupt

  }
  start_delay_standby=millis();
  delay_enable_servos=millis();
  
}

/* go in standby mode */

void active_sleep_mode(){
  Serial.println("Sleep");
  delay(2);
  digitalWrite(POWER_SERVOS,0);  

  SMCR=_BV(SM1);        //Powerdown mode
  bitSet(SMCR,SE);
  sei();
  asm("SLEEP");
}
