const int dt = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start");
  pinMode(dt, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(!digitalRead(dt));
  delay(10);
}
