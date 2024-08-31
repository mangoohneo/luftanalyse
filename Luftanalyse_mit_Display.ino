#include <Adafruit_SH110X.h>
#include <splash.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME680.h>
#include <bme68x.h>
#include <bme68x_defs.h>



#define SEALEVELPRESSURE_HPA (1013.25)

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1

int button = 32;
int current_millis = 0;
int start_millis = 0;
int interval = 5000; //5s of screen activation
bool button_pressed = false;

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


Adafruit_BME680 bme; // I2C


void setup() {
  Serial.begin(9600);
  delay(250); //display power up
  display.begin(i2c_Address, true);

  display.display();
  delay(2000);

  display.clearDisplay();


    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(29, 28);
    display.print("Display Test...");
    delay(5000);
    display.display();
    display.clearDisplay();
    display.display();



  while (!Serial);
  Serial.println(F("BME680 async test"));

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  pinMode(button, INPUT);

}

void loop() {
  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  Serial.print(F("Reading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.println(endTime);




//Display plotting values everytime "button" is pressed for 5s (defined in variable "interval")
  

    refreshDisplay();
    display.display();


/*current_millis = millis();

 if(button_pressed && current_millis - start_millis >= interval) //
 {
    refreshDisplay();
    display.display();
 }

 else
 {
    button_pressed = false;
    display.setContrast(0);
    display.clearDisplay();
    display.display();
 }*/

  
  

  

  
  // There's no need to delay() until millis() >= endTime: bme.endReading()
  // takes care of that. It's okay for parallel work to take longer than
  // BME680's measurement time.



  // Obtain measurement results from BME680. Note that this operation isn't
  // instantaneous even if milli() >= endTime due to I2C/SPI latency.
  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(bme.temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(bme.pressure / 100.0);
  Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(bme.humidity);
  Serial.println(F(" %"));

  Serial.print(F("Gas = "));
  Serial.print(bme.gas_resistance);
  Serial.println(F(" KOhms"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(F(" m"));

  Serial.println();
  delay(1000);
}

void refreshDisplay()
{
  display.clearDisplay();
  display.setCursor(1, 0);
  display.print("Temperature: ");
  display.print(bme.temperature);
  display.print(" C");

  display.setCursor(1, 14);
  display.print("Humidity: ");
  display.print(bme.humidity);
  display.print(" %");

  display.setCursor(1, 28);
  display.print("Alt. AMSL: ");
  display.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  display.println(" m");

  display.setCursor(1, 42);
  display.print("Pressure: ");
  display.print(bme.pressure / 100.0);
  display.println(" hPa");

if(millis()>= 30000)
  {
    display.setCursor(1, 56);
    display.print("Gas: ");
    display.print(bme.gas_resistance* 100.00 /50000.00 );
    display.println(" % Quality");
  }


else
  {
    display.setCursor(1, 56);
    display.print("Gas: ");
    display.print("calibrating...");
  }

  display.display();
}
