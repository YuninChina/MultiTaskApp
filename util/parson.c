/*
 SPDX-License-Identifier: MIT

 Parson 1.2.1 ( http://kgabis.github.com/parson/ )
 Copyright (c) 2012 - 2021 Krzysztof Gabis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/
#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif /* _CRT_SECURE_NO_WARNINGS */
#endif /* _MSC_VER */

#include "parson.h"

#define PARSON_IMPL_VERSION_MAJOR 1
#define PARSON_IMPL_VERSION_MINOR 3
#define PARSON_IMPL_VERSION_PATCH 0

#if (PARSON_VERSION_MAJOR != PARSON_IMPL_VERSION_MAJOR)\
|| (PARSON_VERSION_MINOR != PARSON_IMPL_VERSION_MINOR)\
|| (PARSON_VERSION_PATCH != PARSON_IMPL_VERSION_PATCH)
#error "parson version mismatch between parson.c and parson.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

/* Apparently sscanf is not implemented in some "standard" libraries, so don't use it, if you
 * don't have to. */
#ifdef sscanf
#undef sscanf
#define sscanf THINK_TWICE_ABOUT_USING_SSCANF
#endif

/* strcpy is unsafe */
#ifdef strcpy
#undef strcpy
#endif
#define strcpy USE_MEMCPY_INSTEAD_OF_STRCPY

#define STARTING_CAPACITY 16
#define MAX_NESTING       2048

#ifndef PARSON_DEFAULT_FLOAT_FORMAT
#define PARSON_DEFAULT_FLOAT_FORMAT "%1.17g" /* do not increase precision without incresing NUM_BUF_SIZE */
#endif

#ifndef PARSON_NUM_BUF_SIZE
#define PARSON_NUM_BUF_SIZE 64 /* double printed with "%1.17g" shouldn't be longer than 25 bytes so let's be paranoid and use 64 */
#endif

#define SIZEOF_TOKEN(a)       (sizeof(a) - 1)
#define SKIP_CHAR(str)        ((*str)++)
#define SKIP_WHITESPACES(str) while (isspace((unsigned char)(**str))) { SKIP_CHAR(str); }
#define MAX(a, b)             ((a) > (b) ? (a) : (b))

#undef malloc
#undef free

#if defined(isnan) && defined(isinf)
#define IS_NUMBER_INVALID(x) (isnan((x)) || isinf((x)))
#else
#define IS_NUMBER_INVALID(x) (((x) * 0.0) != 0.0)
#endif

#define OBJECT_INVALID_IX ((size_t)-1)

static PJSON_Malloc_Function parson_malloc = malloc;
static PJSON_Free_Function parson_free = free;

static int parson_escape_slashes = 1;

static char *parson_float_format = NULL;

#define IS_CONT(b) (((unsigned char)(b) & 0xC0) == 0x80) /* is utf-8 continuation byte */

typedef int parson_bool_t;

#define PARSON_TRUE 1
#define PARSON_FALSE 0

typedef struct pjson_string {
    char *chars;
    size_t length;
} PJSON_String;

/* Type definitions */
typedef union pjson_value_value {
    PJSON_String  string;
    double       number;
    PJSON_Object *object;
    PJSON_Array  *array;
    int          boolean;
    int          null;
} PJSON_Value_Value;

struct pjson_value_t {
    PJSON_Value      *parent;
    PJSON_Value_Type  type;
    PJSON_Value_Value value;
};

struct pjson_object_t {
    PJSON_Value    *wrapping_value;
    size_t        *cells;
    unsigned long *hashes;
    char         **names;
    PJSON_Value   **values;
    size_t        *cell_ixs;
    size_t         count;
    size_t         item_capacity;
    size_t         cell_capacity;
};

struct pjson_array_t {
    PJSON_Value  *wrapping_value;
    PJSON_Value **items;
    size_t       count;
    size_t       capacity;
};

/* Various */
static char * read_file(const char *filename);
static void   remove_comments(char *string, const char *start_token, const char *end_token);
static char * parson_strndup(const char *string, size_t n);
static char * parson_strdup(const char *string);
static int    hex_char_to_int(char c);
static PJSON_Status parse_utf16_hex(const char *string, unsigned int *result);
static int         num_bytes_in_utf8_sequence(unsigned char c);
static PJSON_Status   verify_utf8_sequence(const unsigned char *string, int *len);
static parson_bool_t is_valid_utf8(const char *string, size_t string_len);
static parson_bool_t is_decimal(const char *string, size_t length);
static unsigned long hash_string(const char *string, size_t n);

/* PJSON Object */
static PJSON_Object * pjson_object_make(PJSON_Value *wrapping_value);
static PJSON_Status   pjson_object_init(PJSON_Object *object, size_t capacity);
static void          pjson_object_deinit(PJSON_Object *object, parson_bool_t free_keys, parson_bool_t free_values);
static PJSON_Status   pjson_object_grow_and_rehash(PJSON_Object *object);
static size_t        pjson_object_get_cell_ix(const PJSON_Object *object, const char *key, size_t key_len, unsigned long hash, parson_bool_t *out_found);
static PJSON_Status   pjson_object_add(PJSON_Object *object, char *name, PJSON_Value *value);
static PJSON_Value  * pjson_object_getn_value(const PJSON_Object *object, const char *name, size_t name_len);
static PJSON_Status   pjson_object_remove_internal(PJSON_Object *object, const char *name, parson_bool_t free_value);
static PJSON_Status   pjson_object_dotremove_internal(PJSON_Object *object, const char *name, parson_bool_t free_value);
static void          pjson_object_free(PJSON_Object *object);

/* PJSON Array */
static PJSON_Array * pjson_array_make(PJSON_Value *wrapping_value);
static PJSON_Status  pjson_array_add(PJSON_Array *array, PJSON_Value *value);
static PJSON_Status  pjson_array_resize(PJSON_Array *array, size_t new_capacity);
static void         pjson_array_free(PJSON_Array *array);

/* PJSON Value */
static PJSON_Value * pjson_value_init_string_no_copy(char *string, size_t length);
static const PJSON_String * pjson_value_get_string_desc(const PJSON_Value *value);

/* Parser */
static PJSON_Status   skip_quotes(const char **string);
static PJSON_Status   parse_utf16(const char **unprocessed, char **processed);
static char *        process_string(const char *input, size_t input_len, size_t *output_len);
static char *        get_quoted_string(const char **string, size_t *output_string_len);
static PJSON_Value *  parse_object_value(const char **string, size_t nesting);
static PJSON_Value *  parse_array_value(const char **string, size_t nesting);
static PJSON_Value *  parse_string_value(const char **string);
static PJSON_Value *  parse_boolean_value(const char **string);
static PJSON_Value *  parse_number_value(const char **string);
static PJSON_Value *  parse_null_value(const char **string);
static PJSON_Value *  parse_value(const char **string, size_t nesting);

/* Serialization */
static int pjson_serialize_to_buffer_r(const PJSON_Value *value, char *buf, int level, parson_bool_t is_pretty, char *num_buf);
static int pjson_serialize_string(const char *string, size_t len, char *buf);
static int append_indent(char *buf, int level);
static int append_string(char *buf, const char *string);

/* Various */
static char * read_file(const char * filename) {
    FILE *fp = fopen(filename, "r");
    size_t size_to_read = 0;
    size_t size_read = 0;
    long pos;
    char *file_contents;
    if (!fp) {
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    pos = ftell(fp);
    if (pos < 0) {
        fclose(fp);
        return NULL;
    }
    size_to_read = pos;
    rewind(fp);
    file_contents = (char*)parson_malloc(sizeof(char) * (size_to_read + 1));
    if (!file_contents) {
        fclose(fp);
        return NULL;
    }
    size_read = fread(file_contents, 1, size_to_read, fp);
    if (size_read == 0 || ferror(fp)) {
        fclose(fp);
        parson_free(file_contents);
        return NULL;
    }
    fclose(fp);
    file_contents[size_read] = '\0';
    return file_contents;
}

static void remove_comments(char *string, const char *start_token, const char *end_token) {
    parson_bool_t in_string = PARSON_FALSE, escaped = PARSON_FALSE;
    size_t i;
    char *ptr = NULL, current_char;
    size_t start_token_len = strlen(start_token);
    size_t end_token_len = strlen(end_token);
    if (start_token_len == 0 || end_token_len == 0) {
        return;
    }
    while ((current_char = *string) != '\0') {
        if (current_char == '\\' && !escaped) {
            escaped = PARSON_TRUE;
            string++;
            continue;
        } else if (current_char == '\"' && !escaped) {
            in_string = !in_string;
        } else if (!in_string && strncmp(string, start_token, start_token_len) == 0) {
            for(i = 0; i < start_token_len; i++) {
                string[i] = ' ';
            }
            string = string + start_token_len;
            ptr = strstr(string, end_token);
            if (!ptr) {
                return;
            }
            for (i = 0; i < (ptr - string) + end_token_len; i++) {
                string[i] = ' ';
            }
            string = ptr + end_token_len - 1;
        }
        escaped = PARSON_FALSE;
        string++;
    }
}

static char * parson_strndup(const char *string, size_t n) {
    /* We expect the caller has validated that 'n' fits within the input buffer. */
    char *output_string = (char*)parson_malloc(n + 1);
    if (!output_string) {
        return NULL;
    }
    output_string[n] = '\0';
    memcpy(output_string, string, n);
    return output_string;
}

static char * parson_strdup(const char *string) {
    return parson_strndup(string, strlen(string));
}

static int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

static PJSON_Status parse_utf16_hex(const char *s, unsigned int *result) {
    int x1, x2, x3, x4;
    if (s[0] == '\0' || s[1] == '\0' || s[2] == '\0' || s[3] == '\0') {
        return PJSONFailure;
    }
    x1 = hex_char_to_int(s[0]);
    x2 = hex_char_to_int(s[1]);
    x3 = hex_char_to_int(s[2]);
    x4 = hex_char_to_int(s[3]);
    if (x1 == -1 || x2 == -1 || x3 == -1 || x4 == -1) {
        return PJSONFailure;
    }
    *result = (unsigned int)((x1 << 12) | (x2 << 8) | (x3 << 4) | x4);
    return PJSONSuccess;
}

static int num_bytes_in_utf8_sequence(unsigned char c) {
    if (c == 0xC0 || c == 0xC1 || c > 0xF4 || IS_CONT(c)) {
        return 0;
    } else if ((c & 0x80) == 0) {    /* 0xxxxxxx */
        return 1;
    } else if ((c & 0xE0) == 0xC0) { /* 110xxxxx */
        return 2;
    } else if ((c & 0xF0) == 0xE0) { /* 1110xxxx */
        return 3;
    } else if ((c & 0xF8) == 0xF0) { /* 11110xxx */
        return 4;
    }
    return 0; /* won't happen */
}

static PJSON_Status verify_utf8_sequence(const unsigned char *string, int *len) {
    unsigned int cp = 0;
    *len = num_bytes_in_utf8_sequence(string[0]);

    if (*len == 1) {
        cp = string[0];
    } else if (*len == 2 && IS_CONT(string[1])) {
        cp = string[0] & 0x1F;
        cp = (cp << 6) | (string[1] & 0x3F);
    } else if (*len == 3 && IS_CONT(string[1]) && IS_CONT(string[2])) {
        cp = ((unsigned char)string[0]) & 0xF;
        cp = (cp << 6) | (string[1] & 0x3F);
        cp = (cp << 6) | (string[2] & 0x3F);
    } else if (*len == 4 && IS_CONT(string[1]) && IS_CONT(string[2]) && IS_CONT(string[3])) {
        cp = string[0] & 0x7;
        cp = (cp << 6) | (string[1] & 0x3F);
        cp = (cp << 6) | (string[2] & 0x3F);
        cp = (cp << 6) | (string[3] & 0x3F);
    } else {
        return PJSONFailure;
    }

    /* overlong encodings */
    if ((cp < 0x80    && *len > 1) ||
        (cp < 0x800   && *len > 2) ||
        (cp < 0x10000 && *len > 3)) {
        return PJSONFailure;
    }

    /* invalid unicode */
    if (cp > 0x10FFFF) {
        return PJSONFailure;
    }

    /* surrogate halves */
    if (cp >= 0xD800 && cp <= 0xDFFF) {
        return PJSONFailure;
    }

    return PJSONSuccess;
}

static int is_valid_utf8(const char *string, size_t string_len) {
    int len = 0;
    const char *string_end =  string + string_len;
    while (string < string_end) {
        if (verify_utf8_sequence((const unsigned char*)string, &len) != PJSONSuccess) {
            return PARSON_FALSE;
        }
        string += len;
    }
    return PARSON_TRUE;
}

static parson_bool_t is_decimal(const char *string, size_t length) {
    if (length > 1 && string[0] == '0' && string[1] != '.') {
        return PARSON_FALSE;
    }
    if (length > 2 && !strncmp(string, "-0", 2) && string[2] != '.') {
        return PARSON_FALSE;
    }
    while (length--) {
        if (strchr("xX", string[length])) {
            return PARSON_FALSE;
        }
    }
    return PARSON_TRUE;
}

static unsigned long hash_string(const char *string, size_t n) {
#ifdef PARSON_FORCE_HASH_COLLISIONS
    (void)string;
    (void)n;
    return 0;
#else
    unsigned long hash = 5381;
    unsigned char c;
    size_t i = 0;
    for (i = 0; i < n; i++) {
        c = string[i];
        if (c == '\0') {
            break;
        }
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
#endif
}

/* PJSON Object */
static PJSON_Object * pjson_object_make(PJSON_Value *wrapping_value) {
    PJSON_Status res = PJSONFailure;
    PJSON_Object *new_obj = (PJSON_Object*)parson_malloc(sizeof(PJSON_Object));
    if (new_obj == NULL) {
        return NULL;
    }
    new_obj->wrapping_value = wrapping_value;
    res = pjson_object_init(new_obj, 0);
    if (res != PJSONSuccess) {
        parson_free(new_obj);
        return NULL;
    }
    return new_obj;
}

static PJSON_Status pjson_object_init(PJSON_Object *object, size_t capacity) {
    unsigned int i = 0;

    object->cells = NULL;
    object->names = NULL;
    object->values = NULL;
    object->cell_ixs = NULL;
    object->hashes = NULL;

    object->count = 0;
    object->cell_capacity = capacity;
    object->item_capacity = (unsigned int)(capacity * 0.7f);

    if (capacity == 0) {
        return PJSONSuccess;
    }

    object->cells = (size_t*)parson_malloc(object->cell_capacity * sizeof(*object->cells));
    object->names = (char**)parson_malloc(object->item_capacity * sizeof(*object->names));
    object->values = (PJSON_Value**)parson_malloc(object->item_capacity * sizeof(*object->values));
    object->cell_ixs = (size_t*)parson_malloc(object->item_capacity * sizeof(*object->cell_ixs));
    object->hashes = (unsigned long*)parson_malloc(object->item_capacity * sizeof(*object->hashes));
    if (object->cells == NULL
        || object->names == NULL
        || object->values == NULL
        || object->cell_ixs == NULL
        || object->hashes == NULL) {
        goto error;
    }
    for (i = 0; i < object->cell_capacity; i++) {
        object->cells[i] = OBJECT_INVALID_IX;
    }
    return PJSONSuccess;
error:
    parson_free(object->cells);
    parson_free(object->names);
    parson_free(object->values);
    parson_free(object->cell_ixs);
    parson_free(object->hashes);
    return PJSONFailure;
}

static void pjson_object_deinit(PJSON_Object *object, parson_bool_t free_keys, parson_bool_t free_values) {
    unsigned int i = 0;
    for (i = 0; i < object->count; i++) {
        if (free_keys) {
            parson_free(object->names[i]);
        }
        if (free_values) {
            pjson_value_free(object->values[i]);
        }
    }

    object->count = 0;
    object->item_capacity = 0;
    object->cell_capacity = 0;

    parson_free(object->cells);
    parson_free(object->names);
    parson_free(object->values);
    parson_free(object->cell_ixs);
    parson_free(object->hashes);

    object->cells = NULL;
    object->names = NULL;
    object->values = NULL;
    object->cell_ixs = NULL;
    object->hashes = NULL;
}

static PJSON_Status pjson_object_grow_and_rehash(PJSON_Object *object) {
    PJSON_Value *wrapping_value = NULL;
    PJSON_Object new_object;
    char *key = NULL;
    PJSON_Value *value = NULL;
    unsigned int i = 0;
    size_t new_capacity = MAX(object->cell_capacity * 2, STARTING_CAPACITY);
    PJSON_Status res = pjson_object_init(&new_object, new_capacity);
    if (res != PJSONSuccess) {
        return PJSONFailure;
    }

    wrapping_value = pjson_object_get_wrapping_value(object);
    new_object.wrapping_value = wrapping_value;

    for (i = 0; i < object->count; i++) {
        key = object->names[i];
        value = object->values[i];
        res = pjson_object_add(&new_object, key, value);
        if (res != PJSONSuccess) {
            pjson_object_deinit(&new_object, PARSON_FALSE, PARSON_FALSE);
            return PJSONFailure;
        }
        value->parent = wrapping_value;
    }
    pjson_object_deinit(object, PARSON_FALSE, PARSON_FALSE);
    *object = new_object;
    return PJSONSuccess;
}

static size_t pjson_object_get_cell_ix(const PJSON_Object *object, const char *key, size_t key_len, unsigned long hash, parson_bool_t *out_found) {
    size_t cell_ix = hash & (object->cell_capacity - 1);
    size_t cell = 0;
    size_t ix = 0;
    unsigned int i = 0;
    unsigned long hash_to_check = 0;
    const char *key_to_check = NULL;
    size_t key_to_check_len = 0;

    *out_found = PARSON_FALSE;

    for (i = 0; i < object->cell_capacity; i++) {
        ix = (cell_ix + i) & (object->cell_capacity - 1);
        cell = object->cells[ix];
        if (cell == OBJECT_INVALID_IX) {
            return ix;
        }
        hash_to_check = object->hashes[cell];
        if (hash != hash_to_check) {
            continue;
        }
        key_to_check = object->names[cell];
        key_to_check_len = strlen(key_to_check);
        if (key_to_check_len == key_len && strncmp(key, key_to_check, key_len) == 0) {
            *out_found = PARSON_TRUE;
            return ix;
        }
    }
    return OBJECT_INVALID_IX;
}

static PJSON_Status pjson_object_add(PJSON_Object *object, char *name, PJSON_Value *value) {
    unsigned long hash = 0;
    parson_bool_t found = PARSON_FALSE;
    size_t cell_ix = 0;
    PJSON_Status res = PJSONFailure;

    if (!object || !name || !value) {
        return PJSONFailure;
    }

    hash = hash_string(name, strlen(name));
    found = PARSON_FALSE;
    cell_ix = pjson_object_get_cell_ix(object, name, strlen(name), hash, &found);
    if (found) {
        return PJSONFailure;
    }

    if (object->count >= object->item_capacity) {
        res = pjson_object_grow_and_rehash(object);
        if (res != PJSONSuccess) {
            return PJSONFailure;
        }
        cell_ix = pjson_object_get_cell_ix(object, name, strlen(name), hash, &found);
    }

    object->names[object->count] = name;
    object->cells[cell_ix] = object->count;
    object->values[object->count] = value;
    object->cell_ixs[object->count] = cell_ix;
    object->hashes[object->count] = hash;
    object->count++;
    value->parent = pjson_object_get_wrapping_value(object);

    return PJSONSuccess;
}

static PJSON_Value * pjson_object_getn_value(const PJSON_Object *object, const char *name, size_t name_len) {
    unsigned long hash = 0;
    parson_bool_t found = PARSON_FALSE;
    unsigned long cell_ix = 0;
    size_t item_ix = 0;
    if (!object || !name) {
        return NULL;
    }
    hash = hash_string(name, name_len);
    found = PARSON_FALSE;
    cell_ix = pjson_object_get_cell_ix(object, name, name_len, hash, &found);
    if (!found) {
        return NULL;
    }
    item_ix = object->cells[cell_ix];
    return object->values[item_ix];
}

static PJSON_Status pjson_object_remove_internal(PJSON_Object *object, const char *name, parson_bool_t free_value) {
    unsigned long hash = 0;
    parson_bool_t found = PARSON_FALSE;
    size_t cell = 0;
    size_t item_ix = 0;
    size_t last_item_ix = 0;
    size_t i = 0;
    size_t j = 0;
    size_t x = 0;
    size_t k = 0;
    PJSON_Value *val = NULL;

    if (object == NULL) {
        return PJSONFailure;
    }

    hash = hash_string(name, strlen(name));
    found = PARSON_FALSE;
    cell = pjson_object_get_cell_ix(object, name, strlen(name), hash, &found);
    if (!found) {
        return PJSONFailure;
    }

    item_ix = object->cells[cell];
    if (free_value) {
        val = object->values[item_ix];
        pjson_value_free(val);
        val = NULL;
    }

    parson_free(object->names[item_ix]);
    last_item_ix = object->count - 1;
    if (item_ix < last_item_ix) {
        object->names[item_ix] = object->names[last_item_ix];
        object->values[item_ix] = object->values[last_item_ix];
        object->cell_ixs[item_ix] = object->cell_ixs[last_item_ix];
        object->hashes[item_ix] = object->hashes[last_item_ix];
        object->cells[object->cell_ixs[item_ix]] = item_ix;
    }
    object->count--;

    i = cell;
    j = i;
    for (x = 0; x < (object->cell_capacity - 1); x++) {
        j = (j + 1) & (object->cell_capacity - 1);
        if (object->cells[j] == OBJECT_INVALID_IX) {
            break;
        }
        k = object->hashes[object->cells[j]] & (object->cell_capacity - 1);
        if ((j > i && (k <= i || k > j))
         || (j < i && (k <= i && k > j))) {
            object->cell_ixs[object->cells[j]] = i;
            object->cells[i] = object->cells[j];
            i = j;
        }
    }
    object->cells[i] = OBJECT_INVALID_IX;
    return PJSONSuccess;
}

static PJSON_Status pjson_object_dotremove_internal(PJSON_Object *object, const char *name, parson_bool_t free_value) {
    PJSON_Value *temp_value = NULL;
    PJSON_Object *temp_object = NULL;
    const char *dot_pos = strchr(name, '.');
    if (!dot_pos) {
        return pjson_object_remove_internal(object, name, free_value);
    }
    temp_value = pjson_object_getn_value(object, name, dot_pos - name);
    if (pjson_value_get_type(temp_value) != PJSONObject) {
        return PJSONFailure;
    }
    temp_object = pjson_value_get_object(temp_value);
    return pjson_object_dotremove_internal(temp_object, dot_pos + 1, free_value);
}

static void pjson_object_free(PJSON_Object *object) {
    pjson_object_deinit(object, PARSON_TRUE, PARSON_TRUE);
    parson_free(object);
}

/* PJSON Array */
static PJSON_Array * pjson_array_make(PJSON_Value *wrapping_value) {
    PJSON_Array *new_array = (PJSON_Array*)parson_malloc(sizeof(PJSON_Array));
    if (new_array == NULL) {
        return NULL;
    }
    new_array->wrapping_value = wrapping_value;
    new_array->items = (PJSON_Value**)NULL;
    new_array->capacity = 0;
    new_array->count = 0;
    return new_array;
}

static PJSON_Status pjson_array_add(PJSON_Array *array, PJSON_Value *value) {
    if (array->count >= array->capacity) {
        size_t new_capacity = MAX(array->capacity * 2, STARTING_CAPACITY);
        if (pjson_array_resize(array, new_capacity) != PJSONSuccess) {
            return PJSONFailure;
        }
    }
    value->parent = pjson_array_get_wrapping_value(array);
    array->items[array->count] = value;
    array->count++;
    return PJSONSuccess;
}

static PJSON_Status pjson_array_resize(PJSON_Array *array, size_t new_capacity) {
    PJSON_Value **new_items = NULL;
    if (new_capacity == 0) {
        return PJSONFailure;
    }
    new_items = (PJSON_Value**)parson_malloc(new_capacity * sizeof(PJSON_Value*));
    if (new_items == NULL) {
        return PJSONFailure;
    }
    if (array->items != NULL && array->count > 0) {
        memcpy(new_items, array->items, array->count * sizeof(PJSON_Value*));
    }
    parson_free(array->items);
    array->items = new_items;
    array->capacity = new_capacity;
    return PJSONSuccess;
}

static void pjson_array_free(PJSON_Array *array) {
    size_t i;
    for (i = 0; i < array->count; i++) {
        pjson_value_free(array->items[i]);
    }
    parson_free(array->items);
    parson_free(array);
}

/* PJSON Value */
static PJSON_Value * pjson_value_init_string_no_copy(char *string, size_t length) {
    PJSON_Value *new_value = (PJSON_Value*)parson_malloc(sizeof(PJSON_Value));
    if (!new_value) {
        return NULL;
    }
    new_value->parent = NULL;
    new_value->type = PJSONString;
    new_value->value.string.chars = string;
    new_value->value.string.length = length;
    return new_value;
}

/* Parser */
static PJSON_Status skip_quotes(const char **string) {
    if (**string != '\"') {
        return PJSONFailure;
    }
    SKIP_CHAR(string);
    while (**string != '\"') {
        if (**string == '\0') {
            return PJSONFailure;
        } else if (**string == '\\') {
            SKIP_CHAR(string);
            if (**string == '\0') {
                return PJSONFailure;
            }
        }
        SKIP_CHAR(string);
    }
    SKIP_CHAR(string);
    return PJSONSuccess;
}

static PJSON_Status parse_utf16(const char **unprocessed, char **processed) {
    unsigned int cp, lead, trail;
    char *processed_ptr = *processed;
    const char *unprocessed_ptr = *unprocessed;
    PJSON_Status status = PJSONFailure;
    unprocessed_ptr++; /* skips u */
    status = parse_utf16_hex(unprocessed_ptr, &cp);
    if (status != PJSONSuccess) {
        return PJSONFailure;
    }
    if (cp < 0x80) {
        processed_ptr[0] = (char)cp; /* 0xxxxxxx */
    } else if (cp < 0x800) {
        processed_ptr[0] = ((cp >> 6) & 0x1F) | 0xC0; /* 110xxxxx */
        processed_ptr[1] = ((cp)      & 0x3F) | 0x80; /* 10xxxxxx */
        processed_ptr += 1;
    } else if (cp < 0xD800 || cp > 0xDFFF) {
        processed_ptr[0] = ((cp >> 12) & 0x0F) | 0xE0; /* 1110xxxx */
        processed_ptr[1] = ((cp >> 6)  & 0x3F) | 0x80; /* 10xxxxxx */
        processed_ptr[2] = ((cp)       & 0x3F) | 0x80; /* 10xxxxxx */
        processed_ptr += 2;
    } else if (cp >= 0xD800 && cp <= 0xDBFF) { /* lead surrogate (0xD800..0xDBFF) */
        lead = cp;
        unprocessed_ptr += 4; /* should always be within the buffer, otherwise previous sscanf would fail */
        if (*unprocessed_ptr++ != '\\' || *unprocessed_ptr++ != 'u') {
            return PJSONFailure;
        }
        status = parse_utf16_hex(unprocessed_ptr, &trail);
        if (status != PJSONSuccess || trail < 0xDC00 || trail > 0xDFFF) { /* valid trail surrogate? (0xDC00..0xDFFF) */
            return PJSONFailure;
        }
        cp = ((((lead - 0xD800) & 0x3FF) << 10) | ((trail - 0xDC00) & 0x3FF)) + 0x010000;
        processed_ptr[0] = (((cp >> 18) & 0x07) | 0xF0); /* 11110xxx */
        processed_ptr[1] = (((cp >> 12) & 0x3F) | 0x80); /* 10xxxxxx */
        processed_ptr[2] = (((cp >> 6)  & 0x3F) | 0x80); /* 10xxxxxx */
        processed_ptr[3] = (((cp)       & 0x3F) | 0x80); /* 10xxxxxx */
        processed_ptr += 3;
    } else { /* trail surrogate before lead surrogate */
        return PJSONFailure;
    }
    unprocessed_ptr += 3;
    *processed = processed_ptr;
    *unprocessed = unprocessed_ptr;
    return PJSONSuccess;
}


/* Copies and processes passed string up to supplied length.
Example: "\u006Corem ipsum" -> lorem ipsum */
static char* process_string(const char *input, size_t input_len, size_t *output_len) {
    const char *input_ptr = input;
    size_t initial_size = (input_len + 1) * sizeof(char);
    size_t final_size = 0;
    char *output = NULL, *output_ptr = NULL, *resized_output = NULL;
    output = (char*)parson_malloc(initial_size);
    if (output == NULL) {
        goto error;
    }
    output_ptr = output;
    while ((*input_ptr != '\0') && (size_t)(input_ptr - input) < input_len) {
        if (*input_ptr == '\\') {
            input_ptr++;
            switch (*input_ptr) {
                case '\"': *output_ptr = '\"'; break;
                case '\\': *output_ptr = '\\'; break;
                case '/':  *output_ptr = '/';  break;
                case 'b':  *output_ptr = '\b'; break;
                case 'f':  *output_ptr = '\f'; break;
                case 'n':  *output_ptr = '\n'; break;
                case 'r':  *output_ptr = '\r'; break;
                case 't':  *output_ptr = '\t'; break;
                case 'u':
                    if (parse_utf16(&input_ptr, &output_ptr) != PJSONSuccess) {
                        goto error;
                    }
                    break;
                default:
                    goto error;
            }
        } else if ((unsigned char)*input_ptr < 0x20) {
            goto error; /* 0x00-0x19 are invalid characters for json string (http://www.ietf.org/rfc/rfc4627.txt) */
        } else {
            *output_ptr = *input_ptr;
        }
        output_ptr++;
        input_ptr++;
    }
    *output_ptr = '\0';
    /* resize to new length */
    final_size = (size_t)(output_ptr-output) + 1;
    /* todo: don't resize if final_size == initial_size */
    resized_output = (char*)parson_malloc(final_size);
    if (resized_output == NULL) {
        goto error;
    }
    memcpy(resized_output, output, final_size);
    *output_len = final_size - 1;
    parson_free(output);
    return resized_output;
error:
    parson_free(output);
    return NULL;
}

/* Return processed contents of a string between quotes and
   skips passed argument to a matching quote. */
static char * get_quoted_string(const char **string, size_t *output_string_len) {
    const char *string_start = *string;
    size_t input_string_len = 0;
    PJSON_Status status = skip_quotes(string);
    if (status != PJSONSuccess) {
        return NULL;
    }
    input_string_len = *string - string_start - 2; /* length without quotes */
    return process_string(string_start + 1, input_string_len, output_string_len);
}

static PJSON_Value * parse_value(const char **string, size_t nesting) {
    if (nesting > MAX_NESTING) {
        return NULL;
    }
    SKIP_WHITESPACES(string);
    switch (**string) {
        case '{':
            return parse_object_value(string, nesting + 1);
        case '[':
            return parse_array_value(string, nesting + 1);
        case '\"':
            return parse_string_value(string);
        case 'f': case 't':
            return parse_boolean_value(string);
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return parse_number_value(string);
        case 'n':
            return parse_null_value(string);
        default:
            return NULL;
    }
}

static PJSON_Value * parse_object_value(const char **string, size_t nesting) {
    PJSON_Status status = PJSONFailure;
    PJSON_Value *output_value = NULL, *new_value = NULL;
    PJSON_Object *output_object = NULL;
    char *new_key = NULL;

    output_value = pjson_value_init_object();
    if (output_value == NULL) {
        return NULL;
    }
    if (**string != '{') {
        pjson_value_free(output_value);
        return NULL;
    }
    output_object = pjson_value_get_object(output_value);
    SKIP_CHAR(string);
    SKIP_WHITESPACES(string);
    if (**string == '}') { /* empty object */
        SKIP_CHAR(string);
        return output_value;
    }
    while (**string != '\0') {
        size_t key_len = 0;
        new_key = get_quoted_string(string, &key_len);
        /* We do not support key names with embedded \0 chars */
        if (!new_key) {
            pjson_value_free(output_value);
            return NULL;
        }
        if (key_len != strlen(new_key)) {
            parson_free(new_key);
            pjson_value_free(output_value);
            return NULL;
        }
        SKIP_WHITESPACES(string);
        if (**string != ':') {
            parson_free(new_key);
            pjson_value_free(output_value);
            return NULL;
        }
        SKIP_CHAR(string);
        new_value = parse_value(string, nesting);
        if (new_value == NULL) {
            parson_free(new_key);
            pjson_value_free(output_value);
            return NULL;
        }
        status = pjson_object_add(output_object, new_key, new_value);
        if (status != PJSONSuccess) {
            parson_free(new_key);
            pjson_value_free(new_value);
            pjson_value_free(output_value);
            return NULL;
        }
        SKIP_WHITESPACES(string);
        if (**string != ',') {
            break;
        }
        SKIP_CHAR(string);
        SKIP_WHITESPACES(string);
    }
    SKIP_WHITESPACES(string);
    if (**string != '}') {
        pjson_value_free(output_value);
        return NULL;
    }
    SKIP_CHAR(string);
    return output_value;
}

static PJSON_Value * parse_array_value(const char **string, size_t nesting) {
    PJSON_Value *output_value = NULL, *new_array_value = NULL;
    PJSON_Array *output_array = NULL;
    output_value = pjson_value_init_array();
    if (output_value == NULL) {
        return NULL;
    }
    if (**string != '[') {
        pjson_value_free(output_value);
        return NULL;
    }
    output_array = pjson_value_get_array(output_value);
    SKIP_CHAR(string);
    SKIP_WHITESPACES(string);
    if (**string == ']') { /* empty array */
        SKIP_CHAR(string);
        return output_value;
    }
    while (**string != '\0') {
        new_array_value = parse_value(string, nesting);
        if (new_array_value == NULL) {
            pjson_value_free(output_value);
            return NULL;
        }
        if (pjson_array_add(output_array, new_array_value) != PJSONSuccess) {
            pjson_value_free(new_array_value);
            pjson_value_free(output_value);
            return NULL;
        }
        SKIP_WHITESPACES(string);
        if (**string != ',') {
            break;
        }
        SKIP_CHAR(string);
        SKIP_WHITESPACES(string);
    }
    SKIP_WHITESPACES(string);
    if (**string != ']' || /* Trim array after parsing is over */
        pjson_array_resize(output_array, pjson_array_get_count(output_array)) != PJSONSuccess) {
            pjson_value_free(output_value);
            return NULL;
    }
    SKIP_CHAR(string);
    return output_value;
}

static PJSON_Value * parse_string_value(const char **string) {
    PJSON_Value *value = NULL;
    size_t new_string_len = 0;
    char *new_string = get_quoted_string(string, &new_string_len);
    if (new_string == NULL) {
        return NULL;
    }
    value = pjson_value_init_string_no_copy(new_string, new_string_len);
    if (value == NULL) {
        parson_free(new_string);
        return NULL;
    }
    return value;
}

static PJSON_Value * parse_boolean_value(const char **string) {
    size_t true_token_size = SIZEOF_TOKEN("true");
    size_t false_token_size = SIZEOF_TOKEN("false");
    if (strncmp("true", *string, true_token_size) == 0) {
        *string += true_token_size;
        return pjson_value_init_boolean(1);
    } else if (strncmp("false", *string, false_token_size) == 0) {
        *string += false_token_size;
        return pjson_value_init_boolean(0);
    }
    return NULL;
}

static PJSON_Value * parse_number_value(const char **string) {
    char *end;
    double number = 0;
    errno = 0;
    number = strtod(*string, &end);
    if (errno == ERANGE && (number <= -HUGE_VAL || number >= HUGE_VAL)) {
        return NULL;
    }
    if ((errno && errno != ERANGE) || !is_decimal(*string, end - *string)) {
        return NULL;
    }
    *string = end;
    return pjson_value_init_number(number);
}

static PJSON_Value * parse_null_value(const char **string) {
    size_t token_size = SIZEOF_TOKEN("null");
    if (strncmp("null", *string, token_size) == 0) {
        *string += token_size;
        return pjson_value_init_null();
    }
    return NULL;
}

/* Serialization */
#define APPEND_STRING(str) do { written = append_string(buf, (str));\
                                if (written < 0) { return -1; }\
                                if (buf != NULL) { buf += written; }\
                                written_total += written; } while(0)

#define APPEND_INDENT(level) do { written = append_indent(buf, (level));\
                                  if (written < 0) { return -1; }\
                                  if (buf != NULL) { buf += written; }\
                                  written_total += written; } while(0)

static int pjson_serialize_to_buffer_r(const PJSON_Value *value, char *buf, int level, parson_bool_t is_pretty, char *num_buf)
{
    const char *key = NULL, *string = NULL;
    PJSON_Value *temp_value = NULL;
    PJSON_Array *array = NULL;
    PJSON_Object *object = NULL;
    size_t i = 0, count = 0;
    double num = 0.0;
    int written = -1, written_total = 0;
    size_t len = 0;

    switch (pjson_value_get_type(value)) {
        case PJSONArray:
            array = pjson_value_get_array(value);
            count = pjson_array_get_count(array);
            APPEND_STRING("[");
            if (count > 0 && is_pretty) {
                APPEND_STRING("\n");
            }
            for (i = 0; i < count; i++) {
                if (is_pretty) {
                    APPEND_INDENT(level+1);
                }
                temp_value = pjson_array_get_value(array, i);
                written = pjson_serialize_to_buffer_r(temp_value, buf, level+1, is_pretty, num_buf);
                if (written < 0) {
                    return -1;
                }
                if (buf != NULL) {
                    buf += written;
                }
                written_total += written;
                if (i < (count - 1)) {
                    APPEND_STRING(",");
                }
                if (is_pretty) {
                    APPEND_STRING("\n");
                }
            }
            if (count > 0 && is_pretty) {
                APPEND_INDENT(level);
            }
            APPEND_STRING("]");
            return written_total;
        case PJSONObject:
            object = pjson_value_get_object(value);
            count  = pjson_object_get_count(object);
            APPEND_STRING("{");
            if (count > 0 && is_pretty) {
                APPEND_STRING("\n");
            }
            for (i = 0; i < count; i++) {
                key = pjson_object_get_name(object, i);
                if (key == NULL) {
                    return -1;
                }
                if (is_pretty) {
                    APPEND_INDENT(level+1);
                }
                /* We do not support key names with embedded \0 chars */
                written = pjson_serialize_string(key, strlen(key), buf);
                if (written < 0) {
                    return -1;
                }
                if (buf != NULL) {
                    buf += written;
                }
                written_total += written;
                APPEND_STRING(":");
                if (is_pretty) {
                    APPEND_STRING(" ");
                }
                temp_value = pjson_object_get_value_at(object, i);
                written = pjson_serialize_to_buffer_r(temp_value, buf, level+1, is_pretty, num_buf);
                if (written < 0) {
                    return -1;
                }
                if (buf != NULL) {
                    buf += written;
                }
                written_total += written;
                if (i < (count - 1)) {
                    APPEND_STRING(",");
                }
                if (is_pretty) {
                    APPEND_STRING("\n");
                }
            }
            if (count > 0 && is_pretty) {
                APPEND_INDENT(level);
            }
            APPEND_STRING("}");
            return written_total;
        case PJSONString:
            string = pjson_value_get_string(value);
            if (string == NULL) {
                return -1;
            }
            len = pjson_value_get_string_len(value);
            written = pjson_serialize_string(string, len, buf);
            if (written < 0) {
                return -1;
            }
            if (buf != NULL) {
                buf += written;
            }
            written_total += written;
            return written_total;
        case PJSONBoolean:
            if (pjson_value_get_boolean(value)) {
                APPEND_STRING("true");
            } else {
                APPEND_STRING("false");
            }
            return written_total;
        case PJSONNumber:
            num = pjson_value_get_number(value);
            if (buf != NULL) {
                num_buf = buf;
            }
            if (parson_float_format) {
                written = sprintf(num_buf, parson_float_format, num);
            } else {
                written = sprintf(num_buf, PARSON_DEFAULT_FLOAT_FORMAT, num);
            }
            if (written < 0) {
                return -1;
            }
            if (buf != NULL) {
                buf += written;
            }
            written_total += written;
            return written_total;
        case PJSONNull:
            APPEND_STRING("null");
            return written_total;
        case PJSONError:
            return -1;
        default:
            return -1;
    }
}

static int pjson_serialize_string(const char *string, size_t len, char *buf) {
    size_t i = 0;
    char c = '\0';
    int written = -1, written_total = 0;
    APPEND_STRING("\"");
    for (i = 0; i < len; i++) {
        c = string[i];
        switch (c) {
            case '\"': APPEND_STRING("\\\""); break;
            case '\\': APPEND_STRING("\\\\"); break;
            case '\b': APPEND_STRING("\\b"); break;
            case '\f': APPEND_STRING("\\f"); break;
            case '\n': APPEND_STRING("\\n"); break;
            case '\r': APPEND_STRING("\\r"); break;
            case '\t': APPEND_STRING("\\t"); break;
            case '\x00': APPEND_STRING("\\u0000"); break;
            case '\x01': APPEND_STRING("\\u0001"); break;
            case '\x02': APPEND_STRING("\\u0002"); break;
            case '\x03': APPEND_STRING("\\u0003"); break;
            case '\x04': APPEND_STRING("\\u0004"); break;
            case '\x05': APPEND_STRING("\\u0005"); break;
            case '\x06': APPEND_STRING("\\u0006"); break;
            case '\x07': APPEND_STRING("\\u0007"); break;
            /* '\x08' duplicate: '\b' */
            /* '\x09' duplicate: '\t' */
            /* '\x0a' duplicate: '\n' */
            case '\x0b': APPEND_STRING("\\u000b"); break;
            /* '\x0c' duplicate: '\f' */
            /* '\x0d' duplicate: '\r' */
            case '\x0e': APPEND_STRING("\\u000e"); break;
            case '\x0f': APPEND_STRING("\\u000f"); break;
            case '\x10': APPEND_STRING("\\u0010"); break;
            case '\x11': APPEND_STRING("\\u0011"); break;
            case '\x12': APPEND_STRING("\\u0012"); break;
            case '\x13': APPEND_STRING("\\u0013"); break;
            case '\x14': APPEND_STRING("\\u0014"); break;
            case '\x15': APPEND_STRING("\\u0015"); break;
            case '\x16': APPEND_STRING("\\u0016"); break;
            case '\x17': APPEND_STRING("\\u0017"); break;
            case '\x18': APPEND_STRING("\\u0018"); break;
            case '\x19': APPEND_STRING("\\u0019"); break;
            case '\x1a': APPEND_STRING("\\u001a"); break;
            case '\x1b': APPEND_STRING("\\u001b"); break;
            case '\x1c': APPEND_STRING("\\u001c"); break;
            case '\x1d': APPEND_STRING("\\u001d"); break;
            case '\x1e': APPEND_STRING("\\u001e"); break;
            case '\x1f': APPEND_STRING("\\u001f"); break;
            case '/':
                if (parson_escape_slashes) {
                    APPEND_STRING("\\/");  /* to make json embeddable in xml\/html */
                } else {
                    APPEND_STRING("/");
                }
                break;
            default:
                if (buf != NULL) {
                    buf[0] = c;
                    buf += 1;
                }
                written_total += 1;
                break;
        }
    }
    APPEND_STRING("\"");
    return written_total;
}

static int append_indent(char *buf, int level) {
    int i;
    int written = -1, written_total = 0;
    for (i = 0; i < level; i++) {
        APPEND_STRING("    ");
    }
    return written_total;
}

static int append_string(char *buf, const char *string) {
    if (buf == NULL) {
        return (int)strlen(string);
    }
    return sprintf(buf, "%s", string);
}

#undef APPEND_STRING
#undef APPEND_INDENT

/* Parser API */
PJSON_Value * pjson_parse_file(const char *filename) {
    char *file_contents = read_file(filename);
    PJSON_Value *output_value = NULL;
    if (file_contents == NULL) {
        return NULL;
    }
    output_value = pjson_parse_string(file_contents);
    parson_free(file_contents);
    return output_value;
}

PJSON_Value * pjson_parse_file_with_comments(const char *filename) {
    char *file_contents = read_file(filename);
    PJSON_Value *output_value = NULL;
    if (file_contents == NULL) {
        return NULL;
    }
    output_value = pjson_parse_string_with_comments(file_contents);
    parson_free(file_contents);
    return output_value;
}

PJSON_Value * pjson_parse_string(const char *string) {
    if (string == NULL) {
        return NULL;
    }
    if (string[0] == '\xEF' && string[1] == '\xBB' && string[2] == '\xBF') {
        string = string + 3; /* Support for UTF-8 BOM */
    }
    return parse_value((const char**)&string, 0);
}

PJSON_Value * pjson_parse_string_with_comments(const char *string) {
    PJSON_Value *result = NULL;
    char *string_mutable_copy = NULL, *string_mutable_copy_ptr = NULL;
    string_mutable_copy = parson_strdup(string);
    if (string_mutable_copy == NULL) {
        return NULL;
    }
    remove_comments(string_mutable_copy, "/*", "*/");
    remove_comments(string_mutable_copy, "//", "\n");
    string_mutable_copy_ptr = string_mutable_copy;
    result = parse_value((const char**)&string_mutable_copy_ptr, 0);
    parson_free(string_mutable_copy);
    return result;
}

/* PJSON Object API */

PJSON_Value * pjson_object_get_value(const PJSON_Object *object, const char *name) {
    if (object == NULL || name == NULL) {
        return NULL;
    }
    return pjson_object_getn_value(object, name, strlen(name));
}

const char * pjson_object_get_string(const PJSON_Object *object, const char *name) {
    return pjson_value_get_string(pjson_object_get_value(object, name));
}

size_t pjson_object_get_string_len(const PJSON_Object *object, const char *name) {
    return pjson_value_get_string_len(pjson_object_get_value(object, name));
}

double pjson_object_get_number(const PJSON_Object *object, const char *name) {
    return pjson_value_get_number(pjson_object_get_value(object, name));
}

PJSON_Object * pjson_object_get_object(const PJSON_Object *object, const char *name) {
    return pjson_value_get_object(pjson_object_get_value(object, name));
}

PJSON_Array * pjson_object_get_array(const PJSON_Object *object, const char *name) {
    return pjson_value_get_array(pjson_object_get_value(object, name));
}

int pjson_object_get_boolean(const PJSON_Object *object, const char *name) {
    return pjson_value_get_boolean(pjson_object_get_value(object, name));
}

PJSON_Value * pjson_object_dotget_value(const PJSON_Object *object, const char *name) {
    const char *dot_position = strchr(name, '.');
    if (!dot_position) {
        return pjson_object_get_value(object, name);
    }
    object = pjson_value_get_object(pjson_object_getn_value(object, name, dot_position - name));
    return pjson_object_dotget_value(object, dot_position + 1);
}

const char * pjson_object_dotget_string(const PJSON_Object *object, const char *name) {
    return pjson_value_get_string(pjson_object_dotget_value(object, name));
}

size_t pjson_object_dotget_string_len(const PJSON_Object *object, const char *name) {
    return pjson_value_get_string_len(pjson_object_dotget_value(object, name));
}

double pjson_object_dotget_number(const PJSON_Object *object, const char *name) {
    return pjson_value_get_number(pjson_object_dotget_value(object, name));
}

PJSON_Object * pjson_object_dotget_object(const PJSON_Object *object, const char *name) {
    return pjson_value_get_object(pjson_object_dotget_value(object, name));
}

PJSON_Array * pjson_object_dotget_array(const PJSON_Object *object, const char *name) {
    return pjson_value_get_array(pjson_object_dotget_value(object, name));
}

int pjson_object_dotget_boolean(const PJSON_Object *object, const char *name) {
    return pjson_value_get_boolean(pjson_object_dotget_value(object, name));
}

size_t pjson_object_get_count(const PJSON_Object *object) {
    return object ? object->count : 0;
}

const char * pjson_object_get_name(const PJSON_Object *object, size_t index) {
    if (object == NULL || index >= pjson_object_get_count(object)) {
        return NULL;
    }
    return object->names[index];
}

PJSON_Value * pjson_object_get_value_at(const PJSON_Object *object, size_t index) {
    if (object == NULL || index >= pjson_object_get_count(object)) {
        return NULL;
    }
    return object->values[index];
}

PJSON_Value *pjson_object_get_wrapping_value(const PJSON_Object *object) {
    if (!object) {
        return NULL;
    }
    return object->wrapping_value;
}

int pjson_object_has_value (const PJSON_Object *object, const char *name) {
    return pjson_object_get_value(object, name) != NULL;
}

int pjson_object_has_value_of_type(const PJSON_Object *object, const char *name, PJSON_Value_Type type) {
    PJSON_Value *val = pjson_object_get_value(object, name);
    return val != NULL && pjson_value_get_type(val) == type;
}

int pjson_object_dothas_value (const PJSON_Object *object, const char *name) {
    return pjson_object_dotget_value(object, name) != NULL;
}

int pjson_object_dothas_value_of_type(const PJSON_Object *object, const char *name, PJSON_Value_Type type) {
    PJSON_Value *val = pjson_object_dotget_value(object, name);
    return val != NULL && pjson_value_get_type(val) == type;
}

/* PJSON Array API */
PJSON_Value * pjson_array_get_value(const PJSON_Array *array, size_t index) {
    if (array == NULL || index >= pjson_array_get_count(array)) {
        return NULL;
    }
    return array->items[index];
}

const char * pjson_array_get_string(const PJSON_Array *array, size_t index) {
    return pjson_value_get_string(pjson_array_get_value(array, index));
}

size_t pjson_array_get_string_len(const PJSON_Array *array, size_t index) {
    return pjson_value_get_string_len(pjson_array_get_value(array, index));
}

double pjson_array_get_number(const PJSON_Array *array, size_t index) {
    return pjson_value_get_number(pjson_array_get_value(array, index));
}

PJSON_Object * pjson_array_get_object(const PJSON_Array *array, size_t index) {
    return pjson_value_get_object(pjson_array_get_value(array, index));
}

PJSON_Array * pjson_array_get_array(const PJSON_Array *array, size_t index) {
    return pjson_value_get_array(pjson_array_get_value(array, index));
}

int pjson_array_get_boolean(const PJSON_Array *array, size_t index) {
    return pjson_value_get_boolean(pjson_array_get_value(array, index));
}

size_t pjson_array_get_count(const PJSON_Array *array) {
    return array ? array->count : 0;
}

PJSON_Value * pjson_array_get_wrapping_value(const PJSON_Array *array) {
    if (!array) {
        return NULL;
    }
    return array->wrapping_value;
}

/* PJSON Value API */
PJSON_Value_Type pjson_value_get_type(const PJSON_Value *value) {
    return value ? value->type : PJSONError;
}

PJSON_Object * pjson_value_get_object(const PJSON_Value *value) {
    return pjson_value_get_type(value) == PJSONObject ? value->value.object : NULL;
}

PJSON_Array * pjson_value_get_array(const PJSON_Value *value) {
    return pjson_value_get_type(value) == PJSONArray ? value->value.array : NULL;
}

static const PJSON_String * pjson_value_get_string_desc(const PJSON_Value *value) {
    return pjson_value_get_type(value) == PJSONString ? &value->value.string : NULL;
}

const char * pjson_value_get_string(const PJSON_Value *value) {
    const PJSON_String *str = pjson_value_get_string_desc(value);
    return str ? str->chars : NULL;
}

size_t pjson_value_get_string_len(const PJSON_Value *value) {
    const PJSON_String *str = pjson_value_get_string_desc(value);
    return str ? str->length : 0;
}

double pjson_value_get_number(const PJSON_Value *value) {
    return pjson_value_get_type(value) == PJSONNumber ? value->value.number : 0;
}

int pjson_value_get_boolean(const PJSON_Value *value) {
    return pjson_value_get_type(value) == PJSONBoolean ? value->value.boolean : -1;
}

PJSON_Value * pjson_value_get_parent (const PJSON_Value *value) {
    return value ? value->parent : NULL;
}

void pjson_value_free(PJSON_Value *value) {
    switch (pjson_value_get_type(value)) {
        case PJSONObject:
            pjson_object_free(value->value.object);
            break;
        case PJSONString:
            parson_free(value->value.string.chars);
            break;
        case PJSONArray:
            pjson_array_free(value->value.array);
            break;
        default:
            break;
    }
    parson_free(value);
}

PJSON_Value * pjson_value_init_object(void) {
    PJSON_Value *new_value = (PJSON_Value*)parson_malloc(sizeof(PJSON_Value));
    if (!new_value) {
        return NULL;
    }
    new_value->parent = NULL;
    new_value->type = PJSONObject;
    new_value->value.object = pjson_object_make(new_value);
    if (!new_value->value.object) {
        parson_free(new_value);
        return NULL;
    }
    return new_value;
}

PJSON_Value * pjson_value_init_array(void) {
    PJSON_Value *new_value = (PJSON_Value*)parson_malloc(sizeof(PJSON_Value));
    if (!new_value) {
        return NULL;
    }
    new_value->parent = NULL;
    new_value->type = PJSONArray;
    new_value->value.array = pjson_array_make(new_value);
    if (!new_value->value.array) {
        parson_free(new_value);
        return NULL;
    }
    return new_value;
}

PJSON_Value * pjson_value_init_string(const char *string) {
    if (string == NULL) {
        return NULL;
    }
    return pjson_value_init_string_with_len(string, strlen(string));
}

PJSON_Value * pjson_value_init_string_with_len(const char *string, size_t length) {
    char *copy = NULL;
    PJSON_Value *value;
    if (string == NULL) {
        return NULL;
    }
    if (!is_valid_utf8(string, length)) {
        return NULL;
    }
    copy = parson_strndup(string, length);
    if (copy == NULL) {
        return NULL;
    }
    value = pjson_value_init_string_no_copy(copy, length);
    if (value == NULL) {
        parson_free(copy);
    }
    return value;
}

PJSON_Value * pjson_value_init_number(double number) {
    PJSON_Value *new_value = NULL;
    if (IS_NUMBER_INVALID(number)) {
        return NULL;
    }
    new_value = (PJSON_Value*)parson_malloc(sizeof(PJSON_Value));
    if (new_value == NULL) {
        return NULL;
    }
    new_value->parent = NULL;
    new_value->type = PJSONNumber;
    new_value->value.number = number;
    return new_value;
}

PJSON_Value * pjson_value_init_boolean(int boolean) {
    PJSON_Value *new_value = (PJSON_Value*)parson_malloc(sizeof(PJSON_Value));
    if (!new_value) {
        return NULL;
    }
    new_value->parent = NULL;
    new_value->type = PJSONBoolean;
    new_value->value.boolean = boolean ? 1 : 0;
    return new_value;
}

PJSON_Value * pjson_value_init_null(void) {
    PJSON_Value *new_value = (PJSON_Value*)parson_malloc(sizeof(PJSON_Value));
    if (!new_value) {
        return NULL;
    }
    new_value->parent = NULL;
    new_value->type = PJSONNull;
    return new_value;
}

PJSON_Value * pjson_value_deep_copy(const PJSON_Value *value) {
    size_t i = 0;
    PJSON_Value *return_value = NULL, *temp_value_copy = NULL, *temp_value = NULL;
    const PJSON_String *temp_string = NULL;
    const char *temp_key = NULL;
    char *temp_string_copy = NULL;
    PJSON_Array *temp_array = NULL, *temp_array_copy = NULL;
    PJSON_Object *temp_object = NULL, *temp_object_copy = NULL;
    PJSON_Status res = PJSONFailure;
    char *key_copy = NULL;

    switch (pjson_value_get_type(value)) {
        case PJSONArray:
            temp_array = pjson_value_get_array(value);
            return_value = pjson_value_init_array();
            if (return_value == NULL) {
                return NULL;
            }
            temp_array_copy = pjson_value_get_array(return_value);
            for (i = 0; i < pjson_array_get_count(temp_array); i++) {
                temp_value = pjson_array_get_value(temp_array, i);
                temp_value_copy = pjson_value_deep_copy(temp_value);
                if (temp_value_copy == NULL) {
                    pjson_value_free(return_value);
                    return NULL;
                }
                if (pjson_array_add(temp_array_copy, temp_value_copy) != PJSONSuccess) {
                    pjson_value_free(return_value);
                    pjson_value_free(temp_value_copy);
                    return NULL;
                }
            }
            return return_value;
        case PJSONObject:
            temp_object = pjson_value_get_object(value);
            return_value = pjson_value_init_object();
            if (!return_value) {
                return NULL;
            }
            temp_object_copy = pjson_value_get_object(return_value);
            for (i = 0; i < pjson_object_get_count(temp_object); i++) {
                temp_key = pjson_object_get_name(temp_object, i);
                temp_value = pjson_object_get_value(temp_object, temp_key);
                temp_value_copy = pjson_value_deep_copy(temp_value);
                if (!temp_value_copy) {
                    pjson_value_free(return_value);
                    return NULL;
                }
                key_copy = parson_strdup(temp_key);
                if (!key_copy) {
                    pjson_value_free(temp_value_copy);
                    pjson_value_free(return_value);
                    return NULL;
                }
                res = pjson_object_add(temp_object_copy, key_copy, temp_value_copy);
                if (res != PJSONSuccess) {
                    parson_free(key_copy);
                    pjson_value_free(temp_value_copy);
                    pjson_value_free(return_value);
                    return NULL;
                }
            }
            return return_value;
        case PJSONBoolean:
            return pjson_value_init_boolean(pjson_value_get_boolean(value));
        case PJSONNumber:
            return pjson_value_init_number(pjson_value_get_number(value));
        case PJSONString:
            temp_string = pjson_value_get_string_desc(value);
            if (temp_string == NULL) {
                return NULL;
            }
            temp_string_copy = parson_strndup(temp_string->chars, temp_string->length);
            if (temp_string_copy == NULL) {
                return NULL;
            }
            return_value = pjson_value_init_string_no_copy(temp_string_copy, temp_string->length);
            if (return_value == NULL) {
                parson_free(temp_string_copy);
            }
            return return_value;
        case PJSONNull:
            return pjson_value_init_null();
        case PJSONError:
            return NULL;
        default:
            return NULL;
    }
}

size_t pjson_serialization_size(const PJSON_Value *value) {
    char num_buf[PARSON_NUM_BUF_SIZE]; /* recursively allocating buffer on stack is a bad idea, so let's do it only once */
    int res = pjson_serialize_to_buffer_r(value, NULL, 0, PARSON_FALSE, num_buf);
    return res < 0 ? 0 : (size_t)(res) + 1;
}

PJSON_Status pjson_serialize_to_buffer(const PJSON_Value *value, char *buf, size_t buf_size_in_bytes) {
    int written = -1;
    size_t needed_size_in_bytes = pjson_serialization_size(value);
    if (needed_size_in_bytes == 0 || buf_size_in_bytes < needed_size_in_bytes) {
        return PJSONFailure;
    }
    written = pjson_serialize_to_buffer_r(value, buf, 0, PARSON_FALSE, NULL);
    if (written < 0) {
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_serialize_to_file(const PJSON_Value *value, const char *filename) {
    PJSON_Status return_code = PJSONSuccess;
    FILE *fp = NULL;
    char *serialized_string = pjson_serialize_to_string(value);
    if (serialized_string == NULL) {
        return PJSONFailure;
    }
    fp = fopen(filename, "w");
    if (fp == NULL) {
        pjson_free_serialized_string(serialized_string);
        return PJSONFailure;
    }
    if (fputs(serialized_string, fp) == EOF) {
        return_code = PJSONFailure;
    }
    if (fclose(fp) == EOF) {
        return_code = PJSONFailure;
    }
    pjson_free_serialized_string(serialized_string);
    return return_code;
}

char * pjson_serialize_to_string(const PJSON_Value *value) {
    PJSON_Status serialization_result = PJSONFailure;
    size_t buf_size_bytes = pjson_serialization_size(value);
    char *buf = NULL;
    if (buf_size_bytes == 0) {
        return NULL;
    }
    buf = (char*)parson_malloc(buf_size_bytes);
    if (buf == NULL) {
        return NULL;
    }
    serialization_result = pjson_serialize_to_buffer(value, buf, buf_size_bytes);
    if (serialization_result != PJSONSuccess) {
        pjson_free_serialized_string(buf);
        return NULL;
    }
    return buf;
}

size_t pjson_serialization_size_pretty(const PJSON_Value *value) {
    char num_buf[PARSON_NUM_BUF_SIZE]; /* recursively allocating buffer on stack is a bad idea, so let's do it only once */
    int res = pjson_serialize_to_buffer_r(value, NULL, 0, PARSON_TRUE, num_buf);
    return res < 0 ? 0 : (size_t)(res) + 1;
}

PJSON_Status pjson_serialize_to_buffer_pretty(const PJSON_Value *value, char *buf, size_t buf_size_in_bytes) {
    int written = -1;
    size_t needed_size_in_bytes = pjson_serialization_size_pretty(value);
    if (needed_size_in_bytes == 0 || buf_size_in_bytes < needed_size_in_bytes) {
        return PJSONFailure;
    }
    written = pjson_serialize_to_buffer_r(value, buf, 0, PARSON_TRUE, NULL);
    if (written < 0) {
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_serialize_to_file_pretty(const PJSON_Value *value, const char *filename) {
    PJSON_Status return_code = PJSONSuccess;
    FILE *fp = NULL;
    char *serialized_string = pjson_serialize_to_string_pretty(value);
    if (serialized_string == NULL) {
        return PJSONFailure;
    }
    fp = fopen(filename, "w");
    if (fp == NULL) {
        pjson_free_serialized_string(serialized_string);
        return PJSONFailure;
    }
    if (fputs(serialized_string, fp) == EOF) {
        return_code = PJSONFailure;
    }
    if (fclose(fp) == EOF) {
        return_code = PJSONFailure;
    }
    pjson_free_serialized_string(serialized_string);
    return return_code;
}

char * pjson_serialize_to_string_pretty(const PJSON_Value *value) {
    PJSON_Status serialization_result = PJSONFailure;
    size_t buf_size_bytes = pjson_serialization_size_pretty(value);
    char *buf = NULL;
    if (buf_size_bytes == 0) {
        return NULL;
    }
    buf = (char*)parson_malloc(buf_size_bytes);
    if (buf == NULL) {
        return NULL;
    }
    serialization_result = pjson_serialize_to_buffer_pretty(value, buf, buf_size_bytes);
    if (serialization_result != PJSONSuccess) {
        pjson_free_serialized_string(buf);
        return NULL;
    }
    return buf;
}

void pjson_free_serialized_string(char *string) {
    parson_free(string);
}

PJSON_Status pjson_array_remove(PJSON_Array *array, size_t ix) {
    size_t to_move_bytes = 0;
    if (array == NULL || ix >= pjson_array_get_count(array)) {
        return PJSONFailure;
    }
    pjson_value_free(pjson_array_get_value(array, ix));
    to_move_bytes = (pjson_array_get_count(array) - 1 - ix) * sizeof(PJSON_Value*);
    memmove(array->items + ix, array->items + ix + 1, to_move_bytes);
    array->count -= 1;
    return PJSONSuccess;
}

PJSON_Status pjson_array_replace_value(PJSON_Array *array, size_t ix, PJSON_Value *value) {
    if (array == NULL || value == NULL || value->parent != NULL || ix >= pjson_array_get_count(array)) {
        return PJSONFailure;
    }
    pjson_value_free(pjson_array_get_value(array, ix));
    value->parent = pjson_array_get_wrapping_value(array);
    array->items[ix] = value;
    return PJSONSuccess;
}

PJSON_Status pjson_array_replace_string(PJSON_Array *array, size_t i, const char* string) {
    PJSON_Value *value = pjson_value_init_string(string);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_replace_value(array, i, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_replace_string_with_len(PJSON_Array *array, size_t i, const char *string, size_t len) {
    PJSON_Value *value = pjson_value_init_string_with_len(string, len);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_replace_value(array, i, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_replace_number(PJSON_Array *array, size_t i, double number) {
    PJSON_Value *value = pjson_value_init_number(number);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_replace_value(array, i, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_replace_boolean(PJSON_Array *array, size_t i, int boolean) {
    PJSON_Value *value = pjson_value_init_boolean(boolean);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_replace_value(array, i, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_replace_null(PJSON_Array *array, size_t i) {
    PJSON_Value *value = pjson_value_init_null();
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_replace_value(array, i, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_clear(PJSON_Array *array) {
    size_t i = 0;
    if (array == NULL) {
        return PJSONFailure;
    }
    for (i = 0; i < pjson_array_get_count(array); i++) {
        pjson_value_free(pjson_array_get_value(array, i));
    }
    array->count = 0;
    return PJSONSuccess;
}

PJSON_Status pjson_array_append_value(PJSON_Array *array, PJSON_Value *value) {
    if (array == NULL || value == NULL || value->parent != NULL) {
        return PJSONFailure;
    }
    return pjson_array_add(array, value);
}

PJSON_Status pjson_array_append_string(PJSON_Array *array, const char *string) {
    PJSON_Value *value = pjson_value_init_string(string);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_append_value(array, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_append_string_with_len(PJSON_Array *array, const char *string, size_t len) {
    PJSON_Value *value = pjson_value_init_string_with_len(string, len);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_append_value(array, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_append_number(PJSON_Array *array, double number) {
    PJSON_Value *value = pjson_value_init_number(number);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_append_value(array, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_append_boolean(PJSON_Array *array, int boolean) {
    PJSON_Value *value = pjson_value_init_boolean(boolean);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_append_value(array, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_array_append_null(PJSON_Array *array) {
    PJSON_Value *value = pjson_value_init_null();
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_array_append_value(array, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_object_set_value(PJSON_Object *object, const char *name, PJSON_Value *value) {
    unsigned long hash = 0;
    parson_bool_t found = PARSON_FALSE;
    size_t cell_ix = 0;
    size_t item_ix = 0;
    PJSON_Value *old_value = NULL;
    char *key_copy = NULL;

    if (!object || !name || !value || value->parent) {
        return PJSONFailure;
    }
    hash = hash_string(name, strlen(name));
    found = PARSON_FALSE;
    cell_ix = pjson_object_get_cell_ix(object, name, strlen(name), hash, &found);
    if (found) {
        item_ix = object->cells[cell_ix];
        old_value = object->values[item_ix];
        pjson_value_free(old_value);
        object->values[item_ix] = value;
        value->parent = pjson_object_get_wrapping_value(object);
        return PJSONSuccess;
    }
    if (object->count >= object->item_capacity) {
        PJSON_Status res = pjson_object_grow_and_rehash(object);
        if (res != PJSONSuccess) {
            return PJSONFailure;
        }
        cell_ix = pjson_object_get_cell_ix(object, name, strlen(name), hash, &found);
    }
    key_copy = parson_strdup(name);
    if (!key_copy) {
        return PJSONFailure;
    }
    object->names[object->count] = key_copy;
    object->cells[cell_ix] = object->count;
    object->values[object->count] = value;
    object->cell_ixs[object->count] = cell_ix;
    object->hashes[object->count] = hash;
    object->count++;
    value->parent = pjson_object_get_wrapping_value(object);
    return PJSONSuccess;
}

PJSON_Status pjson_object_set_string(PJSON_Object *object, const char *name, const char *string) {
    PJSON_Value *value = pjson_value_init_string(string);
    PJSON_Status status = pjson_object_set_value(object, name, value);
    if (status != PJSONSuccess) {
        pjson_value_free(value);
    }
    return status;
}

PJSON_Status pjson_object_set_string_with_len(PJSON_Object *object, const char *name, const char *string, size_t len) {
    PJSON_Value *value = pjson_value_init_string_with_len(string, len);
    PJSON_Status status = pjson_object_set_value(object, name, value);
    if (status != PJSONSuccess) {
        pjson_value_free(value);
    }
    return status;
}

PJSON_Status pjson_object_set_number(PJSON_Object *object, const char *name, double number) {
    PJSON_Value *value = pjson_value_init_number(number);
    PJSON_Status status = pjson_object_set_value(object, name, value);
    if (status != PJSONSuccess) {
        pjson_value_free(value);
    }
    return status;
}

PJSON_Status pjson_object_set_boolean(PJSON_Object *object, const char *name, int boolean) {
    PJSON_Value *value = pjson_value_init_boolean(boolean);
    PJSON_Status status = pjson_object_set_value(object, name, value);
    if (status != PJSONSuccess) {
        pjson_value_free(value);
    }
    return status;
}

PJSON_Status pjson_object_set_null(PJSON_Object *object, const char *name) {
    PJSON_Value *value = pjson_value_init_null();
    PJSON_Status status = pjson_object_set_value(object, name, value);
    if (status != PJSONSuccess) {
        pjson_value_free(value);
    }
    return status;
}

PJSON_Status pjson_object_dotset_value(PJSON_Object *object, const char *name, PJSON_Value *value) {
    const char *dot_pos = NULL;
    PJSON_Value *temp_value = NULL, *new_value = NULL;
    PJSON_Object *temp_object = NULL, *new_object = NULL;
    PJSON_Status status = PJSONFailure;
    size_t name_len = 0;
    char *name_copy = NULL;
    
    if (object == NULL || name == NULL || value == NULL) {
        return PJSONFailure;
    }
    dot_pos = strchr(name, '.');
    if (dot_pos == NULL) {
        return pjson_object_set_value(object, name, value);
    }
    name_len = dot_pos - name;
    temp_value = pjson_object_getn_value(object, name, name_len);
    if (temp_value) {
        /* Don't overwrite existing non-object (unlike pjson_object_set_value, but it shouldn't be changed at this point) */
        if (pjson_value_get_type(temp_value) != PJSONObject) {
            return PJSONFailure;
        }
        temp_object = pjson_value_get_object(temp_value);
        return pjson_object_dotset_value(temp_object, dot_pos + 1, value);
    }
    new_value = pjson_value_init_object();
    if (new_value == NULL) {
        return PJSONFailure;
    }
    new_object = pjson_value_get_object(new_value);
    status = pjson_object_dotset_value(new_object, dot_pos + 1, value);
    if (status != PJSONSuccess) {
        pjson_value_free(new_value);
        return PJSONFailure;
    }
    name_copy = parson_strndup(name, name_len);
    if (!name_copy) {
        pjson_object_dotremove_internal(new_object, dot_pos + 1, 0);
        pjson_value_free(new_value);
        return PJSONFailure;
    }
    status = pjson_object_add(object, name_copy, new_value);
    if (status != PJSONSuccess) {
        parson_free(name_copy);
        pjson_object_dotremove_internal(new_object, dot_pos + 1, 0);
        pjson_value_free(new_value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_object_dotset_string(PJSON_Object *object, const char *name, const char *string) {
    PJSON_Value *value = pjson_value_init_string(string);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_object_dotset_value(object, name, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_object_dotset_string_with_len(PJSON_Object *object, const char *name, const char *string, size_t len) {
    PJSON_Value *value = pjson_value_init_string_with_len(string, len);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_object_dotset_value(object, name, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_object_dotset_number(PJSON_Object *object, const char *name, double number) {
    PJSON_Value *value = pjson_value_init_number(number);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_object_dotset_value(object, name, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_object_dotset_boolean(PJSON_Object *object, const char *name, int boolean) {
    PJSON_Value *value = pjson_value_init_boolean(boolean);
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_object_dotset_value(object, name, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_object_dotset_null(PJSON_Object *object, const char *name) {
    PJSON_Value *value = pjson_value_init_null();
    if (value == NULL) {
        return PJSONFailure;
    }
    if (pjson_object_dotset_value(object, name, value) != PJSONSuccess) {
        pjson_value_free(value);
        return PJSONFailure;
    }
    return PJSONSuccess;
}

PJSON_Status pjson_object_remove(PJSON_Object *object, const char *name) {
    return pjson_object_remove_internal(object, name, PARSON_TRUE);
}

PJSON_Status pjson_object_dotremove(PJSON_Object *object, const char *name) {
    return pjson_object_dotremove_internal(object, name, PARSON_TRUE);
}

PJSON_Status pjson_object_clear(PJSON_Object *object) {
    size_t i = 0;
    if (object == NULL) {
        return PJSONFailure;
    }
    for (i = 0; i < pjson_object_get_count(object); i++) {
        parson_free(object->names[i]);
        pjson_value_free(object->values[i]);
    }
    object->count = 0;
    return PJSONSuccess;
}

PJSON_Status pjson_validate(const PJSON_Value *schema, const PJSON_Value *value) {
    PJSON_Value *temp_schema_value = NULL, *temp_value = NULL;
    PJSON_Array *schema_array = NULL, *value_array = NULL;
    PJSON_Object *schema_object = NULL, *value_object = NULL;
    PJSON_Value_Type schema_type = PJSONError, value_type = PJSONError;
    const char *key = NULL;
    size_t i = 0, count = 0;
    if (schema == NULL || value == NULL) {
        return PJSONFailure;
    }
    schema_type = pjson_value_get_type(schema);
    value_type = pjson_value_get_type(value);
    if (schema_type != value_type && schema_type != PJSONNull) { /* null represents all values */
        return PJSONFailure;
    }
    switch (schema_type) {
        case PJSONArray:
            schema_array = pjson_value_get_array(schema);
            value_array = pjson_value_get_array(value);
            count = pjson_array_get_count(schema_array);
            if (count == 0) {
                return PJSONSuccess; /* Empty array allows all types */
            }
            /* Get first value from array, rest is ignored */
            temp_schema_value = pjson_array_get_value(schema_array, 0);
            for (i = 0; i < pjson_array_get_count(value_array); i++) {
                temp_value = pjson_array_get_value(value_array, i);
                if (pjson_validate(temp_schema_value, temp_value) != PJSONSuccess) {
                    return PJSONFailure;
                }
            }
            return PJSONSuccess;
        case PJSONObject:
            schema_object = pjson_value_get_object(schema);
            value_object = pjson_value_get_object(value);
            count = pjson_object_get_count(schema_object);
            if (count == 0) {
                return PJSONSuccess; /* Empty object allows all objects */
            } else if (pjson_object_get_count(value_object) < count) {
                return PJSONFailure; /* Tested object mustn't have less name-value pairs than schema */
            }
            for (i = 0; i < count; i++) {
                key = pjson_object_get_name(schema_object, i);
                temp_schema_value = pjson_object_get_value(schema_object, key);
                temp_value = pjson_object_get_value(value_object, key);
                if (temp_value == NULL) {
                    return PJSONFailure;
                }
                if (pjson_validate(temp_schema_value, temp_value) != PJSONSuccess) {
                    return PJSONFailure;
                }
            }
            return PJSONSuccess;
        case PJSONString: case PJSONNumber: case PJSONBoolean: case PJSONNull:
            return PJSONSuccess; /* equality already tested before switch */
        case PJSONError: default:
            return PJSONFailure;
    }
}

int pjson_value_equals(const PJSON_Value *a, const PJSON_Value *b) {
    PJSON_Object *a_object = NULL, *b_object = NULL;
    PJSON_Array *a_array = NULL, *b_array = NULL;
    const PJSON_String *a_string = NULL, *b_string = NULL;
    const char *key = NULL;
    size_t a_count = 0, b_count = 0, i = 0;
    PJSON_Value_Type a_type, b_type;
    a_type = pjson_value_get_type(a);
    b_type = pjson_value_get_type(b);
    if (a_type != b_type) {
        return PARSON_FALSE;
    }
    switch (a_type) {
        case PJSONArray:
            a_array = pjson_value_get_array(a);
            b_array = pjson_value_get_array(b);
            a_count = pjson_array_get_count(a_array);
            b_count = pjson_array_get_count(b_array);
            if (a_count != b_count) {
                return PARSON_FALSE;
            }
            for (i = 0; i < a_count; i++) {
                if (!pjson_value_equals(pjson_array_get_value(a_array, i),
                                       pjson_array_get_value(b_array, i))) {
                    return PARSON_FALSE;
                }
            }
            return PARSON_TRUE;
        case PJSONObject:
            a_object = pjson_value_get_object(a);
            b_object = pjson_value_get_object(b);
            a_count = pjson_object_get_count(a_object);
            b_count = pjson_object_get_count(b_object);
            if (a_count != b_count) {
                return PARSON_FALSE;
            }
            for (i = 0; i < a_count; i++) {
                key = pjson_object_get_name(a_object, i);
                if (!pjson_value_equals(pjson_object_get_value(a_object, key),
                                       pjson_object_get_value(b_object, key))) {
                    return PARSON_FALSE;
                }
            }
            return PARSON_TRUE;
        case PJSONString:
            a_string = pjson_value_get_string_desc(a);
            b_string = pjson_value_get_string_desc(b);
            if (a_string == NULL || b_string == NULL) {
                return PARSON_FALSE; /* shouldn't happen */
            }
            return a_string->length == b_string->length &&
                   memcmp(a_string->chars, b_string->chars, a_string->length) == 0;
        case PJSONBoolean:
            return pjson_value_get_boolean(a) == pjson_value_get_boolean(b);
        case PJSONNumber:
            return fabs(pjson_value_get_number(a) - pjson_value_get_number(b)) < 0.000001; /* EPSILON */
        case PJSONError:
            return PARSON_TRUE;
        case PJSONNull:
            return PARSON_TRUE;
        default:
            return PARSON_TRUE;
    }
}

PJSON_Value_Type pjson_type(const PJSON_Value *value) {
    return pjson_value_get_type(value);
}

PJSON_Object * pjson_object (const PJSON_Value *value) {
    return pjson_value_get_object(value);
}

PJSON_Array * pjson_array(const PJSON_Value *value) {
    return pjson_value_get_array(value);
}

const char * pjson_string(const PJSON_Value *value) {
    return pjson_value_get_string(value);
}

size_t pjson_string_len(const PJSON_Value *value) {
    return pjson_value_get_string_len(value);
}

double pjson_number(const PJSON_Value *value) {
    return pjson_value_get_number(value);
}

int pjson_boolean(const PJSON_Value *value) {
    return pjson_value_get_boolean(value);
}

void pjson_set_allocation_functions(PJSON_Malloc_Function malloc_fun, PJSON_Free_Function free_fun) {
    parson_malloc = malloc_fun;
    parson_free = free_fun;
}

void pjson_set_escape_slashes(int escape_slashes) {
    parson_escape_slashes = escape_slashes;
}

void pjson_set_float_serialization_format(const char *format) {
    if (parson_float_format) {
        parson_free(parson_float_format);
    }
    if (!format) {
        parson_float_format = NULL;
        return;
    }
    parson_float_format = parson_strdup(format);
}
