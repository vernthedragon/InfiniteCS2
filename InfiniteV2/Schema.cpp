#include "Schema.h"
#include "Interfaces.h"

#include <vector>
#include <unordered_map>
static std::unordered_map<std::string, std::unordered_map<std::string, SchemaSystem::SchemaOffset>> schema_data;

bool SchemaSystem::Initialize() {
	std::vector<std::string> modules = { "client.dll", "engine2.dll", "schemasystem.dll" };

	for (std::string& module : modules)
	{
		schema_system_type_scope* schema_class = g_SchemaSystem->FindTypeScopeForModule(module.c_str());

		if (!schema_class)
		{
			continue;
		}

		utl_ts_hash<schema_class_binding_t*> schema_classes = schema_class->GetClasses();

		for (schema_class_binding_t* binding : schema_classes.get_elements())
		{
			schema_class_info_data_t* client_class_info = schema_class->FindTypeDeclaredClass(binding->binary_name);

			if (!client_class_info)
			{
				continue;
			}

			for (std::int16_t j = 0; j < client_class_info->field_size; j++)
			{
				schema_class_field_data_t* field = &client_class_info->fields[j];

				if (!binding->binary_name || !field || !field->name)
				{
					continue;
				}

				schema_data[binding->binary_name][field->name].Class = binding->binary_name;
				schema_data[binding->binary_name][field->name].Prop = field->name;
				schema_data[binding->binary_name][field->name].Offset = field->offset;
			}
		}
	}

	if (schema_data.size() == 0)
	{
		Client->Log("Schema Data is 0\n");

		return false;
	}


	return true;
}

std::uint16_t SchemaSystem::GetSchema(std::string class_name, std::string property_name)
{
	return schema_data[class_name][property_name].Offset;
}