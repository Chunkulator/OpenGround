#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <stdint.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

enum Radio
{
    A7105
};

enum ProtoCmds {
    PROTOCMD_INIT,
    PROTOCMD_DEINIT,
    PROTOCMD_BIND,
    PROTOCMD_CHECK_AUTOBIND,
    PROTOCMD_NUMCHAN,
    PROTOCMD_DEFAULT_NUMCHAN,
    PROTOCMD_CURRENT_ID,
    PROTOCMD_GETOPTIONS,
    PROTOCMD_SETOPTIONS,
    PROTOCMD_TELEMETRYSTATE,
    PROTOCMD_TELEMETRYTYPE,
    PROTOCMD_TELEMETRYRESET,
    PROTOCMD_RESET,
    PROTOCMD_CHANNELMAP,
};

enum TXRX_State {
    TXRX_OFF,
    TX_EN,
    RX_EN,
};

enum PinConfigState {
    CSN_PIN,
    ENABLED_PIN,
    DISABLED_PIN,
    RESET_PIN,
};

#define AETRG (0)
#define UNCHG (1)
#define EATRG (2)
#define TAERG (3)

#define PROTODEF(proto, module, map, cmd, name) proto,
enum protocol_id
{
    PROTOCOL_NONE,
    #include "protocol.h" 
    PROTOCOL_COUNT
};
#undef PROTODEF

#define PROTODEF(proto, module, map, cmd, name) extern uintptr_t cmd(enum ProtoCmds);
#include "protocol.h"
#undef PROTODEF

#include "iface_a7105.h"

#endif //_INTERFACE_H_
