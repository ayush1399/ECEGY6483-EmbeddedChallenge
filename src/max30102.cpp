#include <max30102.h>

MAX30102::MAX30102(PinName SDA, PinName SCL) : i2c(SDA, SCL)
{
    reset();

    char read_buffer[1];

    i2c_read(MAX30102_INTR_ENABLE_1, read_buffer, 1);
    intr_enable_1 = read_buffer[0];
    i2c_read(MAX30102_INTR_ENABLE_2, read_buffer, 1);
    intr_enable_2 = read_buffer[0];
    setup_intr(true, true, true, true);

    i2c_read(MAX30102_SPO2_CONFIG, read_buffer, 1);
    spo2_config = read_buffer[0];
    // setup_spo2();

    // char write_buffer[1];
    // write_buffer[0] = REG_INTR_STATUS_1;
    // i2c.write(MAX30102_WRITE_ADDRESS, write_buffer, 1, true);
    // char read_buffer[1];
    // i2c.read(MAX30102_READ_ADDRESS, read_buffer, 1);

    setup();
}

void MAX30102::getPartId()
{
    char read_buffer[1];
    int read = i2c_read(MAX30102_PART_ID, read_buffer, 1);

    print("Success: %d \t Part ID: %d\n", read, *read_buffer);
}

void MAX30102::getRevisionId()
{
    char read_buffer[1];
    int read = i2c_read(MAX30102_REVISION_ID, read_buffer, 1);

    print("Success: %d \t Revision ID: %d\n", read, *read_buffer);
}

void MAX30102::shutdown()
{
    set_mode(MAX30102_MODE_SHUTDOWN);
}

void MAX30102::reset()
{
    set_mode(MAX30102_MODE_RESET);
}

void MAX30102::setup()
{
}

void MAX30102::set_mode(max30102_mode_t mode)
{
    char to_write[1];
    to_write[0] = mode;

    i2c_write(MAX30102_MODE_CONFIG, to_write, 1);
}

void MAX30102::set_spo2_adc_rge(max30102_spo2_adc_range_t adc_range)
{
    spo2_config |= adc_range << 5;
};

void MAX30102::set_spo2_sr(max30102_spo2_sample_rate_t spo2_sr)
{
    spo2_config |= spo2_sr << 2;
};

void MAX30102::set_spo2_led_pw(max30102_adc_resolution_t led_pw)
{
    spo2_config |= led_pw;
};

void MAX30102::setup_spo2(max30102_spo2_adc_range_t spo2_adc_range, max30102_spo2_sample_rate_t spo2_sr, max30102_adc_resolution_t spo2_led_pw)
{
    set_spo2_adc_rge(spo2_adc_range);
    set_spo2_sr(spo2_sr);
    set_spo2_led_pw(spo2_led_pw);

    char write_buffer[1];
    write_buffer[0] = spo2_config;
    i2c_write(MAX30102_SPO2_CONFIG, write_buffer, 1);
}

void MAX30102::setup_hr()
{
}

void MAX30102::setup_intr(bool enable_a_full, bool enable_ppg_rdy, bool enable_alc_ovf, bool enable_die_temp_rdy)
{
    if (enable_a_full)
    {
        enable_intr_a_full();
    }
    else
    {
        disable_intr_a_full();
    }

    if (enable_ppg_rdy)
    {
        enable_intr_ppg_rdy();
    }
    else
    {
        disable_intr_ppg_rdy();
    }

    if (enable_alc_ovf)
    {
        enable_intr_alc_ovf();
    }
    else
    {
        disable_intr_alc_ovf();
    }

    if (enable_die_temp_rdy)
    {
        enable_intr_die_temp_rdy();
    }
    else
    {
        disable_intr_die_temp_rdy();
    }

    char write_buffer[1];

    write_buffer[0] = intr_enable_1;
    i2c_write(MAX30102_INTR_ENABLE_1, write_buffer, 1);

    write_buffer[0] = intr_enable_2;
    i2c_write(MAX30102_INTR_ENABLE_2, write_buffer, 1);
}

void MAX30102::enable_intr_a_full()
{
    intr_enable_1 |= MAX30102_A_FULL;
};
void MAX30102::disable_intr_a_full()
{
    intr_enable_1 &= ~MAX30102_A_FULL;
};
void MAX30102::enable_intr_ppg_rdy()
{
    intr_enable_1 |= MAX30102_PPG_RDY;
};
void MAX30102::disable_intr_ppg_rdy()
{
    intr_enable_1 &= ~MAX30102_PPG_RDY;
};
void MAX30102::enable_intr_alc_ovf()
{
    intr_enable_1 |= MAX30102_ALC_OVF;
};
void MAX30102::disable_intr_alc_ovf()
{
    intr_enable_1 &= ~MAX30102_ALC_OVF;
};
void MAX30102::enable_intr_die_temp_rdy()
{
    intr_enable_2 |= MAX30102_DIE_TEMP_RDY;
};
void MAX30102::disable_intr_die_temp_rdy()
{
    intr_enable_2 &= ~MAX30102_DIE_TEMP_RDY;
};

void MAX30102::printTemperature()
{
    int t = (int)(die_temp * 100);

    print("Temperature (in Celsius): %d.%d\n", t / 100, t % 100);
}

float MAX30102::getDieTemp()
{
    get_temperature();
    return die_temp;
};

void MAX30102::get_temperature()
{
    char write_buf[1];
    write_buf[0] = MAX30102_DIE_TEMP_STATUS_ENABLED;
    i2c_write(MAX30102_DIE_TEMP_CONFIG, write_buf, 1);

    char read_buf[1];
    do
    {
        i2c_read(MAX30102_INTR_STATUS_2, read_buf, 1);
        if ((uint8_t)(*read_buf) & MAX30102_DIE_TEMP_RDY)
        {
            break;
        }
        thread_sleep_for(100);
    } while (true);

    i2c_read(MAX30102_DIE_TEMP_INT, read_buf, 1);
    int8_t temp_int = (int8_t)(*read_buf);

    i2c_read(MAX30102_DIE_TEMP_FRAC, read_buf, 1);
    uint8_t temp_frac = (uint8_t)(*read_buf);

    die_temp = (float)temp_int + (float)temp_frac * 0.0625;
}

int MAX30102::i2c_write(max30102_registers_t write_register, const char *to_write, const int size)
{
    char write_buffer[size + 1];
    write_buffer[0] = write_register;
    memcpy(write_buffer + 1, to_write, size);

    int ret = i2c.write(MAX30102_WRITE_ADDRESS, write_buffer, size + 1);
    return ret;
}

int MAX30102::i2c_read(max30102_registers_t read_register, char *read_buffer, const int size)
{
    char write_buffer[1];
    write_buffer[0] = read_register;
    int ret = i2c.write(MAX30102_WRITE_ADDRESS, write_buffer, 1, true);
    if (ret)
    {
        return ret;
    }

    ret = i2c.read(MAX30102_READ_ADDRESS, read_buffer, size);
    return ret;
}