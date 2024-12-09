#include "SR04.h"


#define TRIG_PIN1 16
#define ECHO_PIN1 17
#define TRIG_PIN2 22
#define ECHO_PIN2 23
#define TRIG_PIN3 18
#define ECHO_PIN3 19

//initialize sensors to GPIO pins
SR04 s1 = SR04(ECHO_PIN1, TRIG_PIN1);
SR04 s2 = SR04(ECHO_PIN2, TRIG_PIN2);
SR04 s3 = SR04(ECHO_PIN3, TRIG_PIN3);
//variables for sensor data
long a;
long b;
long c;
//current tracks current sensor readings
long current[3];
//avgPos get average distances of back levels every 20 secs
long avgPos[3];
//running is the running total before 
long running[3];
//count cycles
int loopCnt;
//streak tracks consecutive bad posture intervals
//-> default alerts after 1 min of bad posture (3 cycles)
int streak;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  delay(1000);
  current[0] = 0;
  current[1] = 0;
  current[2] = 0;
  running[0] = 0;
  running[1] = 0;
  running[2] = 0;
  loopCnt = 0;
}

void loop() {
  // read sensor data
  a = s1.Distance();
  b = s2.Distance();
  c = s3.Distance();
  // increment cycle count
  loopCnt += 1;

  // every 20 secs, average all sensor readings (1 read/sec)
  if(loopCnt % 20 == 0){
    avgPos[0] = running[0]/20;
    avgPos[1] = running[1]/20;
    avgPos[2] = running[2]/20;
    //if avg values detect bad posture, increment streak
    //bad posture is misalignment of upper/mid/lower back (add more cases later)
    if((avgPos[2]-avgPos[1])>7 || (avgPos[1] - avgPos[0])>9){
      streak += 1;
    }
    else{
      streak = 0;
    }
    //reset running counts
    running[0] = 0;
    running[1] = 0;
    running[2] = 0;
    Serial.print((String)"Averages: "+ avgPos[0] + " " + avgPos[1] + " " + avgPos[2]);
    Serial.print("\n");
    //if 3 cycles bad posture streak (1 min), send alert
    if(streak == 1){
      Serial.print("ALERT! BAD POSTURE DETECTED!\n");
      Serial.print("\n");
      streak = 0;
    }
  }

  //ignore anomalous large readings
  //update current readings 
  if(a > 100){
    current[0] = current[0];
  }
  else{
    current[0] = a;
  }
  if(b > 100){
    current[1] = current[1];
  }
  else{
    current[1] = b;
  }
  if(c > 100){
    current[2] = current[2];
  }
  else{
    current[2] = c;
  }

  Serial.print((String)"upper: " + current[2]);
  Serial.print("\n");
  running[2] += current[2];
  Serial.print((String)"middle: " + current[1]);
  Serial.print("\n");
  running[1] += current[1];
  Serial.print((String)"lower: " + current[0]);
  Serial.print("\n");
  running[0] += current[0];

  Serial.print("\n");
  delay(1000);

}

/*#include <FB_Const.h>
#include <FB_Error.h>
#include <FB_Network.h>
#include <FB_Utils.h>
#include <Firebase.h>
#include <FirebaseFS.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
const char* ssid= "1";
const char* password= "12345678";
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData firebaseData;
void setup(){
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  config.host= "https://stemup-convergent-24-default-rtdb.firebaseio.com/";
  config.api_key = "AIzaSyD0bc3hndYuWWA5ECv2Hc_TYzPcRvD1PtU";
  auth.user.email = "johnnyshen0630@gmail.com";
  auth.user.password = "103170";
  Firebase.begin(&config, &auth);
  if (Firebase.ready()){
    Serial.println("Connected to Firebase");
  }else{
    Serial.println("Failed to connect to Firebase");
  }
}
void loop(){
  float x= 20;
  Firebase.RTDB.setFloat(&firebaseData, "/sensor/temperature", x);*/

  