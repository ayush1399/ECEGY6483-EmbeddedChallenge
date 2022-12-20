#include <mbed.h>
#include <settings.h>
#include <max30102.h>
#include <hm10.h>

HM10 ble(PC_6, PC_7, 9600);
MAX30102 sensor(PF_0, PF_1);
Thread threadTemp(osPriorityBelowNormal);
Thread threadReadAndTransmit(osPriorityAboveNormal7);

void readTemperature()
{
  /* Update temperature approximately every 1 second */
  while (true)
  {
    sensor.updateTemperature();
    ThisThread::sleep_for(1s);
  }
}

void getAndTransmitData()
{
  uint32_t REDData[NUM_SAMPLES_TO_READ];
  uint32_t IRData[NUM_SAMPLES_TO_READ];
  char sendBuff[64];

  while (true)
  {

    if (sensor.samplesReady())
    {
      sensor.readSamples(NUM_SAMPLES_TO_READ);
      sensor.getData(REDData, IRData);
      for (int i = 0; i < NUM_SAMPLES_TO_READ; i++)
      {
        sprintf(sendBuff, "%02d-RD-%010lu", i, REDData[i]);
        ble.write(sendBuff, 16);
        sprintf(sendBuff, "%02d-IR-%010lu", i, IRData[i]);
        ble.write(sendBuff, 16);
      }
    }

    ThisThread::sleep_for(1ms);
  }
}

int main()
{
  threadTemp.start(readTemperature);
  threadReadAndTransmit.start(getAndTransmitData);

  while (1)
  {
    sensor.getPartId();
    sensor.getRevisionId();
    // sensor.printTemperature();
    // int blr = ble.readable();
    // printf("Readable %d\n", blr);
    // printf("Writable %d\n", ble.writable());

    int t = (int)(sensor.getDieTemp() * 100);
    char sendBuff[64];
    sprintf(sendBuff, "TEMP-%02d.%d", t / 100, t % 100);
    ble.write(sendBuff, 12);

    thread_sleep_for(1000);
  }
}