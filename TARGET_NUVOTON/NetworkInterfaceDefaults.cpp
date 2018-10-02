/* Network interface defaults
 * Copyright (c) 2018-2020 Nuvoton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"

#define ETHERNET 1
#define WIFI 2
#define MESH 3
#define CELLULAR 4

#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == ETHERNET

#include "EthInterface.h"

EthInterface *EthInterface::get_default_instance()
{
    return get_target_default_instance();
}

NetworkInterface *NetworkInterface::get_default_instance()
{
    return EthInterface::get_default_instance();
}

#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI
#include "WiFiInterface.h"
#include "ESP8266Interface.h"

#define ESP8266_AT_ONBOARD      1   // On-board ESP8266
#define ESP8266_AT_EXTERN       2   // External ESp8266 through UNO D1/D0

#ifndef ESP8266_AT_SEL
#error("ESP8266_AT_SEL missing. Select ESP8266 on-board/external.")
#endif

WiFiInterface *WiFiInterface::get_default_instance() {
    
#if ESP8266_AT_SEL == ESP8266_AT_ONBOARD
#   if TARGET_NUMAKER_IOT_M487
    static DigitalOut esp_rst(PH_3, 0);         // Reset button pressed
    static ESP8266Interface esp(PH_8, PH_9);

    if (! ((int) esp_rst)) {                    // Reset button released
        wait_ms(5);
        esp_rst = 1;
        wait_ms(5);
    }
#   elif TARGET_NUMAKER_PFM_M2351
    static DigitalIn esp_gpio0(PD_6);           // Go boot mode by default
                                                // User can change to F/W update mode by short'ing ESP8266 GPIO0/GND
                                                // before power-on
    static DigitalOut esp_pwr_off(PD_7, 1);     // Disable power to on-board ESP8266
    static ESP8266Interface esp(PD_1, PD_0);

    if ((int) esp_pwr_off) {                    // Turn on on-board ESP8266
        wait_ms(50);
        esp_pwr_off = 0;
        wait_ms(50);
    }
#   endif
#elif ESP8266_AT_SEL == ESP8266_AT_EXTERN
    static ESP8266Interface esp(D1, D0);
#endif

    return &esp;
}

NetworkInterface *NetworkInterface::get_default_instance()
{
    WiFiInterface *wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        return NULL;
    }

#define concat_(x,y) x##y
#define concat(x,y) concat_(x,y)
#define SECURITY concat(NSAPI_SECURITY_,MBED_CONF_NSAPI_DEFAULT_WIFI_SECURITY)
    wifi->set_credentials(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID, MBED_CONF_NSAPI_DEFAULT_WIFI_PASSWORD, SECURITY);
    return wifi;
}

#endif
