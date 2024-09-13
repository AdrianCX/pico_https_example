# Simple Websocket/HTTPS/TLS server on raspberry pi pico.


Goal here is to have a simple build environment and libraries to create your own TLS server.
A simple 'hello world' is provided as example that can handle simple HTTPS requests and WebSocket clients.

### How to compile:

1. Create certificates or insert your own.
```
cd hello_world/config/certificate
```

Either RSA or EC
```
./create_cert_rsa.sh
./create_cert_ec.sh
```

This generates a self signed certificate in key.h/cert.h.
Alternative is to create a 'certificate' folder with same file format anywhere else in include path.


2. Configure wifi in example (otherwise it won't connect):
```
hello_world/config/wifi.h
```

3. Fetch submodules

```
git submodule update --init --recursive
```

4. Build

```
./build_via_docker.sh
```

Result is: build/docker/hello_world/hello_world_https.uf2

### Logs

There are two different types of logs.

1. Application logs

These are sent over UDP.
Configure a host in "config/logging_config.h"
```
#define LOGGING_SERVER "192.168.100.150"
#define LOGGING_SERVER_PORT 21000
```

and on the remote host:
```
netcat -ukl 21000
```

If it crashes you will see something like this
```
 11    10619 Undef 16-bit instr sr = 0x6100002e
 12    10619 R0  = 0x00000001  R8  = 0x20005b7c
 13    10619 R1  = 0x00000001  R9  = 0x2000dfe8
 14    10619 R2  = 0x2000f670  R10 = 0x000001a6
 15    10619 R3  = 0x00000000  R11 = 0x2000fe68
 16    10619 R4  = 0x2000dfd8  R12 = 0x2000fe5c
 17    10619 R5  = 0x00000000  SP  = 0x20041da0
 18    10620 R6  = 0x20019398  LR  = 0x2000fe68
 19    10620 R7  = 0x20005b7c  PC  = 0x1000064c
 20    10620  -> undef instr16: 0xdeff
 21    10620 crashed at: 0x1000064c
```

using gdb and addresses you can see line where issue occurs:
```
# gdb ./build_docker/hello_world/hello_world.elf

(gdb) info line *0x1000064c
Line 302 of "/source/hello_world/http_session.cpp" starts at address 0x10000648 <_ZN11HTTPSession7on_recvEPhj+20> and ends at 0x10000654 <_ZN11HTTPSessionD2Ev>.
```


2. Other logs (lwip/etc):

You can grab those via:
```
while true; do minicom -b 115200 -o -D /dev/ttyACM0; done
```



3. To enable logs

3.1. If you want full verbose mbedtls logs with packets, add the following
```
hello_world/config/mbedtls_config.h
```
```
#define MBEDTLS_DEBUG_C
```

3.2. If you want pico-tls/altcp mbedtls logs
```
hello_world/config/lwipopts.h
```
```
#define LWIP_DEBUG 1
#define ALTCP_MBEDTLS_DEBUG  LWIP_DBG_ON
#define PICO_TLS_DEBUG LWIP_DBG_ON
```

3.3. If you want TCP stack logs

Change any of the LWIP_DBG_OFF to LWIP_DBG_ON that you are interested in file below.
Use a build that doesn't have NDEBUG defined.

```
hello_world/config/lwipopts_common.h
```