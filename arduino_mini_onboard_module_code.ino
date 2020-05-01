//MOUNTED ON DRONE FOR AVOIDANCE
int durationPing;                 
int inchesdistancePing;                 
const int PingSens = 7;
#include <VirtualWire.h>
void setup() {
 vw_set_ptt_inverted(true); // Required for DR3100
 vw_setup(2000);	 // Bits per sec
 vw_set_tx_pin(8);
}

void loop() {
   pinMode(PingSens, OUTPUT);	                 
   digitalWrite(PingSens, LOW);	                 
   delayMicroseconds(2);
   digitalWrite(PingSens, HIGH);                      
   delayMicroseconds(2);                               
   digitalWrite(PingSens, LOW);                       
   pinMode(PingSens, INPUT);	                         
   int durationPing = pulseIn(PingSens, HIGH);    
 
   inchesdistancePing = durationPing / 74 / 2;
   if(inchesdistancePing < 26)
 {
  const char *c = "1";
  vw_send((uint8_t *)c, 1);
 }
 if(inchesdistancePing >= 26)
 {
   const char *c = "3";
   vw_send((uint8_t *)c, 1);
 }
}