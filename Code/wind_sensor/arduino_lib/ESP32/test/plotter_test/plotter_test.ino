void setup() {
  Serial.begin(9600);

}

int i = 1;           //ith trial
int randNo = 1;      //random integer between 0 or 1 (say, 0 = heads, 1 = tails)
int sumTails = 0;   //add up all occurances of tails and store in sumTails
float relFreq = 0;   //compute and store relative frequency of tails here

void loop() {
  randNo=random(0,2);                    //generate random int between 0 and 1
  sumTails = sumTails + randNo;          //update tails tally
  relFreq = float(sumTails)/float(i);    //update relative frequency

  //report results: ,,
  Serial.print(i);
  Serial.print(',');
  //Serial.print(sumTails);
  //Serial.print(',');
  Serial.print(relFreq);
  Serial.print('\n');
  
  i=i+1;                                 //update ith trial
   
  delay(1000);                           //define update rate

}
