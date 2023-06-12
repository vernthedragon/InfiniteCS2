#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include "Utl.h"



typedef struct _schema_class_info_data_t schema_class_info_data_t;
class schema_system_type_scope;

class schema_enum_binding
{
public:
	virtual const char* get_binding_name() = 0;
	virtual void* as_class_binding() = 0;
	virtual schema_enum_binding* as_enum_binding() = 0;
	virtual const char* get_binary_name() = 0;
	virtual const char* get_project_name() = 0;
public:
	char* m_binding_name;
};

class SchemaType
{
public:
	bool GetSizes(int* out_size1, uint8_t* unk_probably_not_size)
	{
		return reinterpret_cast<int(__thiscall*)(void*, int*, uint8_t*)>(this->m_vtable[3])(this, out_size1, unk_probably_not_size);
	}
public:
	bool GetSize(int* out_size)
	{
		uint8_t smh = 0;
		return GetSizes(out_size, &smh);
	}
public:
	std::uintptr_t* m_vtable;
	const char* m_name;
	schema_system_type_scope* m_type_scope;
	std::uint8_t m_type_category;
	std::uint8_t m_atomic_category;

	SchemaType* GetRefClass()
	{
		if (this->m_type_category != 1)
		{
			return nullptr;
		}

		SchemaType* ptr = this->m_SchemaType;

		while (ptr && ptr->m_type_category == 1)
		{
			ptr = ptr->m_SchemaType;
		}

		return ptr;
	}

	typedef struct _array_t
	{
		std::uint32_t array_size;
		std::uint32_t unknown;
		SchemaType* element_type_;
	} array_t;

	typedef struct _atomic_t
	{
		std::uint64_t gap[2];
		SchemaType* template_typename;
	} atomic_t;

	typedef struct _atomic_tt
	{
		std::uint64_t gap[2];
		SchemaType* templates[2];
	} atomic_tt;

	typedef struct _atomic_i
	{
		std::uint64_t gap[2];
		std::uint64_t integer;
	} atomic_i;

	union
	{
		SchemaType* m_SchemaType;
		schema_class_info_data_t* m_class_info;
		void* m_enum_binding_;
		array_t m_array_;
		atomic_t m_atomic_t_;
		atomic_tt m_atomic_tt_;
		atomic_i m_atomic_i_;
	};
};

typedef struct _schema_class_field_data_t
{
	const char* name;
	SchemaType* type;
	short offset;
	unsigned char pad1[0xE];
} schema_class_field_data_t;

typedef struct _schema_class_info_data_t
{
	unsigned char pad1[0x8];
	const char* name;
	char* module;
	std::int32_t size;
	std::int16_t field_size;
	std::int16_t static_size;
	std::int16_t metadata_size;
	std::int16_t i_unk1;
	std::int16_t i_unk2;
	std::int16_t i_unk3;
	schema_class_field_data_t* fields;
} schema_class_info_data_t;

typedef struct _SchemaType_t
{
	std::uintptr_t* vtable;
	const char* name;
} SchemaType_t;

typedef struct _schema_class_binding_t
{
	_schema_class_binding_t* parent;
	const char* binary_name;
	const char* module_name;
	const char* class_name;
	void* class_info_old_synthesized;
	void* class_info;
	void* this_module_binding_pointer;
	SchemaType_t* SchemaType;
} schema_class_binding_t;

class schema_system_type_scope
{
public:
	schema_class_info_data_t* FindTypeDeclaredClass(const char* name)
	{
		using function_t = schema_class_info_data_t * (__thiscall*)(void*, const char*);
		return (*reinterpret_cast<function_t**>(this))[2](this, name);
	}

	utl_ts_hash<schema_class_binding_t*> GetClasses()
	{
		return this->m_classes;
	}

private:
	char pad1[0x8];
	std::array<char, 256> m_name = { };
	unsigned char pad2[0x450];
	utl_ts_hash<schema_class_binding_t*> m_classes;
	char pad3[0x2804];
};

class ISchemaSystem
{
public:
	schema_system_type_scope* GlobalTypeScope()
	{
		using function_t = schema_system_type_scope * (__thiscall*)(void*);
		return (*reinterpret_cast<function_t**>(this))[11](this);
	}

	schema_system_type_scope* FindTypeScopeForModule(const char* module)
	{
		using function_t = schema_system_type_scope * (__thiscall*)(void*, const char*);
		return (*reinterpret_cast<function_t**>(this))[13](this, module);
	}
};