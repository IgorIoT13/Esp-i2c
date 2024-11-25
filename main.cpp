#include "i2c.h"


Controle_i2c i2c_t = Controle_i2c(21, 22, 0);

void setup() {
  Serial.begin(115200);

  i2c_t.begin();
}

void loop() {
  delay(1000);
  i2c_t.write_data('2');
}
