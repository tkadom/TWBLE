/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "ble_app_gzll_ui.h"
#include "app_error.h"
#include "app_timer.h"
#include "ble_app_gzll_common.h"

#define BUTTON_DETECTION_DELAY  APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)    /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */


/**@brief Function for handling bsp events.
 */
static void bsp_evt_handler(bsp_event_t evt)
{
    switch (evt)
    {
        case BSP_EVENT_KEY_0:
                        running_mode = BLE;
            break;

        case BSP_EVENT_KEY_1:
                        running_mode = GAZELL;
            break;

        default:
                        APP_ERROR_HANDLER((uint8_t)evt);
            break;
    }
}


/**@brief Function for initializing bsp module.
 */
void bsp_init_app(void)
{
        uint32_t err_code;
        err_code = bsp_init(BSP_INIT_LED | BSP_INIT_BUTTONS, BUTTON_DETECTION_DELAY, bsp_evt_handler);
        APP_ERROR_CHECK(err_code);
        err_code = bsp_event_to_button_assign(BLE_BUTTON_ID, BSP_EVENT_KEY_0);
        APP_ERROR_CHECK(err_code);
        err_code = bsp_event_to_button_assign(GZLL_BUTTON_ID, BSP_EVENT_KEY_1);
        APP_ERROR_CHECK(err_code);
        err_code = bsp_buttons_enable((1 << BLE_BUTTON_ID) | (1 << GZLL_BUTTON_ID));
        APP_ERROR_CHECK(err_code);
}
