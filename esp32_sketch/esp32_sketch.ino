#include <WiFi.h>
#include <PubSubClient.h>     
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <WiFiClientSecure.h>

// Пины
#define LED_PWM         45
#define BMP280_SCL      47
#define BMP280_SDA      48
#define PIN_AIN_1       13
#define PIN_AIN_2       14
#define PIN_AIN_3       21
#define PIN_DIN_2       12
#define PIN_LED_DEBUG   11
#define PIN_BUTT_DEBUG  38


static const uint8_t ca_cert[] PROGMEM = {
 0x30, 0x82, 0x05, 0x09, 0x30, 0x82, 0x02, 0xf1, 0xa0, 0x03, 0x02, 0x01,
  0x02, 0x02, 0x14, 0x11, 0x6a, 0xf1, 0x20, 0xea, 0x37, 0xda, 0xde, 0xbf,
  0x7c, 0x15, 0x72, 0x64, 0x84, 0xe5, 0xc3, 0x6f, 0xd8, 0xf7, 0x33, 0x30,
  0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b,
  0x05, 0x00, 0x30, 0x14, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04,
  0x03, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74,
  0x30, 0x1e, 0x17, 0x0d, 0x32, 0x35, 0x30, 0x35, 0x33, 0x30, 0x32, 0x30,
  0x32, 0x33, 0x33, 0x32, 0x5a, 0x17, 0x0d, 0x32, 0x36, 0x30, 0x35, 0x33,
  0x30, 0x32, 0x30, 0x32, 0x33, 0x33, 0x32, 0x5a, 0x30, 0x14, 0x31, 0x12,
  0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x09, 0x6c, 0x6f, 0x63,
  0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x30, 0x82, 0x02, 0x22, 0x30, 0x0d,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05,
  0x00, 0x03, 0x82, 0x02, 0x0f, 0x00, 0x30, 0x82, 0x02, 0x0a, 0x02, 0x82,
  0x02, 0x01, 0x00, 0xd8, 0x78, 0xe5, 0xfc, 0xf1, 0x9f, 0x1c, 0x1e, 0x35,
  0xfc, 0x70, 0xd4, 0x81, 0xf7, 0xfb, 0xb3, 0xa4, 0x0c, 0x26, 0x49, 0xf2,
  0x56, 0x06, 0xc7, 0x66, 0xd7, 0x69, 0x38, 0x2a, 0x4e, 0x5d, 0x53, 0xae,
  0x3a, 0x09, 0x61, 0x97, 0xc1, 0xbc, 0xf1, 0xa0, 0x7c, 0xd8, 0x5c, 0x66,
  0xec, 0x59, 0xe9, 0x17, 0xb6, 0xab, 0x37, 0xf7, 0xbc, 0x78, 0x4f, 0x9c,
  0x77, 0x9d, 0x4c, 0xf9, 0x21, 0xd8, 0x99, 0xc6, 0x5a, 0xc5, 0x2f, 0xf0,
  0x39, 0xad, 0x9c, 0x27, 0x95, 0xa0, 0x4e, 0xd7, 0x53, 0xc3, 0x52, 0x42,
  0x3e, 0xbb, 0x2a, 0x95, 0x47, 0xdf, 0xf0, 0xf1, 0xad, 0xd9, 0xc4, 0xde,
  0x77, 0xf1, 0xaa, 0xa5, 0x2b, 0xd3, 0x55, 0xc7, 0xb1, 0x9d, 0x94, 0xca,
  0x5a, 0x8e, 0x5f, 0xee, 0x52, 0xe4, 0xb6, 0x5c, 0xea, 0xb4, 0x9e, 0x09,
  0x93, 0x2e, 0xba, 0x21, 0xe5, 0xa8, 0x93, 0x01, 0x42, 0x3e, 0xb5, 0x90,
  0x88, 0x9c, 0x7f, 0x48, 0x78, 0x61, 0xcb, 0x4a, 0xb2, 0x00, 0x60, 0x4d,
  0x11, 0x96, 0xd7, 0x8a, 0xe5, 0x4c, 0xfa, 0x9a, 0x61, 0x59, 0xa2, 0x65,
  0x1b, 0xdc, 0x58, 0xdb, 0x2e, 0x13, 0xae, 0x65, 0xfd, 0x49, 0x36, 0xaf,
  0x67, 0x45, 0xc6, 0xe2, 0x2f, 0xce, 0x98, 0x04, 0x3f, 0x8f, 0x4c, 0x76,
  0x01, 0x47, 0xf7, 0x18, 0x57, 0x5e, 0xa5, 0x06, 0x8e, 0xf9, 0x90, 0x4f,
  0x53, 0x19, 0xe0, 0x13, 0x40, 0xb3, 0x04, 0x28, 0x3a, 0x46, 0x79, 0x56,
  0xf9, 0x6c, 0x5a, 0x28, 0xbf, 0x98, 0xb2, 0x47, 0xf5, 0x38, 0xe2, 0x86,
  0xfe, 0x19, 0xa4, 0x59, 0x52, 0x20, 0xba, 0x9e, 0xbb, 0x4b, 0xd8, 0x73,
  0x8d, 0x62, 0xfa, 0x46, 0xd7, 0xbb, 0x5f, 0x5f, 0xcd, 0x47, 0x7b, 0xd6,
  0x30, 0x6a, 0x06, 0x22, 0xed, 0x04, 0xa7, 0x09, 0xee, 0xd4, 0xe5, 0x66,
  0xad, 0x7d, 0xbe, 0xc8, 0x8b, 0xf9, 0xa0, 0xd0, 0x35, 0x10, 0x5c, 0x49,
  0x61, 0x51, 0x01, 0x74, 0x93, 0xfa, 0xae, 0x44, 0x79, 0x19, 0xff, 0x4c,
  0xa4, 0x22, 0x26, 0x42, 0x14, 0xf0, 0x3f, 0x5a, 0x0b, 0x01, 0x23, 0xc0,
  0xba, 0x96, 0x4d, 0xf2, 0xf7, 0x9b, 0x2f, 0xdc, 0x52, 0x2a, 0x12, 0x22,
  0x6b, 0xcf, 0x6e, 0xe8, 0x0e, 0x2b, 0xc0, 0xe1, 0xbe, 0x78, 0x76, 0x55,
  0xd7, 0xd1, 0x1f, 0x38, 0xe1, 0x33, 0x64, 0xe9, 0x73, 0x9d, 0x96, 0x3d,
  0xb4, 0x89, 0x3e, 0xb4, 0xb7, 0xad, 0xfe, 0x89, 0xe0, 0xee, 0xca, 0x44,
  0xd1, 0x0b, 0x78, 0x1b, 0x22, 0x56, 0x56, 0x47, 0xce, 0x05, 0xee, 0x8a,
  0xc2, 0x68, 0xa3, 0x3c, 0x56, 0xf7, 0x56, 0x63, 0xaf, 0x70, 0xe5, 0xe5,
  0xf0, 0xe2, 0x75, 0xfd, 0xf4, 0x15, 0x2d, 0x54, 0xf9, 0x2c, 0x17, 0x8b,
  0x4c, 0x73, 0x09, 0x1f, 0x52, 0x11, 0x61, 0x0d, 0xaf, 0x64, 0x9b, 0xd7,
  0xfc, 0x2a, 0x65, 0xc9, 0x75, 0x0e, 0xf7, 0x73, 0x46, 0xfa, 0xd6, 0x62,
  0x9f, 0x0e, 0x42, 0x62, 0xf0, 0xe3, 0x5f, 0xb1, 0x17, 0x9a, 0x7f, 0xa4,
  0xbb, 0x38, 0xb1, 0x24, 0x41, 0x37, 0x05, 0xb9, 0x46, 0x23, 0x3d, 0xe2,
  0x23, 0x3c, 0xf8, 0x4a, 0x28, 0x3a, 0xa8, 0xe3, 0xaf, 0x4b, 0x6a, 0xdd,
  0x13, 0x84, 0xdb, 0x45, 0xd3, 0xc3, 0x5f, 0x59, 0xfe, 0x1d, 0x97, 0xca,
  0x3b, 0x70, 0x55, 0x56, 0x13, 0xf8, 0xfe, 0x49, 0x76, 0x55, 0x46, 0x16,
  0x34, 0x6b, 0x83, 0x1b, 0xb8, 0x6e, 0x30, 0x6c, 0xe9, 0x1c, 0xea, 0xc4,
  0x65, 0xb4, 0x82, 0xa6, 0xc0, 0x31, 0x6f, 0xec, 0xf6, 0xc9, 0x90, 0x70,
  0x18, 0xfc, 0x70, 0xea, 0x2a, 0xd5, 0xfd, 0xf8, 0x6d, 0x83, 0x2c, 0x6a,
  0x53, 0x00, 0x4a, 0xe9, 0x58, 0x4c, 0x37, 0x41, 0x9a, 0x06, 0x04, 0xf1,
  0x9e, 0x85, 0xfa, 0xa1, 0x44, 0xbd, 0x0f, 0x6f, 0xf7, 0x70, 0x25, 0x02,
  0x03, 0x01, 0x00, 0x01, 0xa3, 0x53, 0x30, 0x51, 0x30, 0x1d, 0x06, 0x03,
  0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x57, 0x73, 0xbd, 0xf6, 0x3d,
  0x43, 0xdd, 0xd7, 0xa1, 0x05, 0x94, 0x1c, 0xf9, 0x7a, 0xeb, 0x56, 0xeb,
  0xba, 0x69, 0x4e, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18,
  0x30, 0x16, 0x80, 0x14, 0x57, 0x73, 0xbd, 0xf6, 0x3d, 0x43, 0xdd, 0xd7,
  0xa1, 0x05, 0x94, 0x1c, 0xf9, 0x7a, 0xeb, 0x56, 0xeb, 0xba, 0x69, 0x4e,
  0x30, 0x0f, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x05,
  0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48,
  0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x03, 0x82, 0x02, 0x01,
  0x00, 0x3e, 0x0d, 0x15, 0x3c, 0x9f, 0xef, 0x28, 0x11, 0x82, 0xde, 0x13,
  0x4b, 0x0a, 0xe9, 0x04, 0x5c, 0x9f, 0x17, 0xa4, 0xbc, 0x81, 0xad, 0x76,
  0x8a, 0x56, 0x42, 0x7c, 0x27, 0xcd, 0xb1, 0x86, 0x3e, 0x1a, 0xe9, 0x67,
  0x90, 0x6e, 0x6b, 0x3f, 0x3f, 0xb3, 0xd4, 0xda, 0xd4, 0x08, 0x90, 0x61,
  0x4e, 0x5b, 0x73, 0xc1, 0xd3, 0x87, 0x90, 0x40, 0xb0, 0x17, 0x93, 0x79,
  0x9e, 0xc8, 0x92, 0xd9, 0x4a, 0x0a, 0xf1, 0x7a, 0x36, 0xc1, 0x8f, 0x08,
  0x62, 0x94, 0x3f, 0xa8, 0x9b, 0xcc, 0x88, 0xeb, 0x58, 0x10, 0x97, 0x7d,
  0x3d, 0x0c, 0x74, 0x07, 0xb7, 0xe9, 0xaf, 0x63, 0xfd, 0x08, 0xb3, 0x7e,
  0x6e, 0xc9, 0xa1, 0x00, 0x1c, 0xea, 0xb9, 0xc5, 0x71, 0xa5, 0x21, 0x36,
  0x30, 0x78, 0x99, 0xdd, 0x69, 0xc9, 0xfd, 0xbd, 0x11, 0xa3, 0xeb, 0x07,
  0xf9, 0x4d, 0x6e, 0x6f, 0xd8, 0xb3, 0x4e, 0xfd, 0x53, 0xb9, 0xa0, 0x98,
  0x51, 0xf9, 0x26, 0x2d, 0x04, 0x67, 0xfd, 0x2a, 0x00, 0x6e, 0x6d, 0xaf,
  0xc6, 0x30, 0xa1, 0x8a, 0xb2, 0x1b, 0x5b, 0x3a, 0x53, 0x1a, 0x36, 0xc2,
  0x62, 0x98, 0xec, 0x5a, 0xf5, 0x5e, 0xba, 0xfa, 0xc4, 0x8a, 0xf9, 0xb5,
  0x73, 0x1a, 0x01, 0xe2, 0xe3, 0x6d, 0xe3, 0xae, 0x55, 0x6b, 0x0c, 0x0e,
  0x8f, 0xe1, 0x98, 0xe2, 0x1d, 0xfa, 0x9f, 0x9f, 0xb4, 0x64, 0xc9, 0xa7,
  0x30, 0x03, 0xe6, 0xe3, 0x21, 0xb6, 0xce, 0x64, 0xa0, 0xcd, 0x38, 0x8e,
  0x86, 0xc7, 0xba, 0x67, 0x46, 0x95, 0xe2, 0x7b, 0xf2, 0xbe, 0x49, 0x61,
  0xe8, 0x41, 0xb2, 0xf9, 0xef, 0xc1, 0x86, 0xcb, 0x7b, 0xc3, 0x9e, 0x31,
  0x24, 0x11, 0x9f, 0x84, 0x1c, 0x59, 0xf7, 0xc0, 0x0d, 0xb1, 0x69, 0xb2,
  0x14, 0x7f, 0x1d, 0x09, 0x70, 0x98, 0x95, 0x9c, 0x61, 0xf5, 0x52, 0xe1,
  0x5b, 0x24, 0x9c, 0x1a, 0xab, 0x64, 0x1c, 0x50, 0x75, 0x4e, 0x1f, 0x4c,
  0xf1, 0x75, 0xbf, 0xdd, 0x0e, 0x97, 0xa0, 0xe6, 0x15, 0xec, 0x71, 0x33,
  0xf7, 0xc4, 0xfa, 0x96, 0xce, 0xd5, 0x52, 0xad, 0x73, 0x21, 0x50, 0x77,
  0x4c, 0x0f, 0x32, 0x05, 0xee, 0xff, 0xbc, 0x0c, 0xd0, 0xe9, 0xe3, 0x9b,
  0xd4, 0x9a, 0x65, 0x76, 0x71, 0x1e, 0xd8, 0x81, 0x7a, 0x65, 0x33, 0xf9,
  0x75, 0x9d, 0xcd, 0x3a, 0x7c, 0xd6, 0xaa, 0x5a, 0x7f, 0x82, 0xa8, 0x75,
  0xfa, 0x96, 0xbd, 0x2c, 0x1e, 0x21, 0x4f, 0x66, 0xdc, 0xdf, 0x9e, 0xd3,
  0x09, 0x96, 0x67, 0x79, 0xc5, 0x16, 0x36, 0x0f, 0x69, 0xa9, 0xe3, 0x3c,
  0x71, 0x0c, 0x0e, 0x7a, 0x32, 0xb2, 0xd1, 0xfa, 0x58, 0x9a, 0xe6, 0x29,
  0x05, 0x02, 0xff, 0x8a, 0xfc, 0x7a, 0xd0, 0x71, 0xee, 0x5e, 0x42, 0xd2,
  0xba, 0x4a, 0x5d, 0xd5, 0x99, 0x63, 0xf5, 0x9e, 0x26, 0xc3, 0x80, 0xb4,
  0xe3, 0xbc, 0xcd, 0x4d, 0x58, 0xd5, 0x7e, 0x64, 0x06, 0xee, 0x60, 0x4a,
  0x2d, 0xc1, 0xb3, 0xc3, 0x94, 0xae, 0x05, 0x9d, 0x7c, 0xbb, 0x71, 0xed,
  0x3d, 0x0f, 0xab, 0x8d, 0x53, 0x36, 0x1d, 0xef, 0x2f, 0x65, 0xc4, 0xd3,
  0x9d, 0x34, 0x22, 0x7f, 0x04, 0xc6, 0xd3, 0xd9, 0xd1, 0x9a, 0xb0, 0xaa,
  0x0c, 0x7f, 0xae, 0x19, 0xae, 0x64, 0xbc, 0xe1, 0x33, 0x82, 0x47, 0x45,
  0x8b, 0x43, 0x6a, 0x04, 0x54, 0x27, 0x8e, 0x4c, 0x45, 0x3b, 0x9b, 0x2f,
  0xa3, 0x5b, 0xed, 0xbf, 0xe0, 0xb2, 0xca, 0x27, 0x38, 0x00, 0x12, 0x01,
  0xac, 0x89, 0xc6, 0x01, 0x35, 0x24, 0x84, 0x63, 0xda, 0x49, 0xd9, 0xb3,
  0xec, 0xe7, 0x96, 0x51, 0x35, 0x6c, 0x16, 0xb4, 0xb3, 0xf3, 0x1d, 0xec,
  0x9e, 0x81, 0x69, 0xfc, 0x67, 0xbc, 0xc9, 0xe4, 0xb2, 0x1c, 0xed, 0x84,
  0xff, 0xbb, 0xaf, 0x49, 0xcd, 0xe0, 0x70, 0x03, 0x94
};
// Настройки WiFi и MQTT
const char* ssid = "Odeyalo";
const char* password = "20012005";
const char* mqtt_server = "mqtt.cloud.yandex.net";
const int mqtt_port = 8883;

// Объекты
WiFiClientSecure espClient;
PubSubClient client(espClient);
Adafruit_BMP280 bmp;  // Датчик BMP280

// Переменные
unsigned long lastMsgTime = 0;
const long interval = 5000;  // Отправка данных каждые 5 сек
bool ledDebugState = false;

void setup() {
  Serial.begin(115200);
  
  // Настройка пинов
  pinMode(LED_PWM, OUTPUT);
  pinMode(PIN_LED_DEBUG, OUTPUT);
  pinMode(PIN_DIN_2, INPUT);
  pinMode(PIN_BUTT_DEBUG, INPUT_PULLUP);
  
  // Инициализация BMP280
  Wire.begin(BMP280_SDA, BMP280_SCL);
  if (!bmp.begin(0x76)) {  // Адрес 0x76 или 0x77
    Serial.println("BMP280 не найден!");
  }

  // Подключение к WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi подключен");

  // Настройка MQTT
  espClient.setCACert(ca_cert, sizeof(ca_cert));
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Мигаем LED_DEBUG каждую секунду
  if (millis() % 1000 < 500) {
    digitalWrite(PIN_LED_DEBUG, HIGH);
  } else {
    digitalWrite(PIN_LED_DEBUG, LOW);
  }

  // Отправка данных в MQTT раз в `interval` мс
  if (millis() - lastMsgTime > interval) {
    lastMsgTime = millis();
    sendDataToCloud();
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Device", "идентификатор-устройства", "пароль-устройства")) {
      client.subscribe("$devices/<ID устройства>/commands");
    } else {
      delay(5000);
    }
  }
}

void sendDataToCloud() {
  // Чтение аналоговых входов
  int ain1 = analogRead(PIN_AIN_1);
  int ain2 = analogRead(PIN_AIN_2);
  int ain3 = analogRead(PIN_AIN_3);
  
  // Чтение цифрового входа
  int din2 = digitalRead(PIN_DIN_2);
  
  // Чтение данных с BMP280
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;  // в гПа

  // Формируем JSON-сообщение
  String payload = "{";
  payload += "\"ain1\":" + String(ain1) + ",";
  payload += "\"ain2\":" + String(ain2) + ",";
  payload += "\"ain3\":" + String(ain3) + ",";
  payload += "\"din2\":" + String(din2) + ",";
  payload += "\"temperature\":" + String(temperature) + ",";
  payload += "\"pressure\":" + String(pressure);
  payload += "}";

  // Отправка в MQTT
  client.publish("$devices/<ID устройства>/events", payload.c_str());
  Serial.println("Данные отправлены: " + payload);
}