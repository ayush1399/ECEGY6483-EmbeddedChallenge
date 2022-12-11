#include <mbed.h>
#include <settings.h>
#include <max30102.h>
#include <hm10.h>

int main()
{
  printf("STM32 Started!\n");
  // HM10 ble(PA_9, PA_10, 9600);
  // HM10 ble(PC_10, PC_11, 9600);
  HM10 ble(PC_6, PC_7, 9600);
  MAX30102 sensor(PF_0, PF_1);

  while (1)
  {
    sensor.getPartId();
    sensor.getRevisionId();
    sensor.printTemperature();
    // int blr = ble.readable();
    // printf("Readable %d\n", blr);
    // printf("Writable %d\n", ble.writable());
    // if (blr)
    // {
    //   printf("BLE - %c\n", ble.read()[0]);
    // }
    // ble.psss();
    // ble.help();
    int t = (int)(sensor.getDieTemp() * 100);
    char sendBuff[64];
    sprintf(sendBuff, "Temperature (in Celsius): %d.%d\n", t / 100, t % 100);
    ble.write(sendBuff, 34);

    thread_sleep_for(1000);
  }
}