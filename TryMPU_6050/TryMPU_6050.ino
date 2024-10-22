#include <Wire.h>
#include <MPU6050.h>

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

}

void loop() {
  // Leer datos del sensor
  //Vector rawAccel = mpu.readRawAccel(); // Lee los valores crudos del acelerómetro
  //Vector normAccel = mpu.readNormalizeAccel(); // Lee los valores normalizados del acelerómetro
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // Imprimir los datos en la consola serial
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
  // Esperar 500ms antesd e la siguiente lectura
  delay(500);

}
