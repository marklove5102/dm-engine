// v3.01
 #define CRC_CHECK_BEGIN \
 __asm _emit 0xEB \
 __asm _emit 0x04 \
 __asm _emit 0xEB \
 __asm _emit 0x05 \
 __asm _emit 0x19 \
 __asm _emit 0x01

 #define CRC_CHECK_END \
 __asm _emit 0xEB \
 __asm _emit 0x04 \
 __asm _emit 0xEB \
 __asm _emit 0x05 \
 __asm _emit 0x29 \
 __asm _emit 0x01

 #define CRC_BUFFER_BEGIN \
 __asm _emit 0xE9 \
 __asm _emit 0x25 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00

 #define CRC_BUFFER_END \
 __asm _emit 0xE9 \
 __asm _emit 0x85 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00

 #define ENVELOPE_CHECK \
 __asm _emit 0xEB \
 __asm _emit 0x04 \
 __asm _emit 0xEB \
 __asm _emit 0x05 \
 __asm _emit 0x97 \
 __asm _emit 0x01

 #define ENVELOPE_CHECK_FUNC \
 __asm _emit 0xEB \
 __asm _emit 0x04 \
 __asm _emit 0xEB \
 __asm _emit 0x05 \
 __asm _emit 0x97 \
 __asm _emit 0x02

 #define USER_POLYBUFFER \
 __asm _emit 0xEB \
 __asm _emit 0x04 \
 __asm _emit 0xEB \
 __asm _emit 0x05 \
 __asm _emit 0x39 \
 __asm _emit 0x19

 #define ENVELOPE_CHECK_BUFFER \
 __asm _emit 0xE9 \
 __asm _emit 0x05 \
 __asm _emit 0x01 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00

 #define ENVELOPE_CHECK_FUNC_BUFFER \
 __asm _emit 0x33 \
 __asm _emit 0xC0 \
 __asm _emit 0xE9 \
 __asm _emit 0x03 \
 __asm _emit 0x01 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00 \
 __asm _emit 0x00

 #define DEFAULT_BUFFER \
 __asm _emit 0x0 __asm _emit 0x0 __asm _emit 0x0 __asm _emit 0x0 \
 __asm _emit 0x0 __asm _emit 0x0 __asm _emit 0x0 __asm _emit 0x0

