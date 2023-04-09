#include "json.h"

/*
    constructors
*/
json json_typeSize(jsontype type, unsigned int n) //initializing types with default values and allocating when necessary (list and object)
{
    json result;
    result.type = type;

    switch (type)
    {
    case JSON_NULL:
        result.value.n = NULL;
        break;
    case JSON_STR:
        result.value.s = "";
        break;
    case JSON_FLOAT:
        result.value.f = 0.0f;
        break;
    case JSON_INT:
        result.value.i = 0;
        break;
    case JSON_BOOL:
        result.value.b = false;
        break;
    case JSON_LIST:
        result.value.l = dynarr_allocate(n);
        break;
    case JSON_OBJ:
        result.value.o = hmap_allocateStrAsKeyWithNum(n);
        break;
    }

    return result;
}

json json_type(jsontype type){ return json_typeSize(type, 0); } //if it took 0 it would just allocate to default value

json json_null() { return json_type(JSON_NULL); }

json json_string(char *value)
{
    json result = json_type(JSON_STR);

    // copy string
    unsigned int n = strlen(value);
    result.value.s = malloc((n + 1) * sizeof(char));
    strcpy(result.value.s, value);

    return result;
}

json json_float(float value)
{
    json result = json_type(JSON_FLOAT);
    result.value.f = value;
    return result;
}

json json_int(int value)
{
    json result = json_type(JSON_INT);
    result.value.i = value;
    return result;
}

json json_bool(bool value)
{
    json result = json_type(JSON_INT);
    result.value.b = value;
    return result;
}

json json_list(dynamicarray value)
{
    json result;
    result.type = JSON_LIST;

    result.value.l = value;
    return result;
}

json json_arr(json *first, unsigned int n)
{
    json result = json_typeSize(JSON_LIST, n);

    // insert values
    for (unsigned int i = 0; i < n; i++)
    {
        dynarr_addLast(&result.value.l, (void *)(first + i));
    }

    return result;
}

json json_obj(hashmap value)
{
    json result;
    result.type = JSON_OBJ;

    result.value.o = value;
    return result;
}

/*
    list modifiers
*/
void json_list_add(json *list, json *value)
{
    dynarr_addLast(&list->value.l, (void *)value);
}

void json_list_add_null(json *list)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_null();

    json_list_add(list, jValue);
}

void json_list_add_string(json *list, char *value)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_string(value);

    json_list_add(list, jValue);
}

void json_list_add_float(json *list, float value)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_float(value);

    json_list_add(list, jValue);
}

void json_list_add_int(json *list, int value)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_int(value);

    json_list_add(list, jValue);
}

void json_list_add_bool(json *list, bool value)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_bool(value);

    json_list_add(list, jValue);
}

/*
    object modifiers
*/
void json_obj_put(json *obj, char *key, json *value)
{
    hmap_put(&obj->value.o, (void *)key, (void *)value);
}

void json_obj_put_null(json *obj, char *key)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_null();

    json_obj_put(obj, key, jValue);
}

void json_obj_put_string(json *obj, char *key, char *value)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_string(value);

    json_obj_put(obj, key, jValue);
}

void json_obj_put_float(json *obj, char *key, float value)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_float(value);

    json_obj_put(obj, key, jValue);
}

void json_obj_put_int(json *obj, char *key, int value)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_int(value);

    json_obj_put(obj, key, jValue);
}

void json_obj_put_bool(json *obj, char *key, bool value)
{
    json *jValue = malloc(sizeof(json));
    *jValue = json_bool(value);

    json_obj_put(obj, key, jValue);
}

/*
    output accessors
*/
strstream json_stringify(json j, int initTabPos, bool isDictVal)
{
    strstream result = strstream_allocDefault();

    // placeholder for end line

    // write element
    switch (j.type)
    {
    case JSON_NULL:
        strstream_concat(&result, JSON_NULL_S);
        break;
    case JSON_STR:
        strstream_concat(&result, "\"%s\"", j.value.s);
        break;
    case JSON_FLOAT:
        strstream_concat(&result, "%f", j.value.f);
        break;
    case JSON_INT:
        strstream_concat(&result, "%d", j.value.i);
        break;
    case JSON_BOOL:
        strstream_concat(&result, "%s", j.value.b ? JSON_TRUE_S : JSON_FALSE_S);
        break;
    case JSON_LIST:
        if (!j.value.l.size)
        {
            // no elements, insert empty square brackets
            strstream_concat(&result, "[]");
            break;
        }

        strstream_concat(&result, "[");

        void *current = NULL;
        dynarr_iterator iterator = dynarr_iterator_new(&j.value.l);

        while ((current = dynarr_iterator_next(&iterator)))
        {
            // recursively get dumped string of the json element
            strstream el_stream = json_stringify(*((json *)current), initTabPos + 1, false);
            // concatenate to return stream
            strstream_concat(&result, "%s,", el_stream.str);
            strstream_clear(&el_stream);
        }

        // has an extra comma at the end
        strstream_retreat(&result, 1);
        strstream_concat(&result, "]");

        break;

    case JSON_OBJ:
        if (!j.value.o.numEntries)
        {
            // no entries, concatenate opening and closing brackets
            strstream_concat(&result, "{}");
            break;
        }

        strstream_concat(&result, "{");

        mapentry *curEntry = NULL;
        hashmap_iterator obj_it = hmap_iterator_new(&j.value.o);

        while ((curEntry = hmap_iterator_next(&obj_it)))
        {

            // key
            strstream_concat(&result, "\"%s\":", (char *)curEntry->key);

            // get dumped string from the value recursively
            strstream el_stream = json_stringify(*((json *)curEntry->val), initTabPos + 1, true);
            // concatenate to return stream
            strstream_concat(&result, "%s,", el_stream.str);
            strstream_clear(&el_stream);
        }

        // has an extra comma at the end
        strstream_retreat(&result, 1);
        strstream_concat(&result, "}");

        break;
    };

    return result;
}

char *json_dump(json j)
{
    // get output as a string
    strstream s = json_stringify(j, 0, false);

    // allocate the return string
    char *result = malloc((s.size + 1) * sizeof(char));
    //strcpy instead of memcpy
    strcpy(result, s.str);
    strstream_clear(&s);
    return result;
}

/*
    destructors
*/
void json_free(json *j)
{
    if (j->type == JSON_STR)
    {
        free(&j->value.s);
    }
    else if (j->type == JSON_LIST)
    { //iterate through values of list
        void *current = NULL;
        dynarr_iterator iterator = dynarr_iterator_new(&j->value.l);

        while ((current = dynarr_iterator_next(&iterator)))
        {
            json_free((json *)current);
        }

        dynarr_free(&j->value.l);
    }
    else if (j->type == JSON_OBJ)
    {
        mapentry *current = NULL;
        hashmap_iterator iterator = hmap_iterator_new(&j->value.o);

        while ((current = hmap_iterator_next(&iterator)))
        {
            json_free((json *)current->val);
        }

        hmap_free(&j->value.o);
    }
}

void json_freeDeep(json *j)
{
    if (j->type == JSON_STR)
    {
        free(j->value.s);
    }
    else if (j->type == JSON_LIST)
    {
        void *current = NULL;
        dynarr_iterator iterator = dynarr_iterator_new(&j->value.l);

        while ((current = dynarr_iterator_next(&iterator)))
        {
            json_freeDeep((json *)current);
        }

        dynarr_freeDeep(&j->value.l);
    }
    else if (j->type == JSON_OBJ)
    {
        mapentry *current = NULL;
        hashmap_iterator iterator = hmap_iterator_new(&j->value.o);

        while ((current = hmap_iterator_next(&iterator)))
        {
            json_freeDeep((json *)current->val);
        }

        hmap_freeDeep(&j->value.o);
    }
}