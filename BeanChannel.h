#include <Arduino.h>
#include <Wire.h>
class BeanChannel {
public:
    void setup();
    void loop();
    char getCurrentValue();
    bool hasData();
    void sendByte(char data);
private:
    void sendData(int dataCount, char *data);
};
