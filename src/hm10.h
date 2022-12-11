#pragma once
#include <mbed.h>
#include <settings.h>

class HM10 : public BaseUtilities
{
public:
    HM10(PinName RX, PinName TX, int baud_rate);
    void setup();
    int readable();
    bool writable();
    char *read();
    void psss();
    void help();
    void write(char *s, int size);

private:
    UnbufferedSerial uart;
    bool sss;
    int renew();
    int set_slave();
    int set_name();
};