const int motorPin = 26;

void setup() {
  // Iniciar la comunicación serial para depuración
  Serial.begin(115200);
  // Inicializar pin de salida
  pinMode(motorPin, OUTPUT);

}

void loop() {
  // Encender el motor
  digitalWrite(motorPin, HIGH);
  Serial.println("Motor de vibración encendido.");
  // Mantener vibración por 1 segundo
  delay(1000);

  // Apagar motor
  digitalWrite(motorPin, LOW);
  Serial.println("Motor de vibración apagado.");
  // Mantener apagado por 1 segundo
  delay(1000);

}
