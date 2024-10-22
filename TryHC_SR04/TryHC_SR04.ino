const int trigPin = 5;
const int echoPin = 18;

// Definir la velocidad del sonido en cm/us
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200);   // Iniciar la comunicación serial para depuración
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
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
  Serial.print("Distancia (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distancia (inch): ");
  Serial.println(distanceInch);
  
  delay(2000);
}
