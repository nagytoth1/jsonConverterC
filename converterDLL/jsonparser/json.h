#ifndef JSON_H
#define JSON_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "strstream.h"
#include "dynamicarray.h"
#include "hashmap.h"

#define TAB "    "
#define ENDL "\n"

typedef char bool;
#define false 0
#define true !0
#define JSON_TRUE_S "true"
#define JSON_FALSE_S "false"

#define JSON_NULL_S "null"

typedef char jsontype;
#define JSON_NULL (jsontype)0
#define JSON_STR (jsontype)1
#define JSON_FLOAT (jsontype)2
#define JSON_INT (jsontype)3
#define JSON_BOOL (jsontype)4
#define JSON_LIST (jsontype)5
#define JSON_OBJ (jsontype)6
typedef union jsonval{
	void *n;
	char *s;
	float f;
	int i;
	bool b;
	dynamicarray l;
	hashmap o;
} jsonval;

typedef struct json{
    jsontype type; //1 byte
	jsonval value; //+20 bytes for union
} json;
/*
    constructors
*/
json json_typeSize(jsontype type, unsigned int n);
json json_type(jsontype type);
json json_null();
json json_string(char *value);
json json_float(float value);
json json_int(int value);
json json_bool(bool value);
json json_list(dynamicarray value);
json json_arr(json *first, unsigned int n);
json json_obj(hashmap value);

json *json_alloc_typeSize(jsontype type, unsigned int n);
json *json_alloc_type(jsontype type);

/*
    list modifiers
*/
void json_list_add(json *list, json *value);
void json_list_add_null(json *list);
void json_list_add_string(json *list, char *value);
void json_list_add_float(json *list, float value);
void json_list_add_int(json *list, int value);
void json_list_add_bool(json *list, bool value);

/*
    object modifiers
*/
void json_obj_put(json *obj, char *key, json *value);
void json_obj_put_null(json *obj, char *key);
void json_obj_put_string(json *obj, char *key, char *value);
void json_obj_put_float(json *obj, char *key, float value);
void json_obj_put_int(json *obj, char *key, int value);
void json_obj_put_bool(json *obj, char *key, bool value);

/*
    output accessors
*/
strstream json_stringify(json j, int initTabPos, bool isDictVal);
char *json_dump(json j);

/*
    destructors
*/
void json_free(json *j);
void json_freeDeep(json *j);

#endif