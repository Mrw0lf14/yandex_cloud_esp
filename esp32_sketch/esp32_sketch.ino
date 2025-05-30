#include <WiFi.h>
#include <PubSubClient.h>     
#include <Wire.h>
#include <Adafruit_BMP280.h>  // Установите библиотеку через менеджер библиотек

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
  espClient.setInsecure();  // Для упрощённого SSL (в продакшене используйте сертификаты)
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