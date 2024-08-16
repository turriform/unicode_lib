#if !defined(LOCAL_UTILS_H)
#define LOCAL_UTILS_H
#include "framework.h"
/* 
    Byte buffer to store the file
    String utils 


*/
struct ByteBuffer
{
    size_t size;
    uint8_t *bytes;
};
void 
byte_buffer_set_from_file(bytebuffer_t *self, const char *filename);

/* String utils */
size_t 
string_get_size(const char *str);
bool 
string_char_is_in(char c, const char *str);
void 
string_clear(char *str, size_t len);


/* Unicode related byte utils */


void
byte_utils_from_str_to_byte_arr(uint8_t byte[], const char *str, size_t str_sz);
void
byte_utils_unicode_to_utf_8(uint8_t unicode_arr[], uint8_t utf_8_arr[]);

uint64_t 
byte_utils_get_all_code_points(uint8_t unicode[]);


/* BYTE BUFFER IMPL */

void byte_buffer_set_from_file(bytebuffer_t *self, const char *filename)
{
    /*
        this function will definetly allocate bytes on heap
        note to self: DO NOT forget to free

    */

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file \n");
        return;
    }

    if (fseek(fp, 0, SEEK_END) < 0)
    {
        perror("Error seeking file \n");
        return;
    }

    self->size = ftell(fp) + 1;

    self->bytes = (uint8_t *)calloc(self->size, sizeof(uint8_t));
    if (NULL == self->bytes)
    {
        return;
    }

    rewind(fp);
    fread(self->bytes, sizeof(uint8_t), self->size - 1, fp);
    self->bytes[self->size - 1] = 0;

    fclose(fp);
}


/* STRING IMPL */


size_t string_get_size(const char *str)
{
   
    size_t i = 0;
    while (str[i] != '\0')
        i++;

    return i;
}

bool string_char_is_in(char c, const char *str)
{
    size_t limit = string_get_size(str);
    for (size_t i = 0; i != limit; i++)
    {
        if (str[i] == c)
        {
            return true;
        }
    }
    return false;
}

void string_clear(char *str, size_t len)
{ // memset for str

    for (size_t i = 0; i != len; i++)
    {
        str[i] = 0;
    }
}


/* Unicode related byte utild IMPL*/


void
byte_utils_from_str_to_byte_arr(uint8_t byte[], const char *str, size_t str_sz)
{

    char byte_tmp[UNICODE_ALPHA_IN_BYTE] = "";
    size_t byte_i = 0;
    for (size_t i = 0, j = 0; i != str_sz; i++)
    {
        byte_tmp[j] = str[i];
        if ((i % UNICODE_ALPHA_IN_BYTE))
        {
            byte[byte_i] = strtol(byte_tmp, NULL, 16);
            j = 0;
            byte_i++;
        }
        j++;
    }
}


void
byte_utils_unicode_to_utf_8(uint8_t unicode_arr[], uint8_t utf_8_arr[])
{

    size_t unicode_i = 0;
  
    while (unicode_arr[unicode_i] == 0)
        unicode_i++;

    uint64_t full_codepoint = byte_utils_get_all_code_points(unicode_arr);

    if (full_codepoint <= 0x7F)
    {
        utf_8_arr[0] = (uint8_t)full_codepoint;
    }
    else if (full_codepoint <= 0x7FF)
    {
        utf_8_arr[0] = 0xC0 | (uint8_t)((full_codepoint >> 6) & 0x1F);
        utf_8_arr[1] = 0x80 | (uint8_t)(full_codepoint & 0x3F);
    }
    else if (full_codepoint <= 0xFFFF)
    {
        utf_8_arr[0] = 0xE0 | (uint8_t)((full_codepoint >> 12) & 0x0F);
        utf_8_arr[1] = 0x80 | (uint8_t)((full_codepoint >> 6) & 0x3F);
        utf_8_arr[2] = 0x80 | (uint8_t)(full_codepoint & 0x3F);
    }
    else if (full_codepoint <= 0x10FFFF)
    {
        utf_8_arr[0] = 0xF0 | (uint8_t)((full_codepoint >> 18) & 0x07);
        utf_8_arr[1] = 0x80 | (uint8_t)((full_codepoint >> 12) & 0x3F);
        utf_8_arr[2] = 0x80 | (uint8_t)((full_codepoint >> 6) & 0x3F);
        utf_8_arr[3] = 0x80 | (uint8_t)(full_codepoint & 0x3F);
    }

   
}


uint64_t 
byte_utils_get_all_code_points(uint8_t unicode[])
{

    uint64_t res =
        (uint64_t)unicode[3] |
        (uint64_t)unicode[2] << 8 |
        (uint64_t)unicode[1] << 16 |
        (uint64_t)unicode[0] << 24;

    return res;
}



#endif // LOCAL_UTILS_H
