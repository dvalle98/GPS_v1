#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void BarraEstados(bool signal, bool ubicacion, bool estado) {

  if (signal) {
    display.drawBitmap(2, 3, SIGNAL_bmp, SIGNAL_WIDTH, SIGNAL_HEIGHT, 1);
  } else {
    display.drawBitmap(2, 3, NOSIGNAL_bmp, NOSIGNAL_WIDTH, NOSIGNAL_HEIGHT, 1);
  }

  if (ubicacion) {
    display.drawBitmap(105, 3, UBICACION_bmp, UBICACION_WIDTH, UBICACION_HEIGHT, 1);
  } else {
    display.drawBitmap(105, 3, SINUBICACION_bmp, SINUBICACION_WIDTH, SINUBICACION_HEIGHT, 1);
  }

  if (estado) {
    display.setTextSize(1, 2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(45, 5);
    display.println(F("GPS OK!"));
  } else {
    display.setTextSize(1, 2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(35, 5);
    display.println(F("NO SIGNAL!"));
  }
  display.display();
}

void informacionPantalla(bool datosEnviados, String satelites, String Latitud, String Longitud) {
  display.clearDisplay();

  display.drawRect(0, 0, 128, 22, SSD1306_WHITE);  // (x,y,ancho,largo) Estados
  BarraEstados(true, true, true);

  display.drawRect(0, 25, 128, 39, SSD1306_WHITE);  // (x,y,ancho,largo) Contenedor

  if (datosEnviados) {
    display.setTextSize(1, 1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(3, 27);
    display.println(F("Datos enviados!"));
  } else {
    display.setTextSize(1, 1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(3, 27);
    display.println(F("NO se envio datos!"));
  }

  display.setTextSize(1, 1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 37);
  display.println("SAT:" + satelites);

  display.setTextSize(1, 1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(46, 37);
  display.println(F("Serial:85fWe3"));

  display.setTextSize(1, 1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 47);
  display.println("LAT:" + Latitud);

  display.setTextSize(1, 1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3, 55);
  display.println("LONG:" + Longitud);


  display.display();
}