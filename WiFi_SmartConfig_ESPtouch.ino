#include "WiFi.h"

//Local
#include "SmartConfigWiFi.h"

// Botón Reset WiFi
const uint8_t RstWF = 4; 

// Bandera (global volátil) para interrupción
volatile bool flagWF = 0;


// Interrupción Borrar Credenciales
void IRAM_ATTR BorrarCredenciales(){
  noInterrupts();
  Serial.println("Reestableciendo credenciales WiFi...");
    writeStringToFlash("", 0); // Reset the SSID
    writeStringToFlash("", 40); // Reset the Password
  Serial.println("Credenciales borradas");
  flagWF = 1;
}

//*******************************************************************************SETUP
void setup() {
  pinMode(2, OUTPUT); // EJEMPLO
  pinMode(WFready, OUTPUT);
  pinMode(RstWF, INPUT_PULLUP);

  Serial.begin(115200);

  // Llamado a función conectar Wi-Fi
  initWiFi();

  // Configuración de Interrupción, al soltar botón
  attachInterrupt(digitalPinToInterrupt(RstWF), BorrarCredenciales, RISING);
}

void loop() {
  
  // --------CÓDIGO LOOP EJEMPLO---------
  Serial.println("---ENTRAMOS EN EL LOOP---");
  digitalWrite(2, HIGH);
  delay(800);
  digitalWrite(2, LOW);
  delay(800);
  // -----------------------------------

  // Reiniciar sistema si se reestablecen las credenciales WiFi
  if(flagWF==1){
    flagWF = 0;
    Serial.println("REINICIO DE SISTEMA");
    ESP.restart();
  }

  // En caso de perder conexión WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Cónexión WiFi perdida!");
    Reconnect();
  }

}
