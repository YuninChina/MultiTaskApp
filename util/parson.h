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

#ifndef parson_parson_h
#define parson_parson_h

#ifdef __cplusplus
extern "C"
{
#endif
#if 0
} /* unconfuse xcode */
#endif

#define PARSON_VERSION_MAJOR 1
#define PARSON_VERSION_MINOR 3
#define PARSON_VERSION_PATCH 0

#define PARSON_VERSION_STRING "1.3.0"

#include <stddef.h>   /* size_t */

/* Types and enums */
typedef struct pjson_object_t PJSON_Object;
typedef struct pjson_array_t  PJSON_Array;
typedef struct pjson_value_t  PJSON_Value;

enum pjson_value_type {
    PJSONError   = -1,
    PJSONNull    = 1,
    PJSONString  = 2,
    PJSONNumber  = 3,
    PJSONObject  = 4,
    PJSONArray   = 5,
    PJSONBoolean = 6
};
typedef int PJSON_Value_Type;

enum pjson_result_t {
    PJSONSuccess = 0,
    PJSONFailure = -1
};
typedef int PJSON_Status;

typedef void * (*PJSON_Malloc_Function)(size_t);
typedef void   (*PJSON_Free_Function)(void *);

/* Call only once, before calling any other function from parson API. If not called, malloc and free
   from stdlib will be used for all allocations */
void pjson_set_allocation_functions(PJSON_Malloc_Function malloc_fun, PJSON_Free_Function free_fun);

/* Sets if slashes should be escaped or not when serializing PJSON. By default slashes are escaped.
 This function sets a global setting and is not thread safe. */
void pjson_set_escape_slashes(int escape_slashes);

/* Sets float format used for serialization of numbers.
   Make sure it can't serialize to a string longer than PARSON_NUM_BUF_SIZE.
   If format is null then the default format is used. */
void pjson_set_float_serialization_format(const char *format);

/* Parses first PJSON value in a file, returns NULL in case of error */
PJSON_Value * pjson_parse_file(const char *filename);

/* Parses first PJSON value in a file and ignores comments (/ * * / and //),
   returns NULL in case of error */
PJSON_Value * pjson_parse_file_with_comments(const char *filename);

/*  Parses first PJSON value in a string, returns NULL in case of error */
PJSON_Value * pjson_parse_string(const char *string);

/*  Parses first PJSON value in a string and ignores comments (/ * * / and //),
    returns NULL in case of error */
PJSON_Value * pjson_parse_string_with_comments(const char *string);

/* Serialization */
size_t      pjson_serialization_size(const PJSON_Value *value); /* returns 0 on fail */
PJSON_Status pjson_serialize_to_buffer(const PJSON_Value *value, char *buf, size_t buf_size_in_bytes);
PJSON_Status pjson_serialize_to_file(const PJSON_Value *value, const char *filename);
char *      pjson_serialize_to_string(const PJSON_Value *value);

/* Pretty serialization */
size_t      pjson_serialization_size_pretty(const PJSON_Value *value); /* returns 0 on fail */
PJSON_Status pjson_serialize_to_buffer_pretty(const PJSON_Value *value, char *buf, size_t buf_size_in_bytes);
PJSON_Status pjson_serialize_to_file_pretty(const PJSON_Value *value, const char *filename);
char *      pjson_serialize_to_string_pretty(const PJSON_Value *value);

void        pjson_free_serialized_string(char *string); /* frees string from pjson_serialize_to_string and pjson_serialize_to_string_pretty */

/* Comparing */
int  pjson_value_equals(const PJSON_Value *a, const PJSON_Value *b);

/* Validation
   This is *NOT* PJSON Schema. It validates json by checking if object have identically
   named fields with matching types.
   For example schema {"name":"", "age":0} will validate
   {"name":"Joe", "age":25} and {"name":"Joe", "age":25, "gender":"m"},
   but not {"name":"Joe"} or {"name":"Joe", "age":"Cucumber"}.
   In case of arrays, only first value in schema is checked against all values in tested array.
   Empty objects ({}) validate all objects, empty arrays ([]) validate all arrays,
   null validates values of every type.
 */
PJSON_Status pjson_validate(const PJSON_Value *schema, const PJSON_Value *value);

/*
 * PJSON Object
 */
PJSON_Value  * pjson_object_get_value  (const PJSON_Object *object, const char *name);
const char  * pjson_object_get_string (const PJSON_Object *object, const char *name);
size_t        pjson_object_get_string_len(const PJSON_Object *object, const char *name); /* doesn't account for last null character */
PJSON_Object * pjson_object_get_object (const PJSON_Object *object, const char *name);
PJSON_Array  * pjson_object_get_array  (const PJSON_Object *object, const char *name);
double        pjson_object_get_number (const PJSON_Object *object, const char *name); /* returns 0 on fail */
int           pjson_object_get_boolean(const PJSON_Object *object, const char *name); /* returns -1 on fail */

/* dotget functions enable addressing values with dot notation in nested objects,
 just like in structs or c++/java/c# objects (e.g. objectA.objectB.value).
 Because valid names in PJSON can contain dots, some values may be inaccessible
 this way. */
PJSON_Value  * pjson_object_dotget_value  (const PJSON_Object *object, const char *name);
const char  * pjson_object_dotget_string (const PJSON_Object *object, const char *name);
size_t        pjson_object_dotget_string_len(const PJSON_Object *object, const char *name); /* doesn't account for last null character */
PJSON_Object * pjson_object_dotget_object (const PJSON_Object *object, const char *name);
PJSON_Array  * pjson_object_dotget_array  (const PJSON_Object *object, const char *name);
double        pjson_object_dotget_number (const PJSON_Object *object, const char *name); /* returns 0 on fail */
int           pjson_object_dotget_boolean(const PJSON_Object *object, const char *name); /* returns -1 on fail */

/* Functions to get available names */
size_t        pjson_object_get_count   (const PJSON_Object *object);
const char  * pjson_object_get_name    (const PJSON_Object *object, size_t index);
PJSON_Value  * pjson_object_get_value_at(const PJSON_Object *object, size_t index);
PJSON_Value  * pjson_object_get_wrapping_value(const PJSON_Object *object);

/* Functions to check if object has a value with a specific name. Returned value is 1 if object has
 * a value and 0 if it doesn't. dothas functions behave exactly like dotget functions. */
int pjson_object_has_value        (const PJSON_Object *object, const char *name);
int pjson_object_has_value_of_type(const PJSON_Object *object, const char *name, PJSON_Value_Type type);

int pjson_object_dothas_value        (const PJSON_Object *object, const char *name);
int pjson_object_dothas_value_of_type(const PJSON_Object *object, const char *name, PJSON_Value_Type type);

/* Creates new name-value pair or frees and replaces old value with a new one.
 * pjson_object_set_value does not copy passed value so it shouldn't be freed afterwards. */
PJSON_Status pjson_object_set_value(PJSON_Object *object, const char *name, PJSON_Value *value);
PJSON_Status pjson_object_set_string(PJSON_Object *object, const char *name, const char *string);
PJSON_Status pjson_object_set_string_with_len(PJSON_Object *object, const char *name, const char *string, size_t len);  /* length shouldn't include last null character */
PJSON_Status pjson_object_set_number(PJSON_Object *object, const char *name, double number);
PJSON_Status pjson_object_set_boolean(PJSON_Object *object, const char *name, int boolean);
PJSON_Status pjson_object_set_null(PJSON_Object *object, const char *name);

/* Works like dotget functions, but creates whole hierarchy if necessary.
 * pjson_object_dotset_value does not copy passed value so it shouldn't be freed afterwards. */
PJSON_Status pjson_object_dotset_value(PJSON_Object *object, const char *name, PJSON_Value *value);
PJSON_Status pjson_object_dotset_string(PJSON_Object *object, const char *name, const char *string);
PJSON_Status pjson_object_dotset_string_with_len(PJSON_Object *object, const char *name, const char *string, size_t len); /* length shouldn't include last null character */
PJSON_Status pjson_object_dotset_number(PJSON_Object *object, const char *name, double number);
PJSON_Status pjson_object_dotset_boolean(PJSON_Object *object, const char *name, int boolean);
PJSON_Status pjson_object_dotset_null(PJSON_Object *object, const char *name);

/* Frees and removes name-value pair */
PJSON_Status pjson_object_remove(PJSON_Object *object, const char *name);

/* Works like dotget function, but removes name-value pair only on exact match. */
PJSON_Status pjson_object_dotremove(PJSON_Object *object, const char *key);

/* Removes all name-value pairs in object */
PJSON_Status pjson_object_clear(PJSON_Object *object);

/*
 *PJSON Array
 */
PJSON_Value  * pjson_array_get_value  (const PJSON_Array *array, size_t index);
const char  * pjson_array_get_string (const PJSON_Array *array, size_t index);
size_t        pjson_array_get_string_len(const PJSON_Array *array, size_t index); /* doesn't account for last null character */
PJSON_Object * pjson_array_get_object (const PJSON_Array *array, size_t index);
PJSON_Array  * pjson_array_get_array  (const PJSON_Array *array, size_t index);
double        pjson_array_get_number (const PJSON_Array *array, size_t index); /* returns 0 on fail */
int           pjson_array_get_boolean(const PJSON_Array *array, size_t index); /* returns -1 on fail */
size_t        pjson_array_get_count  (const PJSON_Array *array);
PJSON_Value  * pjson_array_get_wrapping_value(const PJSON_Array *array);

/* Frees and removes value at given index, does nothing and returns PJSONFailure if index doesn't exist.
 * Order of values in array may change during execution.  */
PJSON_Status pjson_array_remove(PJSON_Array *array, size_t i);

/* Frees and removes from array value at given index and replaces it with given one.
 * Does nothing and returns PJSONFailure if index doesn't exist.
 * pjson_array_replace_value does not copy passed value so it shouldn't be freed afterwards. */
PJSON_Status pjson_array_replace_value(PJSON_Array *array, size_t i, PJSON_Value *value);
PJSON_Status pjson_array_replace_string(PJSON_Array *array, size_t i, const char* string);
PJSON_Status pjson_array_replace_string_with_len(PJSON_Array *array, size_t i, const char *string, size_t len); /* length shouldn't include last null character */
PJSON_Status pjson_array_replace_number(PJSON_Array *array, size_t i, double number);
PJSON_Status pjson_array_replace_boolean(PJSON_Array *array, size_t i, int boolean);
PJSON_Status pjson_array_replace_null(PJSON_Array *array, size_t i);

/* Frees and removes all values from array */
PJSON_Status pjson_array_clear(PJSON_Array *array);

/* Appends new value at the end of array.
 * pjson_array_append_value does not copy passed value so it shouldn't be freed afterwards. */
PJSON_Status pjson_array_append_value(PJSON_Array *array, PJSON_Value *value);
PJSON_Status pjson_array_append_string(PJSON_Array *array, const char *string);
PJSON_Status pjson_array_append_string_with_len(PJSON_Array *array, const char *string, size_t len); /* length shouldn't include last null character */
PJSON_Status pjson_array_append_number(PJSON_Array *array, double number);
PJSON_Status pjson_array_append_boolean(PJSON_Array *array, int boolean);
PJSON_Status pjson_array_append_null(PJSON_Array *array);

/*
 *PJSON Value
 */
PJSON_Value * pjson_value_init_object (void);
PJSON_Value * pjson_value_init_array  (void);
PJSON_Value * pjson_value_init_string (const char *string); /* copies passed string */
PJSON_Value * pjson_value_init_string_with_len(const char *string, size_t length); /* copies passed string, length shouldn't include last null character */
PJSON_Value * pjson_value_init_number (double number);
PJSON_Value * pjson_value_init_boolean(int boolean);
PJSON_Value * pjson_value_init_null   (void);
PJSON_Value * pjson_value_deep_copy   (const PJSON_Value *value);
void         pjson_value_free        (PJSON_Value *value);

PJSON_Value_Type pjson_value_get_type   (const PJSON_Value *value);
PJSON_Object *   pjson_value_get_object (const PJSON_Value *value);
PJSON_Array  *   pjson_value_get_array  (const PJSON_Value *value);
const char  *   pjson_value_get_string (const PJSON_Value *value);
size_t          pjson_value_get_string_len(const PJSON_Value *value); /* doesn't account for last null character */
double          pjson_value_get_number (const PJSON_Value *value);
int             pjson_value_get_boolean(const PJSON_Value *value);
PJSON_Value  *   pjson_value_get_parent (const PJSON_Value *value);

/* Same as above, but shorter */
PJSON_Value_Type pjson_type   (const PJSON_Value *value);
PJSON_Object *   pjson_object (const PJSON_Value *value);
PJSON_Array  *   pjson_array  (const PJSON_Value *value);
const char  *   pjson_string (const PJSON_Value *value);
size_t          pjson_string_len(const PJSON_Value *value); /* doesn't account for last null character */
double          pjson_number (const PJSON_Value *value);
int             pjson_boolean(const PJSON_Value *value);

#ifdef __cplusplus
}
#endif

#endif
