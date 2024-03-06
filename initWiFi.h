#include "EEPROM.h"

// LED de estado Wi-Fi
const uint8_t WFready = 22;

//Set length of char string
#define LENGTH(x) (strlen(x) + 1)
#define EEPROM_SIZE 200

//Variable to store ssid and password
String ssid;
String pss;

//***************************************************************Write to EEPROM
void writeStringToFlash(const char* toStore, int startAddr) {
  int i = 0;
  for (; i < LENGTH(toStore); i++) {
    EEPROM.write(startAddr + i, toStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  EEPROM.commit();
}

//**************************************************************Read from EEPROM
String readStringFromFlash(int startAddr) {
  char in[128];
  int i = 0;
  for (; i < 128; i++) {
    in[i] = EEPROM.read(startAddr + i);
  }
  return String(in);
}
//***************************************************************initWiFi
void initWiFi() {
  if (!EEPROM.begin(EEPROM_SIZE)) { //Init EEPROM
    Serial.println("failed to init EEPROM");
    delay(1000);
  } else {
    //Read SSID stored at address 0
    ssid = readStringFromFlash(0);
    Serial.print("SSID = ");
    Serial.println(ssid);
    //Read Password stored at address 40
    pss = readStringFromFlash(40);
    Serial.print("PASS = ");
    Serial.println(pss);
  }
  //Start WiFi Connection
  WiFi.begin(ssid.c_str(), pss.c_str());
  delay(3000);
  
  //If WiFi is not connected
  if (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(WFready, LOW);
    //Init WiFi as Station, start SmartConfig
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();
 
    //Wait for SmartConfig packet from mobile (ESP TOUCH App)
    Serial.print("Waiting for EspTouch SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(1000);
      Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("SmartConfig received");
    
    //Wait for WiFi to connect to AP
    Serial.print("Waiting for WiFi.");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");      
    }
    
    Serial.println("WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    //Read the connected WiFi SSID and password
    ssid = WiFi.SSID();
    pss = WiFi.psk();
    
    Serial.print("SSID:");
    Serial.println(ssid);
    Serial.print("PASS:");
    Serial.println(pss);
    Serial.println("Storing SSID & PASSWORD in EEPROM");
    
    //Store the ssid at address 0
    writeStringToFlash(ssid.c_str(), 0);
    //Store the password at address 40
    writeStringToFlash(pss.c_str(), 40);
   
    Serial.println("Connection OK");
    //delay(3000);
    //ESP.restart();//For correct connection proccess
  }
  
  else
  {Serial.println("WiFi Connected");
  digitalWrite(WFready, HIGH);
  }
}