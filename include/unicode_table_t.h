#if !defined(L_UNICODE_TABLE_H)
#define L_UNICODE_TABLE_H
#include "framework.h"
#include "utils.h"
#include "unicode_char_t.h"

struct UnicodeTable
{
    size_t items_sz;
    size_t items_offset;
    unicode_char_t **items;
    encoding_type_t encoding_type;
};

unicode_table_t *
unicode_table_create(encoding_type_t encoding_type);

void unicode_table_free(unicode_table_t *self);

unicode_char_t *
unicode_table_get_char_by_index(unicode_table_t *self, size_t index);

void unicode_table_print(unicode_table_t *self);

const char *
unicode_table_get_filename(encoding_type_t encoding_type);

/*IMPL */

unicode_table_t *
unicode_table_create(encoding_type_t encoding_type)
{

    const char *filename = unicode_table_get_filename(encoding_type);

    bytebuffer_t filebytes = (bytebuffer_t){0};

    byte_buffer_set_from_file(&filebytes, filename);

    if (filebytes.bytes == NULL)
    {
        perror("Filebytes are null... exiting \n");
        return NULL;
    }

    // printf("%s", filebytes.bytes);

    // start line counting to avoid realloc
    size_t line_count = 0, i = 0;
    while (i != filebytes.size)
    {

        if (filebytes.bytes[i] == '\n')
            line_count++;
        i++;
    }

    // mem alloc

    unicode_table_t *unicode_table = (unicode_table_t *)malloc(sizeof(unicode_table_t));

    if (NULL == unicode_table)
    {
        perror("unicode_table returned null... exiting \n");
        return NULL;
    }

    unicode_table->items = (unicode_char_t **)calloc(line_count, sizeof(unicode_char_t *));

    if (NULL == unicode_table->items)
    {
        perror(" unicode_table->items returned null... exiting \n");
        free(unicode_table);
        return NULL;
    }

    // allocation ok here, start reading strings

    char table_line_buff[UNICODE_BUFF_MD] = "";

    size_t j = 0, line = 0;
    i = 0;

    for (; i != filebytes.size; i++)
    {
        if (filebytes.bytes[i] == '\n')
        {
            j = 0;
            i++;

            unicode_char_t *unicode_char = (unicode_char_t *)malloc(sizeof(unicode_char_t));
            if (NULL == unicode_char)
            {
                free(unicode_table->items);
                free(unicode_table);
                return NULL;
            }

            *unicode_char = (unicode_char_t){0};
            uni_char_set_from_line(unicode_char, table_line_buff, encoding_type);

            unicode_table->items[line] = unicode_char;

            string_clear(table_line_buff, UNICODE_BUFF_MD);

            line++;
        }
        table_line_buff[j] = filebytes.bytes[i];
        j++;
    }

    // freeing bytes: from byte_buffer_set_from_file
    unicode_table->items_sz = line_count;
    free(filebytes.bytes);

    unicode_table->items_offset = unicode_table->items[0]->table.value;

    return unicode_table;
}

void unicode_table_free(unicode_table_t *self)
{
    if (self->items)
    {
        for (size_t i = 0; i != self->items_sz; i++)
            free(self->items[i]);

        free(self->items);
    }
    free(self);
}

unicode_char_t *
unicode_table_get_char_by_index(unicode_table_t *self, size_t index)
{
    return self->items[index];
}

void unicode_table_print(unicode_table_t *self)
{
    for (size_t i = 0; i != self->items_sz; i++)
        uni_char_print(self->items[i]);
}

const char *unicode_table_get_filename(encoding_type_t encoding_type)

{
    switch (encoding_type)
    {
    case KOI_8_R:
        return "./mappings/KOI8-R.txt";
    case ISO_8859_5:
        return "./mappings/ISO_8859-5.txt";
    case CP1251:
        return "./mappings/CP1251.txt";
    // no such file just a default
    default:
        return "./mappings/KOI8-R.txt";
    }
}

#endif // L_UNICODE_TABLE_H
