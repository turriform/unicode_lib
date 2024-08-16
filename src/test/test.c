#include "framework.h"
#include "utils.h"
#include "unicode_char_t.h"
#include "unicode_table_t.h"


void test_unicode_create()
{
    unicode_char_t *u = uni_char_init();

    uni_char_set_from_line(u, "0xCE	043D	#	CYRILLIC SMALL LETTER EN", KOI_8_R);
    // uni_char_print(u);

    free(u);
}

void test_unicode_table()
{
    unicode_table_t *koi_8 = unicode_table_create(KOI_8_R);
    // unicode_table_print(koi_8);
    unicode_table_free(koi_8);
}

void test_file_decoding()
{
    decode_file_contents("./input/koi8.txt", "./output/koi-8-decoded.txt", KOI_8_R);
    decode_file_contents("./input/iso-8859-5.txt", "./output/iso-8859-5-decoded.txt", ISO_8859_5);
    decode_file_contents("./input/cp1251.txt", "./output/cp1251-decoded.txt", CP1251);
}

void run_tests()
{
    test_unicode_create();
    test_unicode_table();
    test_file_decoding();
}
