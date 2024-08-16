#if !defined(L_MAIN_DECODE_H)
#define L_MAIN_DECODE_H
#include "framework.h"
#include "unicode_char_t.h"
#include "unicode_table_t.h"

decoding_status_t decode_init(int argc, char *argv[]);
decoding_status_t decode_file_contents(const char *input_filename, const char *output_filename, encoding_type_t encoding);

decoding_status_t decode_file_contents(const char *input_filename, const char *output_filename, encoding_type_t encoding)
{
    /*
        -- creating specific table
        -- read input file
        -- convert to utf-8
        -- write to the output file

    */

    FILE *fp_in = fopen(input_filename, "rb");
    if (NULL == fp_in)
    {
        printf("FAILED: Input file doesnt exist %s\n", input_filename);

        return DECODE_FAILED;
    }

    unicode_table_t *unicode_table = unicode_table_create(encoding);
    if (NULL == unicode_table)
    {
        printf("Error allocating unicode_table\n");
        return DECODE_FAILED;
    }

    FILE *fp_out = fopen(output_filename, "wb");

    if (NULL == fp_out)
    {
        unicode_table_free(unicode_table);
        if (fp_in != NULL)
            fclose(fp_in);
        perror("Error opening output file\n");

        return DECODE_FAILED;
    }

    unicode_char_t *uni_char = NULL;
    uint8_t c;

    fseek(fp_in, 0, SEEK_END);
    size_t input_file_sz = ftell(fp_in);
    rewind(fp_in);
    for (size_t i = 0; i != input_file_sz; i++)
    {
        fread(&c, 1, 1, fp_in);
        uni_char = unicode_table->items[c - unicode_table->items_offset];

        fwrite(uni_char->utf_8.bytes, uni_char->utf_8.real_size, 1, fp_out);
    }

    // unicode_table_print(unicode_table);
    printf("OK: attempting to decode file %s from %s to UTF-8\n", input_filename, uni_char_encoding_name_by_type(encoding));
    printf("OK: file was decoded \nFile location: %s\n", output_filename);

    unicode_table_free(unicode_table);
    fclose(fp_in);
    fclose(fp_out);

    return DECODE_OK;
}

decoding_status_t decode_init(int argc, char *argv[])
{
    const char *error_str = "Usage: prog filename.txt encoding \n\n1\tKOI-8r\n2\tISO-8859-5\n3\tcp1251\n";

    if (argc < 3)
    {
        printf("%s", error_str);
        return DECODE_FAILED;
    }

    uint8_t code_arg = atoi(argv[2]);
    encoding_type_t type = (encoding_type_t)code_arg;

    const char *type_str = uni_char_encoding_name_by_type(type);
    printf("%s selected \n", type_str);

    if (type <= NO_ENCODING || type >= NUM_ENCODING)
    {
        printf("%s", error_str);
        return DECODE_FAILED;
    }

    return decode_file_contents(argv[1], "./output/decoded.txt", type);
}

#endif // L_MAIN_DECODE_H
