// This class will recieve input FROM the Raspberry Pi

int x;

void setup() {
  Serial.begin(9600); //maybe 115200 instead?
  Serial.setTimeout(1);
}

void  loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();
  Serial.print(x + 1);
}