#if !defined(L_UNICODE_CHAR_H)
#define L_UNICODE_CHAR_H
#include "framework.h"
#include "utils.h"

struct UnicodeChar
{
    struct
    {
        uint8_t bytes[UNICODE_MAX_BYTES];
        uint8_t real_size;

    } unicode;
    struct
    {
        uint8_t bytes[UNICODE_UTF_8_MAX_BYTES];
        uint8_t real_size;

    } utf_8;

    struct
    {
        uint8_t value;
        uint8_t coords[2];
        encoding_type_t type;
    } table;
};

unicode_char_t *
uni_char_init();

unicode_char_t *
uni_char_set_from_line(unicode_char_t *self, const char *line, encoding_type_t table_type);

void uni_char_get_unicode_byte_sz(unicode_char_t *self);

void uni_char_get_utf_8_byte_sz(unicode_char_t *self);

void uni_char_print(unicode_char_t *self);

void uni_char_print_lite(unicode_char_t *self);

const char *
uni_char_encoding_name_by_type(encoding_type_t type);

const char *
uni_char_encoding_name(unicode_char_t *self);

// IMPL

unicode_char_t *uni_char_init()
{
    unicode_char_t *uni = (unicode_char_t *)malloc(sizeof(unicode_char_t));
    if (NULL == uni)
    {
        return NULL;
    }
    *uni = (unicode_char_t){0};

    return uni;
}

unicode_char_t *uni_char_set_from_line(unicode_char_t *self, const char *line, encoding_type_t table_type)
{
    // size_t line_sz = string_get_size(line);
    char table_val_str[UNICODE_BUFF_SM] = "";
    char unicode_str[UNICODE_BUFF_SM] = "";

    const char *delims = " \t#";

    size_t i = 0;
    while (string_char_is_in(line[i], delims))
        i++;

    // writing first element into table_val_str
    size_t j = 0;
    while (!string_char_is_in(line[i], delims))
    {
        table_val_str[j++] = line[i++];
    }
    while (string_char_is_in(line[i], delims))
        i++;

    j = 0;

    // writing second element into table_val_str
    while (!string_char_is_in(line[i], delims))
    {
        unicode_str[j++] = line[i++];
    }

    // mormalizing unicode str
    size_t unicode_str_len = string_get_size(unicode_str);
    size_t unicode_str_trimmed_len = UNICODE_MAX_BYTES * UNICODE_ALPHA_IN_BYTE;
    size_t leading_zeroes_len = unicode_str_trimmed_len - unicode_str_len;

    char unicode_str_trimmed[UNICODE_MAX_BYTES * UNICODE_ALPHA_IN_BYTE] = "";

    i = 0;
    j = 0;
    if (unicode_str_len > 2 && string_char_is_in(unicode_str[1], "xX"))
    {

        leading_zeroes_len += 2;
        j += 2;
    }

    for (; i != leading_zeroes_len; i++)
        unicode_str_trimmed[i] = '0';

    for (; j != unicode_str_trimmed_len; i++)

        unicode_str_trimmed[i] = unicode_str[j++];

    // unicode_str_trimmed should be OK at this point

    byte_utils_from_str_to_byte_arr(self->unicode.bytes, unicode_str_trimmed, unicode_str_trimmed_len);

    uni_char_get_unicode_byte_sz(self);

    // utf-8

    byte_utils_unicode_to_utf_8(self->unicode.bytes, self->utf_8.bytes);

    uni_char_get_utf_8_byte_sz(self);
    // filling table section

    self->table.value = strtol(table_val_str, NULL, 16);
    self->table.type = table_type;

    self->table.coords[0] = self->table.value >> 4;
    self->table.coords[1] = self->table.value & 0x0F;
    return self;
}

void uni_char_get_unicode_byte_sz(unicode_char_t *self)
{
    // exception for the null 0x0000
    if (self->unicode.bytes[UNICODE_MAX_BYTES - 1] == 0)
    {
        self->unicode.real_size = 1;
        return;
    }

    size_t i = 0;
    while (self->unicode.bytes[i] == 0)
        i++;
    self->unicode.real_size = (uint8_t)UNICODE_MAX_BYTES - i;
}

void uni_char_get_utf_8_byte_sz(unicode_char_t *self)
{
    // exception for the null 00 00 00 00 00 00
    if (self->utf_8.bytes[0] == 0)
    {
        self->unicode.real_size = 1;
        return;
    }

    size_t i = 0;
    while (self->utf_8.bytes[i] != 0)
        i++;
    self->utf_8.real_size = i;
}

void uni_char_print(unicode_char_t *self)
{

    printf("unicode\n");
    printf("\tsize: %d\n", self->unicode.real_size);
    printf("\tbytes: ");
    for (size_t i = 0; i != UNICODE_MAX_BYTES; i++)
        printf("%X   ", self->unicode.bytes[i]);
    printf("\n");

    printf("utf-8\n");
    printf("\tsize: %d \n", self->utf_8.real_size);
    printf("\tbytes: ");
    for (size_t i = 0; i != UNICODE_UTF_8_MAX_BYTES; i++)
        printf("%X   ", self->utf_8.bytes[i]);
    printf("\n");
    printf("\tlooks like: %s\n", self->utf_8.bytes);

    printf("table\n");
    printf("encoding: %s\n", uni_char_encoding_name(self));
    printf("\tindex: %d\n", self->table.value);
    printf("\tcoords: [%d, %d] \n", self->table.coords[0], self->table.coords[1]);
}

const char *uni_char_encoding_name_by_type(encoding_type_t type)
{
    switch (type)
    {
    case KOI_8_R:
        return "KOI8-r";
    case ISO_8859_5:
        return "ISO-8859-5";
    case CP1251:
        return "CP1251";
    default:
        return "NO ENCODING";
    }
}

const char *uni_char_encoding_name(unicode_char_t *self)
{
    return uni_char_encoding_name_by_type(self->table.type);
}

void uni_char_print_lite(unicode_char_t *self)
{
    printf("index: %d\t, %s\n", self->table.value, self->utf_8.bytes);
}

#endif // L_UNICODE_CHAR_H
