/* half-duplex(one-wire) semi-soft AVR serial communication routines */
#ifndef SEMI_SOFT_SERIAL_H
#define SEMI_SOFT_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HDSS_FUNC_PREFIX
  #define _JOIN_NAME(prefix, body) prefix##body
  #define JOIN_NAME(prefix, body) _JOIN_NAME(prefix, body)
  #define HDSS_INITIATOR_INIT JOIN_NAME(HDSS_FUNC_PREFIX,_initiator_init)
  #define HDSS_SEND_BYTES JOIN_NAME(HDSS_FUNC_PREFIX,_send_bytes)
#else
  #define HDSS_INITIATOR_INIT hdss_initiator_init
  #define HDSS_SEND_BYTES hdss_send_bytes
#endif

void HDSS_INITIATOR_INIT(void);
void hdss_responder_init(void);
void HDSS_SEND_BYTES(const uint8_t *datap, uint16_t datalen, _Bool change_receiver);

#ifdef __cplusplus
}
#endif

#endif /* SEMI_SOFT_SERIAL_H */
