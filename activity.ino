/*
 * Code to be uploaded to the LightBlue Bean
 * Code for the activity tracker
 * Prints timer (in seconds) and delta acceleration to Serial
 */

unsigned long time; //used as a timer to graph activity and time (micros(): max 70 minutes)
float prev_acc = 0; //tracks the acceleration previously read
void setup() {
    Serial.begin(57600);
}

void loop() {
    /*Activity Tracker*/
    time = micros(); //tracks time from the beginning of the program
    float seconds = time/1000000.0; //converts to seconds - we can disregard and only track micros is more convenient for graphing. 
    AccelerationReading reading = Bean.getAcceleration(); //takes acceleration from Bean
    /* extracts the acceleration from each axis from the reading */
    char x = abs(reading.xAxis) / 2; 
    char y = abs(reading.yAxis) / 2;
    char z = abs(reading.zAxis) / 2;
//    Bean.setLed(0,255,0);
    /* Finds the average acceleration */
    float acc = sqrt(x*x + y*y + z*z);
    float deltaAcc = abs(acc - prev_acc);
    if(deltaAcc > 10.0){ //seems to be the threshold for substantial movements
        Bean.setLed(0,0,255);
    }
    /* logs information to Serial */ 
    Serial.print(seconds);
    Serial.print(":");
    Serial.println(deltaAcc);
    prev_acc = acc;
}
