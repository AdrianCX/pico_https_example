/* MIT License

Copyright (c) 2024 Adrian Cruceru - https://github.com/AdrianCX/pico_https

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <string.h>
#include <time.h>

#include "pico/stdio.h"
#include "pico/cyw43_arch.h"

#include "tls_listener.h"
#include "listener.h"
#include "request_handler.h"

#include "wifi.h"
#include "pico_logger.h"

#include <malloc.h>
#include "lwip/stats.h"
#include "pico/platform.h"

#include "lwip/timeouts.h"
int main() {
    mallopt(M_TRIM_THRESHOLD, 1024);
    
    stdio_init_all();

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_NETHERLANDS)) {
        printf("failed to initialise\n");
        return 1;
    }
    cyw43_pm_value(CYW43_PM2_POWERSAVE_MODE, 20, 1, 1, 1);
    
    cyw43_arch_enable_sta_mode();
    cyw43_arch_disable_ap_mode();

    int err_code = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000);

    if (err_code != 0)
    {
        for (int i=0;i<5;++i)
        {
            printf("[%d] failed to connect to wifi, ssid[%s], password[%s] err[%d], restarting in %d seconds\n", i, WIFI_SSID, WIFI_PASSWORD, err_code, 5-i);
            sleep_ms(1000);
        }

        #define AIRCR_Register (*((volatile uint32_t*)(PPB_BASE + 0x0ED0C)))
        AIRCR_Register = 0x5FA0004;
        return 1;
    }
    
    start_logging_server();
    
    trace("Starting");
    
    TLSListener *client = new TLSListener();
    client->listen(443, RequestHandler::create);

    uint32_t start = to_us_since_boot(get_absolute_time())/1000;
    
    while (true) {
        cyw43_arch_poll();
        sys_check_timeouts();

        uint32_t now = to_us_since_boot(get_absolute_time())/1000;
        if (now - start > 30000)
        {
            start = now;

            struct mallinfo m = mallinfo();
            
            trace("meminfo[arena=%d, hblkhd=%d, uordblks=%d, usmblks=%d, fordblks=%d, fsmblks=%d, keepcost=%d], numSessions=%d, lwip[avail=%d, used=%d, max=%d, err=%d, illegal=%d] tcp[xmit=%d, recv=%d, fw=%d, drop=%d, chkerr=%d, lenerr=%d, memerr=%d, rterr=%d, proterr=%d, opterr=%d, err=%d, cachehit=%d] udp[xmit=%d, recv=%d, fw=%d, drop=%d, chkerr=%d, lenerr=%d, memerr=%d, rterr=%d, proterr=%d, opterr=%d, err=%d, cachehit=%d]",
                  (uint32_t)m.arena, (uint32_t)m.hblkhd, (uint32_t)m.uordblks, (uint32_t)m.usmblks, (uint32_t)m.fordblks, (uint32_t)m.fsmblks, (uint32_t)m.keepcost, Session::getNumSessions(), (uint32_t)lwip_stats.mem.avail, (uint32_t)lwip_stats.mem.used, (uint32_t)lwip_stats.mem.max, (uint32_t)lwip_stats.mem.err, (uint32_t)lwip_stats.mem.illegal,
                  (uint32_t)lwip_stats.tcp.xmit, (uint32_t)lwip_stats.tcp.recv, (uint32_t)lwip_stats.tcp.fw, (uint32_t)lwip_stats.tcp.drop,
                  (uint32_t)lwip_stats.tcp.chkerr, (uint32_t)lwip_stats.tcp.lenerr, (uint32_t)lwip_stats.tcp.memerr, (uint32_t)lwip_stats.tcp.rterr,
                  (uint32_t)lwip_stats.tcp.proterr, (uint32_t)lwip_stats.tcp.opterr, (uint32_t)lwip_stats.tcp.err, (uint32_t)lwip_stats.tcp.cachehit,
                  (uint32_t)lwip_stats.udp.xmit, (uint32_t)lwip_stats.udp.recv, (uint32_t)lwip_stats.udp.fw, (uint32_t)lwip_stats.udp.drop,
                  (uint32_t)lwip_stats.udp.chkerr, (uint32_t)lwip_stats.udp.lenerr, (uint32_t)lwip_stats.udp.memerr, (uint32_t)lwip_stats.udp.rterr,
                  (uint32_t)lwip_stats.udp.proterr, (uint32_t)lwip_stats.udp.opterr, (uint32_t)lwip_stats.udp.err, (uint32_t)lwip_stats.udp.cachehit);
        }
    }

    cyw43_arch_deinit();
    return 0;
}

