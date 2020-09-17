#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>


const char *ssid     = "HAMMER INNOVATION LAB";
const char *password = "curiosityisgood";
const long utcOffsetInSeconds = 28800;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


#define LEDPIN            5
#define NUMPIXELS      60
Adafruit_NeoPixel clockpixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_RGB + NEO_KHZ800);



int BuzzerPin = 4;     // Connect Buzzer to Arduino pin 11
int Si2 =1975;
int LaS2=1864;
int La2= 1760;
int SolS2=1661;
int Sol2=1567;
int FaS2=1479;
int Fa2= 1396;
int Mi2= 1318;
int ReS2=1244;
int Re2= 1174;
int DoS2=1108;
int Do2= 1046;
// Low Octave
int Si = 987;
int LaS= 932;
int La = 880;
int SolS=830;
int Sol= 783;
int FaS= 739;
int Fa=  698;
int Mi=  659;
int ReS= 622;
int Re = 587;
int DoS =554;
int Do = 523;
// define the notes
int rounda=0;
int roundp=0;
int white= 0;
int whitep=0;
int black=0;  
int blackp=0;
int quaver=0;
int quaverp =0;
int semiquaver=0;
int semiquaverp=0;
int bpm= 120;


int hcsr501pin = D7;
int hcsr501val;
int humanstat=0;


void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

  clockpixels.begin();

      pinMode(BuzzerPin,OUTPUT);
      black= 35000/bpm; 
      blackp=black*1.5;
      white= black*2;
      whitep=white*1.5;
      rounda= black*4;
      roundp= rounda*1.5;
      quaver= black/2;
      quaverp=quaver*1.5;
      semiquaver= black/4;
      semiquaverp=semiquaver*1.5;
  
}

void loop() {
  if(humanstat>=20){
    stanbyclock();
  }else{
    timeClient.update();
  
    Serial.print(daysOfTheWeek[timeClient.getDay()]);
    Serial.print(", ");
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.print(":");
    Serial.println(timeClient.getSeconds());
    //Serial.println(timeClient.getFormattedTime());
  
    baseclock();
  }
}

void baseclock(){
  int hour=0;
  for(int i=0 ; i<NUMPIXELS ; i++){
    if(i%5==0){
      clockpixels.setPixelColor(i, clockpixels.Color(0,0,2));
    }else{
      clockpixels.setPixelColor(i, clockpixels.Color(0,0,0));
    }

    if( (i%5==0 && hour==(timeClient.getHours()%12)) || (i==0 && (timeClient.getHours()%12)==12 ) ){
      clockpixels.setPixelColor(i, clockpixels.Color(0,2,2));
    }

    if(i==timeClient.getMinutes()){
      clockpixels.setPixelColor(i, clockpixels.Color(0,2,0));
    }

    if(i%5==0){
      hour++;
    }
  }

  clockpixels.show();
    if(timeClient.getMinutes()==0){
       alarm();
    }else{
       delay(5000);
    }

    if(checkhuman()){
      humanstat=0;
    }else{
      humanstat++;
    }
}

void stanbyclock(){
  for(int i=0 ; i<NUMPIXELS ; i++){
      clockpixels.setPixelColor(i, clockpixels.Color(0,0,0));
  }
  clockpixels.show();
}

void alarmanimation(int bright){
  if((timeClient.getHours()%12)==0){
     clockpixels.setPixelColor(0, clockpixels.Color(0,0,bright));
  }else{
    clockpixels.setPixelColor(((timeClient.getHours()%12)*5), clockpixels.Color(0,0,bright));
  }
  clockpixels.show();
}


void alarm(){
  stanbyclock();
  for(int j=0 ; j<20 ; j++){
      tone(BuzzerPin,Mi,black);
      alarmanimation(30);
      delay(1000);
      tone(BuzzerPin,La,black);
      alarmanimation(60);
      delay(1000);
      tone(BuzzerPin,Si,black);
      alarmanimation(90);
      delay(1000);
  }
}


boolean checkhuman(){
     hcsr501val = digitalRead(hcsr501pin);
    if (hcsr501val == LOW)
    {
      return false;
    }
    else
    {
      return true;
    }
}

