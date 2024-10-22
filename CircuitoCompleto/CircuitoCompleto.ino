#include <Wire.h>
#include <MPU6050.h>

const int motorPin = 26;

const int trigPin = 5;
const int echoPin = 18;

// Definir la velocidad del sonido en cm/us
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

MPU6050 mpu;


void setup() {
  Serial.begin(115200);
  // Inicializar I2C
  Wire.begin();

  // Inicializar el sensor MPU-6050
  Serial.println("Iniciando el sensor MPU-6050...");

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("Error al iniciar el sensor MPU-6050. Verifica la conexión.");
    while(1);
  }
  
  Serial.println("MPU-6050 iniciado correctamente.");

  // Inicializar pin de salida
  pinMode(motorPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  // -------------- GROVE MOTOR -----------------------
  // Encender el motor
  digitalWrite(motorPin, HIGH);
  delay(2000);
  
  // --------------- HC-SR04 ------------------------
  // Asegurarse de que el Trigger está apagado antes de iniciar la medición
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  
  // Enviar una señal de 10 microsegundos al Trigger para iniciar la medición
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigPin, LOW);
  
  // Leer el tiempo de viaje del pulso usando el pin Echo (en microsegundos)
  duration = pulseIn(echoPin, HIGH);
  
  // Calcular la distancia en centímetros
  distanceCm = duration * SOUND_SPEED / 2;
  
  // Convertir la distancia a pulgadas
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Mostrar la distancia en el Monitor Serial
  Serial.println("----- HC-SR04 -----");
  Serial.print("Distancia (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distancia (inch): ");
  Serial.println(distanceInch);
  

    // -------------- MPU-6050 -------------------------
  
  // Leer datos del sensor
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // Imprimir los datos en la consola serial
  Serial.println("----- MPU-6050 -----"); 
  Serial.print("Aceleración X: "); 
  Serial.print(ax);
  Serial.print(" | Aceleración Y: "); 
  Serial.print(ay);
  Serial.print(" | Aceleración Z: "); 
  Serial.println(az);
  
  Serial.print("Giroscopio X: "); 
  Serial.print(gx);
  Serial.print(" | Giroscopio Y: "); 
  Serial.print(gy);
  Serial.print(" | Giroscopio Z: "); 
  Serial.println(gz);

    // Apagar motor
  digitalWrite(motorPin, LOW);
  
  // Esperar 500ms antes de la siguiente lectura
  delay(2000);


}
