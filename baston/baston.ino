#include <Wire.h>
#include <MPU6050.h>
#include <ThingSpeak.h>
#include <WiFi.h>

MPU6050 mpu;

// Protocolo MQTT - Credenciales del dispositivo MQTT enn ThingSpeak
#define ClIENT_ID "ASUJGBQaJCghOhIaMDEhETg"
#define USERNAME "ASUJGBQaJCghOhIaMDEhETg"
#define PASSWORD "p6vAtwUqp7RxZkroBKY332Eg"


// Pines de sensor ultrasónico
const int trigPin = 5;
const int echoPin = 18;

// Pin grove motor
const int motorPin = 26;

long duration;
float distance;

int totalAcceleration;

// Umbral para detectar caída
const int FALL_THRESHOLD = 30000;

// timer
uint32_t startMillis;
const uint16_t timerDuration = 15000;

// Credenciales WiFi
const char *ssid = "IoT";
const char *password = "1t3s0IoT23";
WiFiClient  client;

// ThingSpeak
uint32_t channelNumber = 2727289;
const char *writeAPIKey = "IPP3Y8NN0LQAQZI6";

void setup() {
  Serial.begin(115200);

  // Inicializar sensor MPU6050
  Wire.begin();
  Serial.println("Inicializando el sensor MPU-6050...");
  mpu.initialize();
  if(!mpu.testConnection()) {
    Serial.println("Error al iniciar el sensor MPU-6050. Verifica la conexión.");
    while(1);
  }
  Serial.println("MPU-6050 iniciado correctamente.");

  // Inicializar sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicializar motor de vibración
  pinMode(motorPin, OUTPUT);

  // Inicializar ThingSpeak client
  ThingSpeak.begin(client);
  connectWiFi();

  // Inicializar timer
  startMillis = millis();

}

void loop() {
  // Leer datos del acelerómetro y giroscopio
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // Imprimir los datos del acelerómetro y giroscopio
  Serial.print("Aceleración X: "); Serial.print(ax);
  Serial.print(" | Aceleración Y: "); Serial.print(ay);
  Serial.print(" | Aceleración Z: "); Serial.println(az);
  
  Serial.print("Giroscopio X: "); Serial.print(gx);
  Serial.print(" | Giroscopio Y: "); Serial.print(gy);
  Serial.print(" | Giroscopio Z: "); Serial.println(gz);

  // Calcular el valor absoluto de la aceleración total
  totalAcceleration = abs(ax) + abs(ay) + abs(az);
  Serial.print("Aceleración de "); Serial.println(totalAcceleration);
  // Verificar si se excede el umbral de caída
  if(totalAcceleration > FALL_THRESHOLD) {
    Serial.println("¡Caída detectada! Activando alerta de emergencia...");
    simulateEmergencyCall();
  }

  // Leer datos del sensor ultrasónico
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Activar el motor de vibración si la distancia es menor a 1 metro
  if(distance < 100){
    digitalWrite(motorPin, HIGH);
    Serial.println("Obstáculo detectado, vibración activada...");
    delay(2000); // Mantener el motor activado durante 2 segundos
    digitalWrite(motorPin, LOW); // Apagar el motor
  } else{
    digitalWrite(motorPin, LOW);
    Serial.println("No hay obstáculos, vibración desactivada...");
  }

  // Enviar los datos a ThingSpeak cada 15 segundos
  if (millis() >= timerDuration + startMillis) {
    sendDataToThingSpeak();
    // Reiniciar timer
    startMillis = millis();
  }
  // Las lecturas se hacen cada segundo
  delay(1000);
}

// Función para simular llamada a contacto de emergencia
void simulateEmergencyCall() {
  Serial.println("Simulando llamada a contacto de emergencia...");
  // Esperar 5 segundos como simulación de la llamada
  delay(5000);
}

// Hacer conexión a internet
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.println(); 
  Serial.print("Wait for WiFi..."); 
  Serial.print(ssid);
   
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
    delay(500); 
  } 
   
  Serial.println(""); 
  Serial.println("WiFi Connected"); 
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
}

void sendDataToThingSpeak() {
  // Revisar conexión WiFi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("No se detecto conexion a Internet");
        connectWiFi();
    }

    // Establecer a qué campo se manda cada dato
    ThingSpeak.setField(1, totalAcceleration);
    ThingSpeak.setField(2, distance);

    // Establecer mensajes de status
    String status;

    if(totalAcceleration >= 30000)
      status = "¡Alerta! Riesgo de caida";
    else
      status = "Aceleración normal, sin aparente riesgo";

    // Establecer status
    ThingSpeak.setStatus(status);

    if(distance <= 100)
      status = "¡Alerta! Riesgo de choque";
    else
      status = "No hay objeto cercano";

    // Establecer status
    ThingSpeak.setStatus(status);

    // Escribir datos en canal de ThingSpeak
    int code = ThingSpeak.writeFields(channelNumber, writeAPIKey);
    if(code == 200)
        Serial.println("Canal de Thingspeak actualizado correctamente.");  
    else
        Serial.println("Hubo un problema actualizando el canal. Error HTTP: " + String(code));
}
