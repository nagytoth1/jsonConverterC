#include <stdlib.h>
#include "src/json.h"

extern __declspec(dllexport) char* __stdcall create_json(unsigned short* device_ids, unsigned short length){
	json device_list_json = json_type(JSON_LIST);
	//we need an instance for each JSON-object (each device) to give them different deviceIDs
	json* device_objects_json = malloc(sizeof(json) * length);
	for (unsigned short i = 0; i < length; i++)
		device_objects_json[i] = json_type(JSON_OBJ); //set them as JSON-objects

	for (unsigned short i = 0; i < length; i++)
    {
        json_obj_put_int(&device_objects_json[i], "azonos", device_ids[i]); //add as JSON object {"azonos":16388}
        json_list_add(&device_list_json, &device_objects_json[i]); //add to JSON-list [{...},{...},{"azonos":16388}]
    }
	unsigned short n;
	char* output = json_dump(device_list_json, false, (int*)&n);
	for (unsigned short i = 0; i < length; i++)
	{
		json_freeDeep(&device_objects_json[i]);
	}
	free(device_objects_json);
	json_freeDeep(&device_list_json);
	return output;
}

extern __declspec(dllexport) char* __stdcall writing_output(){
	json json_list = json_type(JSON_LIST);
	json json_object_arr[1];

	json_object_arr[0] = json_type(JSON_OBJ); //set them as JSON-objects
	json_obj_put_int(&json_object_arr[0], "azonos", 16388); //add as JSON object {"azonos":16388}

	json_list_add(&json_list, &json_object_arr[0]); //add to JSON-list [{...},{...},{"azonos":16388}]

	int n;
	char* output = json_dump(json_list, false, &n);
	json_freeDeep(&json_object_arr[0]);
	free(json_object_arr);
	json_freeDeep(&json_list);
	return output;
}
