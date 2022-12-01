#include <mbed.h>
// #include "max30102.h"

// MAX30102 sensor;
I2C i2c(PF_0, PF_1);

// I2C address
#define MAX30102_WRITE_ADDRESS 0xAE
#define MAX30102_READ_ADDRESS 0xAF

#define MAX30102_CONFIG 0x09
#define MAX30102_REG_SPO2_CONFIG 0x0A /**< spo2 config register */

#define MAX30102_REG_REVISION_ID 0xFE
#define MAX30102_REG_PART_ID 0xFF // Should always be 0x15. Identical to MAX30102.

#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define MAX30102_INT_ENABLE 0x02

/**
 * @brief max30102 sample averaging enumeration definition
 */
typedef enum
{
  MAX30102_SAMPLE_AVERAGING_1 = 0x00,  /**< no averaging */
  MAX30102_SAMPLE_AVERAGING_2 = 0x01,  /**< averaging 2 */
  MAX30102_SAMPLE_AVERAGING_4 = 0x02,  /**< averaging 4 */
  MAX30102_SAMPLE_AVERAGING_8 = 0x03,  /**< averaging 8 */
  MAX30102_SAMPLE_AVERAGING_16 = 0x04, /**< averaging 16 */
  MAX30102_SAMPLE_AVERAGING_32 = 0x05, /**< averaging 32 */
} max30102_sample_averaging_t;

/**
 * @brief max30102 mode enumeration definition
 */
typedef enum
{
  MAX30102_MODE_HEART_RATE = 0x02, /**< heart rate mode */
  MAX30102_MODE_SPO2 = 0x03,       /**< spo2 mode */
  MAX30102_MODE_MULTI_LED = 0x07,  /**< multi-led mode */
  MAX30102_MODE_SHUTDOWN = 0x80,
  MAX30102_MODE_RESET = 0x40,
} max30102_mode_t;

/**
 * @brief max30102 spo2 sample rate enumeration definition
 */
typedef enum
{
  MAX30102_SPO2_SAMPLE_RATE_50_HZ = 0,   /**< 50Hz */
  MAX30102_SPO2_SAMPLE_RATE_100_HZ = 1,  /**< 100Hz */
  MAX30102_SPO2_SAMPLE_RATE_200_HZ = 2,  /**< 200Hz */
  MAX30102_SPO2_SAMPLE_RATE_400_HZ = 3,  /**< 400Hz */
  MAX30102_SPO2_SAMPLE_RATE_800_HZ = 4,  /**< 800Hz */
  MAX30102_SPO2_SAMPLE_RATE_1000_HZ = 5, /**< 1000Hz */
  MAX30102_SPO2_SAMPLE_RATE_1600_HZ = 6, /**< 1600Hz */
  MAX30102_SPO2_SAMPLE_RATE_3200_HZ = 7, /**< 3200Hz */
} max30102_spo2_sample_rate_t;

/**
 * @brief max30102 adc resolution enumeration definition
 */
typedef enum
{
  MAX30102_ADC_RESOLUTION_15_BIT = 0, /**< 15 bits */
  MAX30102_ADC_RESOLUTION_16_BIT = 1, /**< 16 bits */
  MAX30102_ADC_RESOLUTION_17_BIT = 2, /**< 17 bits */
  MAX30102_ADC_RESOLUTION_18_BIT = 3, /**< 18 bits */
} max30102_adc_resolution_t;

class MAX30102
{
public:
  MAX30102(void)
  {

    reset();

    char write_buffer[1];
    write_buffer[0] = REG_INTR_STATUS_1;
    i2c.write(MAX30102_WRITE_ADDRESS, write_buffer, 1, true);
    char read_buffer[1];
    i2c.read(MAX30102_READ_ADDRESS, read_buffer, 1);

    setup();
  }

  void getPartId()
  {
    char write_buffer[1];
    write_buffer[0] = MAX30102_REG_PART_ID;
    i2c.write(MAX30102_WRITE_ADDRESS, write_buffer, 1, true);
    printf("MAX30102_REG_PART_ID i2c.write: %d\n", write);
    char read_buffer[1];
    int read = i2c.read(MAX30102_READ_ADDRESS, read_buffer, 1);
    printf("Success: %d \t Part ID: %c\n\n", read, *read_buffer);
  }

  void getRevisionId()
  {
    char write_buffer[1];
    write_buffer[0] = MAX30102_REG_REVISION_ID;
    int write = i2c.write(MAX30102_WRITE_ADDRESS, write_buffer, 1, true);
    printf("MAX30102_REG_REVISION_ID i2c.write: %d\n", write);
    char read_buffer[1];
    int read = i2c.read(MAX30102_READ_ADDRESS, read_buffer, 1);
    printf("Success: %d \t Revision ID: %c\n\n", *read_buffer);
  }

  void shutdown()
  {
    set_mode(MAX30102_MODE_SHUTDOWN);
  }

  void reset()
  {
    set_mode(MAX30102_MODE_RESET);
  }

private:
  void setup()
  {
  }

  void set_mode(max30102_mode_t mode)
  {
    char write_buffer[2];

    write_buffer[0] = MAX30102_CONFIG;
    write_buffer[1] = mode;

    i2c.write(MAX30102_WRITE_ADDRESS, write_buffer, 2);
  }
};

int main()
{
  printf("STM32F429ZS Started!\n");
  MAX30102 sensor;
  while (true)
  {
    sensor.getPartId();
    sensor.getRevisionId();
    thread_sleep_for(10000);
  }
}