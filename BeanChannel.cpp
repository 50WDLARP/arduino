#include "BeanChannel.h"


#define BEAN_ADDRESS 1
void BeanChannel::setup() {
    Wire.begin();
}


void BeanChannel::sendByte(char data) {
    sendData(BEAN_ADDRESS, &data); // Send one byte
}
void BeanChannel::sendData(int dataCount, char *data) {

    // send each byte in the array of data to the bean
    Wire.beginTransmission(BEAN_ADDRESS);
    for (int i = 0; i < dataCount; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}

void BeanChannel::loop() {
    Wire.requestFrom(BEAN_ADDRESS, 1); // Every cycle, poll the bean for data.
}
bool BeanChannel::hasData() {
    return Wire.available();
}
char BeanChannel::getCurrentValue() {
    return Wire.read();
}
