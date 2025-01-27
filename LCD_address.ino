#include <Wire.h>

void setup() {
  Wire.begin(); // I2C 버스 초기화

  Serial.begin(9600);
  while (!Serial); // 시리얼 포트가 준비될 때까지 대기

  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ ) {
    // I2C 주소로 스캔 시도
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    // 오류 코드가 없으면 디바이스를 찾은 것임
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) 
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // 5 초 후 다시 스캔
}
