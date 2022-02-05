//#include "WiFi.h"
#include <U8x8lib.h>

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

unsigned long ct; // current time
unsigned long nt; // next time
int ctr = 0;
char buf[3];
char buf2[300];

const int PttPin = 4;

#define NUMSAMP (60*6*2)
int sample[NUMSAMP];
#define NEXTTIME 500

int sampleTot = 0;
int nextSamp = 0;
int samp;

float fract = 0.0;
int fractI = 0;

void setup()
{
  // Set up display
  u8x8.begin();
  //u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFont(u8x8_font_inb46_4x8_r);
  
  nt = millis() + NEXTTIME;
  Serial.begin(115200);

  pinMode(PttPin, INPUT);
  for (int i=0; i<NUMSAMP; i++)
     sample[i]=0;
  
}

void loop()
{
  ct = millis();
  if (ct < nt) return;
  nt = nt + NEXTTIME;

  if (digitalRead(PttPin) == HIGH) //Tx active
     samp = 1;
  else
     samp = 0;
  sampleTot = sampleTot - sample[nextSamp] + samp;
  sample[nextSamp] = samp;
  nextSamp++;
  if (nextSamp == NUMSAMP)
     nextSamp=0;

  fract = (((float)sampleTot)*100.0)/((float)NUMSAMP);
  sprintf(buf2, "---sampleTot = %d fract %f NS %d\n", sampleTot, fract, NUMSAMP);
  Serial.write(buf2);
  
  
  //fract = ((float)sampleTot*100.0)/(float)NUMSAMP;
  //sprintf(buf2,"---%d %f %f\n",sampleTot, (float)NUMSAMP, fract);
  //Serial.write(buf2);
  
  
  
  fractI = (int)(fract + 0.5);
  sprintf(buf2,"-- %f %d\n",fract, fractI);
  Serial.write(buf2);
  
  //ctr = ctr + 10;
  //if (ctr>100) ctr = 0;

// u8x8.clearDisplay();
  sprintf(buf, "%3d", fractI);
  u8x8.drawString(0, 0, buf);

  // diag - show how much time this took
  sprintf(buf2,"%d msec %d %d\n",millis()-ct, sampleTot, fractI);
  Serial.write(buf2);
}
