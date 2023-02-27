#include "esp_task_wdt.h"

#define NOSIGNAL_HEIGHT 16
#define NOSIGNAL_WIDTH 16
static const unsigned char PROGMEM NOSIGNAL_bmp[] = { 0x22, 0x07, 0x14, 0x07, 0x08, 0x07, 0x14, 0x77, 0x22, 0x77, 0x00, 0x77, 0x0E, 0x77, 0x0E, 0x77,
                                                      0x0E, 0x77, 0xEE, 0x77, 0xEE, 0x77, 0xEE, 0x77, 0xEE, 0x77, 0xEE, 0x77, 0xEE, 0x77, 0xEE, 0x77 };

#define SIGNAL_HEIGHT 16
#define SIGNAL_WIDTH 16
static const unsigned char PROGMEM SIGNAL_bmp[] = { 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x77, 0x00, 0x77, 0x00, 0x77, 0x00, 0x77,
                                                    0x0E, 0x77, 0x0E, 0x77, 0x0E, 0x77, 0xEE, 0x77, 0xEE, 0x77, 0xEE, 0x77, 0xEE, 0x77, 0xEE, 0x77 };

#define UBICACION_HEIGHT 16
#define UBICACION_WIDTH 16
static const unsigned char PROGMEM UBICACION_bmp[] = { 0x00, 0x00, 0x00, 0x06, 0x00, 0x1E, 0x00, 0xFC, 0x07, 0xFC, 0x1F, 0xF8, 0x7F, 0xF8, 0x0F, 0xF8,
                                                       0x01, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xE0, 0x00, 0x60, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00 };

#define SINUBICACION_HEIGHT 16
#define SINUBICACION_WIDTH 16
static const unsigned char PROGMEM SINUBICACION_bmp[] = { 0x00, 0x00, 0x00, 0x06, 0x00, 0x1E, 0x00, 0xFC, 0x07, 0xFC, 0x1F, 0xF8, 0x7F, 0xF8, 0x0F, 0xF8,
                                                          0x01, 0xF0, 0x00, 0xF0, 0x84, 0xF0, 0x48, 0xE0, 0x30, 0x60, 0x30, 0x40, 0x48, 0x40, 0x84, 0x00 };




#include "FuncionesGPS.H"
#include "FuncionesGPRS.H"
#include "FuncionesPantalla.h"


void setup() {
  esp_task_wdt_init(20, false);  
  Serial.begin(9600);
  Serial2.begin(GPRSBaud);
  Serial1.begin(GPSBaud, SERIAL_8N1, 2, 4); //baud, conf, Rx, Tx

  sim800l = new SIM800L((Stream *)&Serial2, SIM800_RST_PIN, 200, 512);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  Serial.println();
 

  BarraEstados(false, false, false);
  setupModule();


}

void loop() {
esp_task_wdt_reset();
  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  Latitud(gps.location.lat(), gps.location.isValid(), 11, 6);
  Longitud(gps.location.lng(), gps.location.isValid(), 12, 6);
  Serial.println();

   if (gps.location.isValid() ) {
    ConexionGPS = true;
    EnviarDatosGPRS(latitud, longitud);
  } else {
    ConexionGPS = false;
  }

  informacionPantalla(DatosEnviadosGPRS, satelites, latitud, longitud);
  BarraEstados(ConexionGPRS, ConexionGPS, (ConexionGPRS && ConexionGPS));


  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println(F("No GPS data received: check wiring"));
    ConexionGPS = false;
  }



    if(!DatosEnviadosGPRS){
    setupModule();
  }


  
}
