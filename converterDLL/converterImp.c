#include <stdlib.h>
#include "json.h"

extern __declspec(dllexport) char* __stdcall create_json(int device_ids[], int length){
		json device_list_json = json_type(JSON_LIST);
		json* device_objects_json = (json*) malloc(sizeof(json)*length);

		for (int i = 0; i < length; i++)
			device_objects_json[i] = json_type(JSON_OBJ); //set them as JSON-objects
        for (int i = 0; i < length; i++)
		{
			json_obj_put_int(&device_objects_json[i], "azonos", device_ids[i]); //add as JSON object {"azonos":16388}
			json_list_add(&device_list_json, &device_objects_json[i]); //add to JSON-list [{...},{...},{"azonos":16388}]
		}
		char* output = json_dump(device_list_json);
		for (int i = 0; i < length; i++)
		{
			json_freeDeep(&device_objects_json[i]);
		}
        json_freeDeep(device_objects_json);
		return output;
}
