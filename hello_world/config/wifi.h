#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

//#define WIFI_SSID "ssid"
//#define WIFI_PASSWORD "password"

#if !defined(WIFI_SSID) || !defined(WIFI_PASSWORD)
#error "Please define WIFI_SSID/WIFI_PASSWORD in this file" 
#endif


#endif //WIFI_CONFIG_H