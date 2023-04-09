#include <stdio.h>
#include <stdlib.h>

#include "json.h"

char* create_json(unsigned short* device_ids, unsigned short length){
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
	char* output = json_dump(device_list_json);
	for (unsigned short i = 0; i < length; i++)
	{
		json_freeDeep(&device_objects_json[i]);
	}
	free(device_objects_json);
	json_freeDeep(&device_list_json);
	return output;
}

int main()
{
	unsigned short tomb[2] = {16388, 43392};
	printf("%s\n", create_json(tomb, 2));
    return 0;
}