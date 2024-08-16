#if !defined(UNICODE_CHAR_FRAMEWORK_H)
#define UNICODE_CHAR_FRAMEWORK_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void run_tests(void);

typedef struct UnicodeChar unicode_char_t;
typedef struct ByteBuffer bytebuffer_t;
typedef struct UnicodeTable unicode_table_t;

#define UNICODE_MAX_BYTES 4
#define UNICODE_UTF_8_MAX_BYTES 6
#define UNICODE_ALPHA_IN_BYTE 2
#define UNICODE_BUFF_SM 16
#define UNICODE_BUFF_MD 64
#define UNICODE_FILE_BUFF 4096

#define BYTE_NEWLINE 0xA
#define BYTE_EOF 0x4

typedef enum EncodingType
{
    NO_ENCODING,
    KOI_8_R,
    ISO_8859_5,
    CP1251,
    NUM_ENCODING

} encoding_type_t;

typedef enum DecodingStatus
{
    DECODE_OK,
    DECODE_FAILED

} decoding_status_t;

#endif // UNICODE_CHAR_FRAMEWORK_H
