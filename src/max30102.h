#pragma once
#include <mbed.h>
#include <settings.h>

#define NUM_SAMPLES_TO_READ 24

// I2C address
typedef enum
{
    MAX30102_INTR_STATUS_1 = 0x00,
    MAX30102_INTR_STATUS_2 = 0x01,
    MAX30102_INTR_ENABLE_1 = 0x02,
    MAX30102_INTR_ENABLE_2 = 0x03,
    MAX30102_FIFO_WR_PTR = 0x04,
    MAX30102_OVF_COUNTER = 0x05,
    MAX30102_FIFO_RD_PTR = 0x06,
    MAX30102_FIFO_DATA = 0x07,
    MAX30102_FIFO_CONFIG = 0x08,
    MAX30102_MODE_CONFIG = 0x09,
    MAX30102_SPO2_CONFIG = 0x0A,
    MAX30102_LED_PW_1 = 0x0C,
    MAX30102_LED_PW_2 = 0x0D,
    MAX30102_LED_PILOT = 0x10,
    MAX30102_DIE_TEMP_INT = 0x1F,
    MAX30102_DIE_TEMP_FRAC = 0x20,
    MAX30102_DIE_TEMP_CONFIG = 0x21,
    MAX30102_WRITE_ADDRESS = 0xAE,
    MAX30102_READ_ADDRESS = 0xAF,
    MAX30102_REVISION_ID = 0xFE,
    MAX30102_PART_ID = 0xFF,
} max30102_registers_t;

/**
 * @brief max30102 sample averaging enumeration definition
 */
typedef enum
{
    MAX30102_SAMPLE_AVERAGING_1 = 0x00,
    MAX30102_SAMPLE_AVERAGING_2 = 0x01,
    MAX30102_SAMPLE_AVERAGING_4 = 0x02,
    MAX30102_SAMPLE_AVERAGING_8 = 0x03,
    MAX30102_SAMPLE_AVERAGING_16 = 0x04,
    MAX30102_SAMPLE_AVERAGING_32 = 0x05,
} max30102_sample_averaging_t;

typedef enum
{
    MAX30102_FIFO_ROLLOVER_DISABLED = 0x00,
    MAX30102_FIFO_ROLLOVER_ENABLED = 0x01,
} max30102_fifo_rollover_t;

typedef enum
{
    MAX30102_FIFO_UNREAD_17 = 0x00,
    MAX30102_FIFO_UNREAD_18 = 0x01,
    MAX30102_FIFO_UNREAD_19 = 0x02,
    MAX30102_FIFO_UNREAD_20 = 0x03,
    MAX30102_FIFO_UNREAD_21 = 0x04,
    MAX30102_FIFO_UNREAD_22 = 0x05,
    MAX30102_FIFO_UNREAD_23 = 0x06,
    MAX30102_FIFO_UNREAD_24 = 0x07,
    MAX30102_FIFO_UNREAD_25 = 0x08,
    MAX30102_FIFO_UNREAD_26 = 0x09,
    MAX30102_FIFO_UNREAD_27 = 0x0A,
    MAX30102_FIFO_UNREAD_28 = 0x0B,
    MAX30102_FIFO_UNREAD_29 = 0x0C,
    MAX30102_FIFO_UNREAD_30 = 0x0D,
    MAX30102_FIFO_UNREAD_31 = 0x0E,
    MAX30102_FIFO_UNREAD_32 = 0x0F,
} max30102_fifo_a_full_unread_val_t;

/**
 * @brief max30102 mode enumeration definition
 */
typedef enum
{
    MAX30102_MODE_HEART_RATE = 0x02,
    MAX30102_MODE_SPO2 = 0x03,
    MAX30102_MODE_MULTI_LED = 0x07,
    MAX30102_MODE_SHUTDOWN = 0x80,
    MAX30102_MODE_RESET = 0x40,
} max30102_mode_t;

typedef enum
{
    MAX30102_SPO2_SAMPLE_RATE_50_HZ = 0x0,
    MAX30102_SPO2_SAMPLE_RATE_100_HZ = 0x1,
    MAX30102_SPO2_SAMPLE_RATE_200_HZ = 0x2,
    MAX30102_SPO2_SAMPLE_RATE_400_HZ = 0x3,
    MAX30102_SPO2_SAMPLE_RATE_800_HZ = 0x4,
    MAX30102_SPO2_SAMPLE_RATE_1000_HZ = 0x5,
    MAX30102_SPO2_SAMPLE_RATE_1600_HZ = 0x6,
    MAX30102_SPO2_SAMPLE_RATE_3200_HZ = 0x7,
} max30102_spo2_sample_rate_t;

typedef enum
{
    MAX30102_ADC_RESOLUTION_15_BIT = 0x0,
    MAX30102_ADC_RESOLUTION_16_BIT = 0x1,
    MAX30102_ADC_RESOLUTION_17_BIT = 0x2,
    MAX30102_ADC_RESOLUTION_18_BIT = 0x3,
} max30102_adc_resolution_t;

typedef enum
{
    MAX30102_SPO2_ADC_RANGE_2048 = 0x0,
    MAX30102_SPO2_ADC_RANGE_4096 = 0x1,
    MAX30102_SPO2_ADC_RANGE_8192 = 0x2,
    MAX30102_SPO2_ADC_RANGE_16384 = 0x3,
} max30102_spo2_adc_range_t;

typedef enum
{
    MAX30102_DIE_TEMP_STATUS_DISABLED = 0x0,
    MAX30102_DIE_TEMP_STATUS_ENABLED = 0x1,
} max30102_die_temp_status_t;

typedef enum
{
    MAX30102_CURRENT_7mA = 0x24,
    MAX30102_CURRENT_25mA = 0x7F,
} max30102_led_cr_t;

typedef enum
{
    MAX30102_A_FULL = 0x80,
    MAX30102_PPG_RDY = 0x40,
    MAX30102_ALC_OVF = 0x20,
    MAX30102_DIE_TEMP_RDY = 0x02
} max30102_intr_flag_t;

class MAX30102 : public BaseUtilities
{
public:
    MAX30102(PinName SDA, PinName SCL);
    void getPartId();
    void getRevisionId();
    void shutdown();
    void reset();

    void printTemperature();
    float getDieTemp();
    void updateTemperature();

    void readSamples(const int num);
    void getData(uint32_t *REDData, uint32_t *IRData);
    bool samplesReady();

private:
    I2C i2c;

    uint32_t RED[NUM_SAMPLES_TO_READ];
    uint32_t IR[NUM_SAMPLES_TO_READ];

    uint8_t spo2_config;
    uint8_t intr_enable_1;
    uint8_t intr_enable_2;

    float die_temp;

    void setup(void);
    void set_mode(const max30102_mode_t mode);

    void setup_intr(bool enable_a_full, bool enable_ppg_rdy, bool enable_alc_ovf, bool enable_die_temp_rdy);
    void enable_intr_a_full();
    void disable_intr_a_full();
    void enable_intr_ppg_rdy();
    void disable_intr_ppg_rdy();
    void enable_intr_alc_ovf();
    void disable_intr_alc_ovf();
    void enable_intr_die_temp_rdy();
    void disable_intr_die_temp_rdy();

    void setup_spo2(max30102_spo2_adc_range_t spo2_adc_range, max30102_spo2_sample_rate_t spo2_sr, max30102_adc_resolution_t spo2_led_pw);
    void set_spo2_adc_rge(max30102_spo2_adc_range_t adc_range);
    void set_spo2_sr(max30102_spo2_sample_rate_t spo2_sr);
    void set_spo2_led_pw(max30102_adc_resolution_t led_pw);

    void set_fifo_config(max30102_sample_averaging_t fifo_sample_average, max30102_fifo_rollover_t fifo_rollover, max30102_fifo_a_full_unread_val_t fifo_unread);

    void set_led1_pa(max30102_led_cr_t led_cr_mA);
    void set_led2_pa(max30102_led_cr_t led_cr_mA);
    void set_pilot_led_pa(max30102_led_cr_t led_cr_mA);

    void update_temperature();

    int i2c_write(max30102_registers_t write_register, const char *to_write, const int size);
    int i2c_read(max30102_registers_t read_from, char *read_buffer, const int size);
};