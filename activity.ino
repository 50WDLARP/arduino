/*
 * Code for the activity tracker
 * prints timer (in seconds) and delta acceleration to Serial
 */

unsigned long time; //used as a timer to graph activity and time (micros(): max 70 minutes)
float prev_acc = 0;
void setup() {
    Serial.begin(57600);
}

void loop() {
    /*Activity Tracker*/
    time = micros();
    float seconds = time/1000000.0;
    AccelerationReading reading = Bean.getAcceleration();
    char x = abs(reading.xAxis) / 2;
    char y = abs(reading.yAxis) / 2;
    char z = abs(reading.zAxis) / 2;
//    Bean.setLed(0,255,0);
    float acc = sqrt(x*x + y*y + z*z);
    float deltaAcc = abs(acc - prev_acc);
    if(deltaAcc > 10.0){
        Bean.setLed(0,0,255);
    }
    Serial.print(seconds);
    Serial.print(":");
    Serial.println(deltaAcc);
    prev_acc = acc;
}
