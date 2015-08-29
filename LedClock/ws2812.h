#ifndef WS2812_H_
#define WS2812_H_
#include <stdint.h>
// Useful typedefs
typedef unsigned char u_char;		// 8 bit
typedef unsigned int u_int;			// 16 bit

// Transmit codes
#define HIGH_CODE	(0xF0)			// b11110000
#define LOW_CODE	(0xC0)			// b11000000

#define NUM_LEDS	(60)			// NUMBER OF LEDS IN YOUR STRIP

// Configure processor to output to data strip
void initStrip(uint8_t offset);

// Send colors to the strip and show them. Disables interrupts while processing.
void showStrip(void);

// Set the color of a certain LED
void setLEDColor(u_int p, u_char r, u_char g, u_char b);

// Clear the color of all LEDs (make them black/off)
void clearStrip(void);

// Fill the strip with a solid color. This will update the strip.
void fillStrip(u_char r, u_char g, u_char b);

typedef struct {
	u_char red;
	u_char green;
	u_char blue;
} LED;

#endif /* WS2812_H_ */
