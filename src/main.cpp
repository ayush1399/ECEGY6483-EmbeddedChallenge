#include <mbed.h>
// this file has all the functions for interacting
// with the screen
#include "drivers/LCD_DISCO_F429ZI.h"
#define BACKGROUND 1
#define FOREGROUND 0
#define GRAPH_PADDING 5

LCD_DISCO_F429ZI lcd;
// buffer for holding displayed text strings
char display_buf[3][60];

// sets the background layer
// to be visible, transparent, and
// resets its colors to all black
void setup_background_layer()
{
  lcd.SelectLayer(BACKGROUND);
  lcd.Clear(LCD_COLOR_BLACK);
  lcd.SetBackColor(LCD_COLOR_BLACK);
  lcd.SetTextColor(LCD_COLOR_BLUE);
  lcd.SetLayerVisible(BACKGROUND, ENABLE);
  lcd.SetTransparency(BACKGROUND, 0x7Fu);
}

// resets the foreground layer to
// all black
void setup_foreground_layer()
{
  lcd.SelectLayer(FOREGROUND);
  lcd.Clear(LCD_COLOR_BLACK);
  lcd.SetBackColor(LCD_COLOR_WHITE);
  lcd.SetTextColor(LCD_COLOR_DARKBLUE);
}

// Documents
// Manual for dev board: https://www.st.com/resource/en/user_manual/um1670-discovery-kit-with-stm32f429zi-mcu-stmicroelectronics.pdf
// gyroscope datasheet: https://www.mouser.com/datasheet/2/389/dm00168691-1798633.pdf

SPI spi(PF_9, PF_8, PF_7, PC_1, use_gpio_ssel); // mosi, miso, sclk, cs

// address of first register with gyro data
#define OUT_X_L 0x28

// register fields(bits): data_rate(2),Bandwidth(2),Power_down(1),Zen(1),Yen(1),Xen(1)
#define CTRL_REG1 0x20

// configuration: 200Hz ODR,50Hz cutoff, Power on, Z on, Y on, X on
#define CTRL_REG1_CONFIG 0b01'10'1'1'1'1

// register fields(bits): reserved(1), endian-ness(1),Full scale sel(2), reserved(1),self-test(2), SPI mode(1)
#define CTRL_REG4 0x23

// configuration: reserved,little endian,500 dps,reserved,disabled,4-wire mode
#define CTRL_REG4_CONFIG 0b0'0'01'0'00'0

#define SPI_FLAG 1

uint8_t write_buf[32];
uint8_t read_buf[32];

EventFlags flags;
// The spi.transfer() function requires that the callback
// provided to it takes an int parameter
void spi_cb(int event)
{
  flags.set(SPI_FLAG);
}

int main()
{
  // Setup the spi for 8 bit data, high steady state clock,
  // second edge capture, with a 1MHz clock rate
  spi.format(8, 3);
  spi.frequency(1'000'000);

  write_buf[0] = CTRL_REG1;
  write_buf[1] = CTRL_REG1_CONFIG;
  spi.transfer(write_buf, 2, read_buf, 2, spi_cb, SPI_EVENT_COMPLETE);
  flags.wait_all(SPI_FLAG);

  write_buf[0] = CTRL_REG4;
  write_buf[1] = CTRL_REG4_CONFIG;
  spi.transfer(write_buf, 2, read_buf, 2, spi_cb, SPI_EVENT_COMPLETE);
  flags.wait_all(SPI_FLAG);

  setup_background_layer();
  setup_foreground_layer();

  while (1)
  {
    int16_t raw_gx, raw_gy, raw_gz;
    float gx, gy, gz;
    // prepare the write buffer to trigger a sequential read
    write_buf[0] = OUT_X_L | 0x80 | 0x40;
    // start sequential sample reading
    spi.transfer(write_buf, 7, read_buf, 7, spi_cb, SPI_EVENT_COMPLETE);
    flags.wait_all(SPI_FLAG);
    // read_buf after transfer: garbage byte, gx_low,gx_high,gy_low,gy_high,gz_low,gz_high
    // Put the high and low bytes in the correct order lowB,HighB -> HighB,LowB
    raw_gx = (((uint16_t)read_buf[2]) << 8) | ((uint16_t)read_buf[1]);
    raw_gy = (((uint16_t)read_buf[4]) << 8) | ((uint16_t)read_buf[3]);
    raw_gz = (((uint16_t)read_buf[6]) << 8) | ((uint16_t)read_buf[5]);

    // printf("RAW|\tgx: %d \t gy: %d \t gz: %d\t",raw_gx,raw_gy,raw_gz);

    gx = ((float)raw_gx) * (17.5f * 0.017453292519943295769236907684886f / 1000.0f);
    gy = ((float)raw_gy) * (17.5f * 0.017453292519943295769236907684886f / 1000.0f);
    gz = ((float)raw_gz) * (17.5f * 0.017453292519943295769236907684886f / 1000.0f);
    printf("Actual|\tgx: %4.5f \t gy: %4.5f \t gz: %4.5f\n", gx, gy, gz);

    lcd.SelectLayer(FOREGROUND);

    snprintf(display_buf[0], 60, "gx: %4.5f", gx);
    lcd.DisplayStringAt(0, LINE(17), (uint8_t *)display_buf[0], LEFT_MODE);

    snprintf(display_buf[1], 60, "gy: %4.5f", gy);
    lcd.DisplayStringAt(0, LINE(18), (uint8_t *)display_buf[1], LEFT_MODE);

    snprintf(display_buf[2], 60, "gz: %4.5f", gz);
    lcd.DisplayStringAt(0, LINE(19), (uint8_t *)display_buf[1], LEFT_MODE);

    thread_sleep_for(100);
  }
}
//*/

// // DON'T COMMENT OUT THE ABOVE CODE

// ///*//START OF EXAMPLE 1---------------------------------------------
// int main()
// {
//   setup_background_layer();

//   setup_foreground_layer();

//   // creates c-strings in the display buffers, in preparation
//   // for displaying them on the screen
//   snprintf(display_buf[0], 60, "width: %d pixels", lcd.GetXSize());
//   snprintf(display_buf[1], 60, "height: %d pixels", lcd.GetYSize());
//   lcd.SelectLayer(FOREGROUND);
//   // display the buffered string on the screen
//   lcd.DisplayStringAt(0, LINE(18), (uint8_t *)display_buf[1], LEFT_MODE);
//   lcd.DisplayStringAt(0, LINE(19), (uint8_t *)display_buf[0], LEFT_MODE);

//   // draw the graph window on the background layer
//   //  with x-axis tick marks every 10 pixels
//   // draw_graph_window(10);

//   lcd.SelectLayer(FOREGROUND);
//   while (1)
//   {
//     for (int j = 0; j < 235; j++)
//     {

//       // loop through each column of the graph, and update the data,
//       // erasing the old points and drawing the new points, one pixel at a time.
//       for (unsigned int i = 0; i < graph_width; i++)
//       {
//         // the x coordinate of the graph value being updated.
//         // think about it like a vertical line
//         // that sweeps across the graph from left to right,
//         // updating each point in the graph as it travels across.
//         const uint32_t target_x_coord = GRAPH_PADDING + i;
//         // previous iterations function value at the target x coordinate
//         const float last_function_val = sin((i + j - 1) * 0.1) + 1.0;
//         // current iterations function value at the target x coordinate
//         const float current_function_val = sin((i + j) * 0.1) + 1.0;
//         // y coordinate of the previous function value at the target x coordinate
//         const uint32_t old_pixelY = mapPixelY(last_function_val, 0, 2, GRAPH_PADDING, GRAPH_PADDING + graph_height);
//         // y coordinate of the current function value at the target x coordinate
//         const uint32_t new_pixelY = mapPixelY(current_function_val, 0, 2, GRAPH_PADDING, GRAPH_PADDING + graph_height);
//         // remove (set to black) the old pixel for the current x coordinate
//         // from the screen
//         lcd.DrawPixel(target_x_coord, old_pixelY, LCD_COLOR_BLACK);
//         // draw the new pixel for the current x coordinate on the screen
//         lcd.DrawPixel(target_x_coord, new_pixelY, LCD_COLOR_BLUE);
//       }
//       // delay to control update speed
//       thread_sleep_for(10);
//     }
//     for (int i = 0; i < 230; i++)
//     {
//       // remove old pixels after the last iteration
//       uint32_t old_pixel = mapPixelY(sin((i + 234) * 0.1) + 1, -1, 1, 5, 235);
//       lcd.DrawPixel(5 + i, old_pixel, LCD_COLOR_BLACK);
//     }
//   }
// }