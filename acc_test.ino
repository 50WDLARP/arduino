float prev_acc = 0;
void setup() {
   Serial.begin(57600);
}

void loop() {
    AccelerationReading reading = Bean.getAcceleration();
    char x = abs(reading.xAxis) / 2;
    char y = abs(reading.yAxis) / 2;
    char z = abs(reading.zAxis) / 2;
    Bean.setLed(0,255,0);
//    Bean.setLed(x, y, z);
    int i_x = x;
    int i_y = y;
    int i_z = z;
//    Serial.println(i_x);
//    Serial.println(i_y);
//    Serial.println(i_z);
    float acc = sqrt(x*x + y*y + z*z);
    if (abs(acc - prev_acc) > 10.0){
      Serial.println("MOVE");
      Bean.setLed(255,0,0);
    } else {
      Bean.setLed(0,255,0);
    }
    prev_acc = acc;
//    Serial.println(acc);
}
