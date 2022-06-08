void getTimeStamp(){
  updateTime();
  timeStampForMemoryCard();
}
void timeStampForMemoryCard(){
  timeStampMemoryCard = String(Year) + ":";
  timeStampMemoryCard = timeStampMemoryCard + String(Month)  + ":" ;
  timeStampMemoryCard = timeStampMemoryCard + String(Date)   + ":" ;
  timeStampMemoryCard = timeStampMemoryCard + String(Hour)   + ":" ;
  timeStampMemoryCard = timeStampMemoryCard + String(Minute) + ":" ;
  timeStampMemoryCard = timeStampMemoryCard + String(Second)+ ":" ;
  timeStampMemoryCard = timeStampMemoryCard + String(MilliSecond);
}

void timeStampForTelemetry(){
  
}

void updateTime(){
  
  
  Year   = (int) Clock.getYear();
  Month  = (int) Clock.getMonth(century);
  Date   = (int) Clock.getDate();
  Hour   = (int) Clock.getHour(h12Flag, pmFlag);
  Minute = (int) Clock.getMinute();
  Second = (int) Clock.getSecond();
  MilliSecond = (int) millisTime();

  if (MilliSecond<0){
    Second = Second-1;
    MilliSecond = 1000+MilliSecond;
    if (Second < 0){
      Minute = Minute-1;
      Second = 59;
       if (Minute<0){
        Hour = Hour-1;
        Minute = 59;
       }
    }
  }
}

long millisTime(){
  long t;
  t = millis() - (Hour*3600 + Minute*60 + Second - initTime)*1000  - initMillis;
  return t;
}
void updateInitTime(){ 
  Hour   = (int) Clock.getHour(h12Flag, pmFlag);
  Minute = (int) Clock.getMinute();
  Second = (int) Clock.getSecond();
  initTime =  Hour*3600 + Minute*60 + Second;
  initMillis = millis();
}
