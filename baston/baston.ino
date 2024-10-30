#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Pines de sensor ultrasónico
const int trigPin = 5;
const int echoPin = 18;

// Pin grove motor
const int motorPin = 26;

long duration;
int distance;

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
  // Las lecturas se hacen cada medio segundo
  delay(2000);
}
