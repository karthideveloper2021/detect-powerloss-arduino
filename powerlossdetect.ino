volatile bool status=true;
#include <EEPROM.h>
#include <RTClib.h> 

RTC_DS3231 rtc;
DateTime now;


void setup() {
  // put your setup code here, to run once:
  TCCR1A = B00000000;
  TCCR1B = B00000100;       //prescalar 252
  TCNT1 = 64911;        //10ms
  TIMSK1 = B00000001;   //enable overflow flag
  Serial.begin(115200);
  pinMode(A0,INPUT);

  
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  if (rtc.lostPower()) {
  Serial.println("RTC lost power, let's set the time!");
  // When time needs to be set on a new device, or after a power loss, the
  // following line sets the RTC to the date & time this sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

}

  Serial.println(EEPROM.read(1));

}


ISR(TIMER1_OVF_vect){
  // Serial.println(millis());
  TCNT1=64911;
  if(analogRead(A0)<910){
    pinMode(A0,OUTPUT);
    digitalWrite(A0,HIGH);
    return;
  }
  pinMode(A0,INPUT);
  if(analogRead(A0)>1000){
    if(status){
      
      // now=rtc.now();
      EEPROM.write(1,now.minute());
      Serial.println("cutoff updated...");
      status=false;
    }
    
  }
  // Serial.println(analogRead(A0));
  
}

void loop() {
  // put your main code here, to run repeatedly:
  now=rtc.now();
  delay(500);
}
