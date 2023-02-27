#include "SIM800L.h"
#define SIM800_RST_PIN 15
const char APN[] = "internet.movistar.com.co";
String URL = "";
SIM800L* sim800l;
int tamano = 0;


static const uint32_t GPRSBaud = 9600;
bool ConexionGPRS = false;
bool DatosEnviadosGPRS = false;

void setupModule() {
  // Wait until the module is ready to accept AT commands
  while (!sim800l->isReady()) {
    Serial.println(F("Problem to initialize AT command, retry in 1 sec"));
    delay(1000);
  }
  Serial.println(F("Setup Complete!"));

  // Wait for the GSM signal
  uint8_t signal = sim800l->getSignal();
  while (signal <= 0) {
    delay(1000);
    signal = sim800l->getSignal();
  }
  Serial.print(F("Signal OK (strenght: "));
  Serial.print(signal);
  Serial.println(F(")"));
  delay(1000);

  // Wait for operator network registration (national or roaming network)
  NetworkRegistration network = sim800l->getRegistrationStatus();
  while (network != REGISTERED_HOME && network != REGISTERED_ROAMING) {
    delay(1000);
    network = sim800l->getRegistrationStatus();
  }
  Serial.println(F("Network registration OK"));
  delay(1000);

  // Setup APN for GPRS configuration
  bool success = sim800l->setupGPRS(APN);
  while (!success) {
    success = sim800l->setupGPRS(APN);
    delay(5000);
  }
  Serial.println(F("GPRS config OK"));

  // Establish GPRS connectivity (5 trials)
  bool connected = false;
  for (uint8_t i = 0; i < 5 && !connected; i++) {
    delay(1000);
    connected = sim800l->connectGPRS();
  }

  // Check if connected, if not reset the module and setup the config again
  if (connected) {
    Serial.print(F("GPRS connected with IP "));
    Serial.println(sim800l->getIP());
  } else {
    Serial.println(F("GPRS not connected !"));
    Serial.println(F("Reset the module."));
    sim800l->reset();
    setupModule();
    return;
  }
  ConexionGPRS = true;
}


bool EnviarDatosGPRS(String Lat, String Long) {

  Serial.println("Enviara datos...");
  Serial.println(F("Start HTTP GET..."));

  URL = "http://gps.controlacademic.co/apicloudsignaltest/modulotransporte/Transporte/GuardarPuntosGPS?latitude="+Lat+"&longitude="+Long+"&id=UUW726&speed=1";  
  tamano = URL.length()+1;
  char charArrayURL[tamano];
  URL.toCharArray(charArrayURL, tamano);
  Serial.println(charArrayURL);

  // Do HTTP GET communication with 10s for the timeout (read)
  uint16_t rc = sim800l->doGet(charArrayURL, 5000);
  if (rc == 200) {
    // Success, output the data received on the serial
    Serial.print(F("HTTP GET successful ("));
    Serial.print(sim800l->getDataSizeReceived());
    Serial.println(F(" bytes)"));
    Serial.print(F("Received : "));
    Serial.println(sim800l->getDataReceived());
    DatosEnviadosGPRS = true;
  } else {
    // Failed...
    Serial.print(F("HTTP GET error "));
    Serial.println(rc);
    DatosEnviadosGPRS = false;
    setupModule();
  }
  /*
  // Close GPRS connectivity (5 trials)
  bool disconnected = sim800l->disconnectGPRS();
  for(uint8_t i = 0; i < 5 && !connected; i++) {
    delay(1000);
    disconnected = sim800l->disconnectGPRS();
  }
  
  if(disconnected) {
    Serial.println(F("GPRS disconnected !"));
  } else {
    Serial.println(F("GPRS still connected !"));
  }

  // Go into low power mode
  bool lowPowerMode = sim800l->setPowerMode(MINIMUM);
  if(lowPowerMode) {
    Serial.println(F("Module in low power mode"));
  } else {
    Serial.println(F("Failed to switch module to low power mode"));
  }*/

  Serial.println("Termino de enviar datos");
  delay(5000);


  return DatosEnviadosGPRS;
}
