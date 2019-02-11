/*
 * The Tinusaur Project
 *
 * TinuDHT - Tinusaur DHT11 Library
 *
 * @created: 2015-01-01
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/tinudht
 *
 */

// ============================================================================

#define F_CPU 1000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

// ======================================
//                ATtiny
//               25/45/85
//              +----------+
//        RST --+ PB5  Vcc +---(+)-------
// ----OWOWOD---+ PB3  PB2 +-- 
// -DHT11DATA---+ PB4  PB1 +-- 
// -------(-)---+ GND  PB0 +-- 
//              +----------+
// ======================================

#define OWOWOD_PORT	PB3	// OWOWOD Port
#define TINUDHT_PIN PB4

// ----------------------------------------------------------------------------

#include "../owowod/owowod.h"
#include "../owowod/debugging.h"
#include "../tinudht/tinudht.h"

// ============================================================================

int main(void) {

	// ---- Initialization ----
	DEBUGGING_INIT();
	DEBUGGING_STRING("TinuDHT Test\n\r");

	_delay_ms(100);	// Delay for DHT11 to stabilize (REQUIRED on power on)

	// ---- Main Loop ----
	uint8_t num = 0;
	while (1) {
		DEBUGGING_NUMDECUP(num);

		// --------------------------------------------------------------------

		// Retrieve the data from the DHT11

		TinuDHT tinudht;
		uint8_t tinudht_result = tinudht_read(&tinudht, TINUDHT_PIN);
		if (tinudht_result == TINUDHT_OK) {
			// Output the results

			DEBUGGING_STRING(" H:");
			DEBUGGING_NUMDECP(tinudht.humidity);
			DEBUGGING_STRING(" %");

			DEBUGGING_STRING(" T:");
			DEBUGGING_NUMDECP(tinudht.temperature);
			DEBUGGING_STRING(" C");

		} else {
			DEBUGGING_STRING(" ERROR ");
			DEBUGGING_NUMDECUP(tinudht_result);
		}

		// --------------------------------------------------------------------

		DEBUGGING_STRING("\n\r");
		_delay_ms(1000);
		// IMPORTANT: Do not query the DHT11 more often than 1 time per second.
		num++;
	}
	
	return (0);
}

// ============================================================================

