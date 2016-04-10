#include "BeanChannel.h"
void BeanChannel::setup() {
    Wire.begin();
}


void BeanChannel::sendByte(char data) {
    sendData(1, &data);
}
void BeanChannel::sendData(int dataCount, char *data) {
    Wire.beginTransmission(1);
    for (int i = 0; i < dataCount; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}

void BeanChannel::loop() {
    Wire.requestFrom(1, 1);
}
bool BeanChannel::hasData() {
    return Wire.available();
}
char BeanChannel::getCurrentValue() {
    return Wire.read();
}
