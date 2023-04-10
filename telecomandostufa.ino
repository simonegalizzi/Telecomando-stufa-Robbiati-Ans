
 // Uncomment the following line to enable serial debug output
//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif 

#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include <SPI.h>
#include <Wire.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#include "SinricPro.h"
#include "SinricProLight.h"

#define WIFI_SSID         "XXX"    
#define WIFI_PASS         "XXX"
#define APP_KEY           ""      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        ""   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define LIGHT_ID          ""    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define BAUD_RATE         115200                // Change baudrate to your need

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

// define array of supported color temperatures
int colorTemperatureArray[] = {2200, 2700, 4000, 5500, 7000};  
int max_color_temperatures = sizeof(colorTemperatureArray) / sizeof(colorTemperatureArray[0]); // calculates how many elements are stored in colorTemperature array
// tempi segnale stufa
int lungo3= 400;
int lungo2= 2130;
int lungo1= 847;
int lungo= 1680;
int mini= 320;
int ini= 330;
int SHORTUNO= 406;
int timez= 416;
int hola= 444;
int SHORT_SPACE= 847;
int LONG_SIG_DIST= 8790;
int BETW =116360;
int sec =117460;
int terz= 123970;
int TREUNO= 1240;
int CICCIO= 1230;
int SEND_PIN= 2;
int DUEUNO= 826;
int TREZERO= 2520;
int PAUSA= 416;
int PAUSA1= 4260;
int PAUSA2= 2900;
int PAUSA4= 831;
int PAUSA5= 1350;
int PAUSA6= 1670;
int PAUSA7= 937;
int PAUSA8= 8000;
int PAUSA9 =430;
int PAUSA10= 2120;
int PAUSA11= 4750;
int PAUSA12= 3900;
int PAUSA13= 1700;
int PAUSA14= 1270;
int PAUSA15= 955;
int PAUSA16= 841;
int PAUSA17=424;
int uno= 416;
int PAUSA18= 2220;
int PAUSA19= 1800;
int PAUSA20= 861;
int PAUSA21= 2640;
int PAUSA22= 526;
int PAUSA23= 370;

bool lamp=true;



String ip;
String INIZIO =   "IZFZFZFZFGFZFZFZFZFLPZEZEZKZEQEJEXF%EYEJEVPZPZFZP";
String INIZIO2 =  "IZFZFZFZFGFZFZFZFZFLPZEZEZKZEQEJEXF%EYE6E[2]2Z$";
String SPEGNI =   "IZFZFZFZFGFZFZFZFZFLPZEZEZKZPJEJEZE[E%EYEJEVEZMZEZP";
String SPEGNI2 =  "IZFZFZFZFGFZFZFZFZFLPZEZEZKZPJEJEZE[E%EYE6E[EZMZE";
String POT2 =     "IZFZFZFZFGFZFZFZFZFLPZEZEZKZ1XEXE%ERP[8*M*E";
String POT21 =    "IZFZFZFZFGFZFZFZFZFLPZEZEZKZ1XEXE%EZE998VFZ1*P";
String POT22 =    "IZFZFZFZFGFZFZFZFZFLPZEZEZKZ1XEXE%E*EYEVE?ECE";
String POT3 =      "IZFZFZFZFGFZFZFZFZFLPZEZEZKZE*E6EXE%E%2*26E";
String POT31 =     "IZFZFZFZFGFZFZFZFZFLPZEZEZKZE*E6EXE%E%2[2*2*E";
String POT32 =     "IZFZFZFZFGFZFZFZFZFLPZEZEZKZE*E6EXE%EZE?EVKZP*P";
String POT33 =     "IZFZFZFZFGFZFZFZFZFLPZEZEZKZE*E6EXE%E6E6EV2YE@2";

String PRIMO =    "HA@F@F@F@FT2C$*$*$92R$9$*$?$VEJE?FVSR&V&C&N&V&Z&?&X&0EC1C2XE?&J&V&YEC&32CPCE?M4MG&Z&5&YECE52C&6&ZEV&CMJ&5&J&Z&Z&Z&V7J8Z898?8*8V8?8*8V8?8*8V8?8*8V8?8*8V8?8*8V8?8*8V8?8*8V8?8*8V8?&J89M4802Z2*F";
String SECONDO =  "HA@F@F@F@FT2C$*$*$92R$9$*$5$X$*$5$X2R$9$@$N$9$@$?$X$0$@1@2X$?$*$V$Y$@$32C2Z$=M4MG$C$+$Y$@$+2@$6$Z$V$@M*$+$*$@$@$@$V7*8C898?$*$9$?$*$V$?$*$V$?$*$V$?$*$V$?$*$V$?$*$V$?$*$V$?$*$V$?$*E9M4$02Z2*$";



void setupWiFi() {
  u8x8.begin();
  u8x8.setPowerSave(0);

  pinMode(SEND_PIN, OUTPUT);
  digitalWrite(SEND_PIN, LOW);
  
  u8x8.clear();
  u8x8.setFont(u8x8_font_victoriabold8_r);
  u8x8.drawString(0, 0, "CONNESSIONE");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    if (lamp){
      u8x8.setFont(u8x8_font_profont29_2x3_r);
      u8x8.drawString(0, 4, "....");
    }else{
      u8x8.setFont(u8x8_font_profont29_2x3_r);
      u8x8.drawString(0, 4, "    ");
    }
    delay(100);
    !lamp;
  }
  u8x8.clear();
  IPAddress localIP = WiFi.localIP();
  ip.concat((String)localIP[0]);
  ip.concat(".");
  ip.concat((String)localIP[1]);
  ip.concat(".");
  ip.concat((String)localIP[2]);
  ip.concat(".");
  ip.concat((String)localIP[3]);
  const char *indirizzo = ip.c_str();
  //strcpy(indirizzo,ip);
  //indirizzo.concat((char)localIP[1]);
  //indirizzo.concat('.');
  //indirizzo.concat((char)localIP[2]);
  //indirizzo.concat('.');
  //indirizzo.concat((char)localIP[3]);
  u8x8.setFont(u8x8_font_victoriabold8_r);
  u8x8.drawString(0, 0, indirizzo);
  //Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

void sendSig(String sig) {

    for (int i = 0; i < sig.length(); i++) {
      switch (sig[i]) {
        case '$':  
          
          sigPuls(uno);
         
          break;
        case '8': 
        
          sigPuls(lungo3);
         
          break;
        case '7':  
          
          sigPuls(lungo2);
         
          break;
        case '2': 
        
          sigPuls(lungo1);
          
          break;
        case '1': 
         
          sigPuls(lungo);
        
          break;
        case '&': 
         
          sigPuls(hola);
         
          break;
        case 'A':  
         
         sigPuls(mini);
          break;
        case 'I': 
        
          sigPuls(ini);
         
          break;
        case 'E':  
         
       sigPuls(timez);
          break;
         case 'F': 
         
          sigPuls(SHORTUNO);
         
          break;
        case 'K':  
         
        sigPuls(CICCIO);
         
          break;
        case 'L':  
          delayMicroseconds(LONG_SIG_DIST);
          break;
        case 'G':  
          delayMicroseconds(SHORT_SPACE);
          break;
        case 'P':  
         
          sigPuls(DUEUNO);
         
          break;
        case 'N':  
          delayMicroseconds(TREZERO);
          break;
        case 'M': 
         
          sigPuls(TREUNO);
         
          break;
        case 'Z':  
          delayMicroseconds(PAUSA);
          break;
        case 'Q':  
          delayMicroseconds(TREUNO);
          break;
        case '%':  
          delayMicroseconds(PAUSA1);
          break;
        case 'R':  
          delayMicroseconds(PAUSA2);
          break;
        case 'J':  
          delayMicroseconds(PAUSA4);
          break;
        case 'X': 
          delayMicroseconds(PAUSA5);
          break;
         case 'Y':  
          delayMicroseconds(PAUSA6);
          break;
        case 'V':  
          delayMicroseconds(PAUSA7);
          break;
        case 'T': 
          delayMicroseconds(PAUSA8);
          break;
        case 'C':  
          delayMicroseconds(PAUSA9);
          break;
        case 'H':  
          delayMicroseconds(BETW);
          break;
        case '?':  
          delayMicroseconds(PAUSA10);
          break;
        case '0':  
          delayMicroseconds(PAUSA11);
          break;
        case '3':  
          delayMicroseconds(lungo);
          break;
         case '4':  
          delayMicroseconds(PAUSA12);
          break;
        case '5':  
          delayMicroseconds(PAUSA13);
          break;
        case '6':  
          delayMicroseconds(PAUSA14);
          break;
        case '9':  
          delayMicroseconds(PAUSA15);
          break;
        case '*':  
          delayMicroseconds(PAUSA16);
          break;
         case '@': 
          delayMicroseconds(PAUSA17);
          break;
        case '=': 
          delayMicroseconds(PAUSA18);
          break;
       case '+':  
          delayMicroseconds(PAUSA19);
          break;
        case '#':  
          delayMicroseconds(PAUSA20);
          break;
        case '/':  
          delayMicroseconds(PAUSA21);
          break;
         case '(':  
          delayMicroseconds(sec);
          break;
         case ')':  
          delayMicroseconds(terz);
          break;
        case '[':  
          delayMicroseconds(PAUSA22);
          break;
         case ']': 
          delayMicroseconds(PAUSA23);
          break;
        default:
          Serial.println("Error");
          break;
        
     
    }
    //   Serial.println(sig);
  }
}

void sigPuls(int duration) {
  digitalWrite(SEND_PIN, HIGH);
  delayMicroseconds(duration);
  digitalWrite(SEND_PIN, LOW);
}
void fuoco(){
    sendSig(INIZIO);
    sendSig(PRIMO);
    delay(123);
    sendSig(INIZIO2);
    sendSig(SECONDO);
    delay(123);
    sendSig(INIZIO2);
    sendSig(SECONDO);
    delay(1000);
    sendSig(INIZIO);
    sendSig(PRIMO);
    delay(123);
    sendSig(INIZIO2);
    sendSig(SECONDO);
    delay(123);
    sendSig(INIZIO2);
    sendSig(SECONDO);
}

void smorza(){
    sendSig(SPEGNI);
   
    delay(415);
    sendSig(SPEGNI2);
   
    delay(415);
    sendSig(SPEGNI2);
   
    delay(415);
    sendSig(SPEGNI2);
   
    delay(415);
    sendSig(SPEGNI2);
   
    delay(415);
 
    sendSig(SPEGNI2);
}

bool onPowerState(const String &deviceId, bool &state) {
  
  //Serial.printf("%s \r\n", state?"ac":"sp");
 // if (state){
   u8x8.setFont(u8x8_font_profont29_2x3_r);
   u8x8.drawString(0, 2, "STO");
   u8x8.drawString(0, 5, "INVIANDO");
   fuoco();
   u8x8.clear();
 /* }else{
   u8x8.setFont(u8x8_font_profont29_2x3_r);
   u8x8.drawString(0, 2, "STO");
   u8x8.drawString(0, 5, "INVIANDO");
   smorza();
   u8x8.clear(); 
  }*/
  u8x8.setFont(u8x8_font_victoriabold8_r);
  const char *indirizzo = ip.c_str();
  u8x8.drawString(0, 0, indirizzo);
  u8x8.setFont(u8x8_font_profont29_2x3_r);
  u8x8.drawString(0, 2, "SERVER");
  u8x8.drawString(0, 5, "ON");
  //Serial.printf("Device %s power turned %s \r\n", deviceId.c_str(), state?"on":"off");
  //device_state.powerState = state;
  return true; // request handled properly
}

void setupSinricPro() {
  // get a new Light device from SinricPro
  SinricProLight &myLight = SinricPro[LIGHT_ID];

  // set callback function to device
  myLight.onPowerState(onPowerState);


  // setup SinricPro
  SinricPro.onConnected([](){ u8x8.setFont(u8x8_font_profont29_2x3_r);
                              u8x8.drawString(0, 2, "SERVER");
                              u8x8.drawString(0, 5, "ON"); }); 
  SinricPro.onDisconnected([](){ u8x8.drawString(0, 2, "SERVER");
                                 u8x8.drawString(0, 5, "OFF"); }); 
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  //Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  //setupColorTemperatureIndex(); // setup our helper map
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
