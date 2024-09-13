#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

#include "lwipopts_common.h"

/* TCP WND must be at least 16 kb to match TLS record size
   or you will get a warning "altcp_tls: TCP_WND is smaller than the RX decrypion buffer, connection RX might stall!" */
#undef TCP_WND
#define TCP_WND  16384

#define LWIP_ALTCP               1
#define LWIP_ALTCP_TLS           1
#define LWIP_ALTCP_TLS_MBEDTLS   1
#define ALTCP_MBEDTLS_USE_SESSION_TICKETS 1
#define ALTCP_MBEDTLS_USE_SESSION_CACHE 1

#define LWIP_DEBUG 0
#define ALTCP_MBEDTLS_DEBUG  LWIP_DBG_OFF
#define PICO_TLS_DEBUG LWIP_DBG_OFF
#define LWIP_MBEDTLSDIR PICO_MBEDTLS_PATH

#define LWIP_NOASSERT

#endif

