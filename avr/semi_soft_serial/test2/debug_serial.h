#include <stdint.h>
#include <stdbool.h>
#define SOFT_TX_PORT D,3  // TXO
#define HDSS_FUNC_PREFIX debug_print
#undef PARITY_ENABLE
void debug_print_initiator_init(void);
void debug_print_send_bytes(const uint8_t *datap, uint16_t datalen, bool change_receiver);
