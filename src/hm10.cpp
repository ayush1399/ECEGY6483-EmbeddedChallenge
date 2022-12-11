#include <hm10.h>

HM10::HM10(PinName RX, PinName TX, int baud_rate) : uart(RX, TX, baud_rate)
{
    sss = false;
    // uart.set_baud(baud_rate);
    while (!uart.writable())
    {
    };
    sss = true;
    setup();
}

void HM10::help()
{
    uart.write("AT+HELP\r\n", 9);
}

void HM10::write(char *s, int size)
{
    uart.write(s, size);
}

void HM10::setup()
{
    print("Renew Command: %d\n", renew());
    print("Set Slave Command: %d\n", set_slave());
    print("Set Name Command: %d\n", set_name());
}

int HM10::renew()
{
    return uart.write("AT+RENEW\r\n", 10);
}

int HM10::set_slave()
{
    return uart.write("AT+ROLE0\r\n", 10);
}

int HM10::set_name()
{
    return uart.write("AT+NAME6483\r\n", 18);
}

int HM10::readable()
{
    return uart.readable();
}

bool HM10::writable()
{
    return uart.writable();
}

char *HM10::read()
{
    char read_buffer[10];
    uart.read(read_buffer, 10);
    return read_buffer;
}

void HM10::psss()
{
    if (sss)
    {
        print("TRUE\n");
    }
    else
    {
        print("FALSE\n");
    }
}

// void HM10::set_service_pulse_oximeter(){

// }