#include "declarations.h"
#include "config.h"
#include "menu_icons.h"
#include "init.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, "south-america.pool.ntp.org", -5 * 3600, 60000);

// Menu e items
char menuItems[NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"Linterna"},
    {"Cronometro"},
    {"Salir"}};

const unsigned char *icons_bitmaps[NUM_ITEMS] = {
    image_icon_lintern_bits,
    image_icon_clock_bits,
    image_icon_out_bits};

// Fecha
const char *daysOfTheWeek[7] = {"DOM", "LUN", "MAR", "MIE", "JUE", "VIE", "SAB"};
const char *monthsOfTheYear[12] = {"ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DIC"};

void initWifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS, 6);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectandose a la red Wi-Fi...");
  }
  Serial.println("Conexion exitosa");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void initNtpClient()
{
  ntpClient.begin();
}

void initButtons()
{
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  Serial.println("Botones incializados ...");
}

void initScreen()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  Serial.println("Pantalla funcionando ...");
  display.clearDisplay();
}