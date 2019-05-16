#ifndef _PROTOSPI_H
#define _PROTOSPI_H

void PROTO_CS_LO(u8 radio);
void PROTO_CS_HI(u8 radio);
void PROTOSPI_xfer(u8 byte);
u8 PROTOSPI_read3wire(void);

#endif
