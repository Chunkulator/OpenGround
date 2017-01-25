/*
    Copyright 2016 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/ or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

    author: fishpepper <AT> gmail.com
*/

#include <stdint.h>

#include "debug.h"
#include "console.h"
#include "led.h"
#include "delay.h"
#include "screen.h"

void assert_failed(uint8_t* filename, uint32_t line) {
    // report position
    if (debug_is_initialized()) {
        debug("!ASSERT: line ");
        debug_put_uint16(line);
        debug("\nin ");
        debug(filename);
        debug_put_newline();
        debug_flush();
        console_render();
        screen_update();
    }

    // infinite loop
    led_button_r_off();
    led_button_l_on();
    while (1) {
        led_button_r_toggle();
        led_button_l_toggle();
        delay_us(50*1000);
    }
}

