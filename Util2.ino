
#include <U8x8lib.h>

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

TaskHandle_t Task1;
TaskHandle_t Task2;

unsigned long ct; // current time
unsigned long nt; // next time
int ctr = 0;
char buf[3];
char buf2[300];

const int PttPin = 5;

#define FRACTSEC 32
#define NUMSAMP (60*6*FRACTSEC)
int sample[NUMSAMP];
#define NEXTTIME (1000/FRACTSEC)

int sampleTot = 0;
int nextSamp = 0;
int samp;
byte cur_util;

float fract = 0.0;
int fractI = 0;

void setup()
{
  // Set up display
  u8x8.begin();
  u8x8.setFont(u8x8_font_inb46_4x8_r);

//create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
 delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
//  xTaskCreatePinnedToCore(
//                    Task2code,   /* Task function. */
//                    "Task2",     /* name of task. */
//                    10000,       /* Stack size of task */
////                    NULL,        /* parameter of the task */
//                    1,           /* priority of the task */
//                    &Task2,      /* Task handle to keep track of created task */
//                    1);          /* pin task to core 1 */
//  delay(500); 
  
 
  Serial.begin(115200);

  pinMode(PttPin, INPUT_PULLUP);
  for (int i=0; i<NUMSAMP; i++)
     sample[i]=0;

  nt = millis() + NEXTTIME;
}

// Sampling Task
void Task1code( void * pvParameters ){
//  Serial.print("Sampling on core ");
//  Serial.println(xPortGetCoreID());
  for(;;){
//    cur_util++;
    delay (1);
    ct = millis();
    if (ct >= nt){
       nt = nt + NEXTTIME;

     if (digitalRead(PttPin) == LOW) //Tx active
        samp = 1;
     else
       samp = 0;
     sampleTot = sampleTot - sample[nextSamp] + samp;
     sample[nextSamp] = samp;
     nextSamp++;
     if (nextSamp == NUMSAMP)
        nextSamp=0;

     fract = (((float)sampleTot)*100.0)/((float)NUMSAMP);
     cur_util = (byte)(fract + 0.5);
    }
  } 
}

//Not sure if this is needed
//void Task2code( void * pvParameters ){
//  for(;;){
//    Serial.print("Task2 running on core ");
//    Serial.println(xPortGetCoreID());
//    //delay(7000);
//    sprintf(buf, "%3d", (int)cur_util);
//    u8x8.drawString(0, 0, buf);
//  }
//}


void loop()
{
    sprintf(buf, "%3d", (int)cur_util);
    u8x8.drawString(0, 0, buf);
}
