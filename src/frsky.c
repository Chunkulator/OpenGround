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
    along with this program.  If not, see <http:// www.gnu.org/ licenses/>.

    author: fishpepper <AT> gmail.com
*/

#include "main.h"
#include <string.h>
#include <stdio.h>
#include "frsky.h"
#include "debug.h"
#include "timeout.h"
#include "led.h"
#include "delay.h"
#include "wdt.h"
#include "io.h"
#include "clocksource.h"
#include "storage.h"
#include "adc.h"
#include "telemetry.h"

#include <libopencm3/stm32/timer.h>

void frsky_init(void) {
    // uint8_t i;
    debug("frsky: init\n"); debug_flush();

    telemetry_init();

    frsky_init_timer();

    frsky_tx_set_enabled(1);

    debug("frsky: init done\n"); debug_flush();
}

void frsky_init_timer(void) {
    // TIM3 clock enable
    rcc_periph_clock_enable(RCC_TIM3);

    // init timer3 for 9ms
    timer_reset(TIM3);

    // enable the TIM3 gloabal Interrupt
    nvic_enable_irq(NVIC_TIM3_IRQ);
    nvic_set_priority(NVIC_TIM3_IRQ, NVIC_PRIO_FRSKY);

    // compute prescaler value
    // we want one ISR every 9ms
    // setting TIM_Period to 9000 will reuqire
    // a prescaler so that one timer tick is 1us (1MHz)
    uint16_t prescaler = (uint16_t) (rcc_timer_frequency  / 1000000) - 1;

    // time base as calculated above
    timer_set_prescaler(TIM3, prescaler);
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    // timer should count with 1MHz thus 9000 ticks = 9ms
    timer_set_period(TIM3, 9000-1);

    // DO NOT ENABLE INT yet!

    // enable timer
    timer_enable_counter(TIM3);
}

void frsky_tx_set_enabled(uint32_t enabled) {
    // TIM Interrupts enable? -> tx active
    if (enabled) {
        // enable ISR
        timer_enable_irq(TIM3, TIM_DIER_UIE);
    } else {
        // stop ISR
        timer_disable_irq(TIM3, TIM_DIER_UIE);
        // make sure last packet was sent
        delay_ms(20);
    }
}

void frsky_handle_telemetry(void) {
    // handle incoming telemetry data
    telemetry_process();
}

void frsky_get_rssi(uint8_t *rssi, uint8_t *rssi_telemetry) {
    *rssi           = 0;
    *rssi_telemetry = 0;
}

void TIM3_IRQHandler(void)
{
    if (timer_get_flag(TIM3, TIM_SR_UIF)){
        // clear flag (NOTE: this should never be done at the end of the ISR)
        timer_clear_flag(TIM3, TIM_SR_UIF);
    }
}

void frsky_send_bindpacket(uint8_t bind_packet_id) {
    (void) bind_packet_id;
}

uint8_t frsky_bind_jumper_set(void) {
    debug("frsky: BIND jumper set = "); debug_flush();
    if (0) {  // io_bind_request()) {
        debug("YES -> binding\n");
        return 1;
    } else {
        debug("NO -> no binding\n");
        return 0;
    }
}

void frsky_do_clone_prepare(void) {
    debug("frsky: do clone\n"); debug_flush();

    // set up leds:
    led_button_r_off();
    led_button_l_on();
}

void frsky_enter_bindmode(void) {
    debug("frsky: do bind\n"); debug_flush();

    // set up leds:
    led_button_r_on();
    led_button_l_off();
}

void frsky_do_clone_finish(void) {
    // save to persistant storage:
    storage_save();

    // done, end up in fancy blink code
    debug("frsky: finished binding. please reset\n");
    led_button_l_on();
}

void frsky_autotune_prepare(void) {
    debug("frsky: autotune\n"); debug_flush();

    debug("frsky: entering bind loop\n"); debug_flush();

    led_button_r_off();
}

uint32_t frsky_autotune_do(void) {
    // search for best fscal 0 match
    // reset wdt
    wdt_reset();

    return 1;
}


void frsky_autotune_finish(void) {
}


void frsky_enter_rxmode(uint8_t channel) {
    (void) channel;
}


void frsky_tune_channel(uint8_t ch) {
    (void) ch;
}

void frsky_fetch_txid_and_hoptable_prepare(void) {
}

uint32_t frsky_fetch_txid_and_hoptable_do(void) {
    // fetch hopdata array
    // reset wdt
    wdt_reset();
    return 1;
}

void frsky_fetch_txid_and_hoptable_finish(void) {
}

void frsky_calib_pll(void) {
}

void frsky_set_channel(uint8_t hop_index) {
    (void) hop_index;
}

void frsky_increment_channel(int8_t cnt) {
    (void) cnt;
}

uint8_t frsky_extract_rssi(uint8_t rssi_raw) {
#define FRSKY_RSSI_OFFSET 70
    if (rssi_raw >= 128) {
        // adapted to fit better to the original values... FIXME: find real formula
        // return(rssi_raw * 18)/ 32 - 82;
        return((((uint16_t)rssi_raw) * 18)>>5) - 82;
    } else {
        return((((uint16_t)rssi_raw) * 18)>>5) + 65;
    }
}

