#pragma once
#include <cstdint>
#include "Vectors.h"
#include "Matrix.h"
#include <array>
#include <vector>
class UtlMemoryPool
{
public:
	std::int32_t block_size()
	{
		return this->m_blocks_per_blob;
	}

	std::int32_t count()
	{
		return this->m_block_allocated_size;
	}

	std::int32_t peak_count()
	{
		return this->m_peak_alloc;
	}

private:
	std::int32_t m_block_size = 0;
	std::int32_t m_blocks_per_blob = 0;
	std::int32_t m_grow_mode = 0;
	std::int32_t m_blocks_allocated = 0;
	std::int32_t m_block_allocated_size = 0;
	std::int32_t m_peak_alloc = 0;
};

template<class T, class key_type = std::uint64_t>
class utl_ts_hash
{
public:
	static std::uint64_t invalid_handle()
	{
		return static_cast<std::uint64_t>(0);
	}

	std::int32_t block_size()
	{
		return this->m_entry_memory.block_size();
	}

	std::int32_t count()
	{
		return this->m_entry_memory.count();
	}

	std::vector<T> get_elements()
	{
		std::vector<T> list;

		std::int32_t n_count = this->count();
		std::int32_t n_index = 0;
		hash_unallocated_data_t* unallocated_data = this->m_buckets.m_unallocated_data;
		for (hash_unallocated_data_t* element = unallocated_data; element; element = element->m_next)
		{
			for (std::int32_t i = 0; i < this->block_size() && i != n_count; i++)
			{
				list.emplace_back(element->m_current_block_list.at(i).m_data);
				n_index++;

				if (n_index >= n_count)
				{
					break;
				}
			}
		}
		return list;
	}

public:
	template<typename dataT>
	struct hash_fixed_data_internal_t
	{
		key_type m_ui_key;
		hash_fixed_data_internal_t<dataT>* m_next;
		dataT m_data;
	};

	using hash_fixed_data_t = hash_fixed_data_internal_t< T >;

	template< typename dataT >
	struct hash_fixed_struct_data_internal_t
	{
		dataT m_data;
		key_type m_ui_key;
	};

	using hash_fixed_struct_data = hash_fixed_struct_data_internal_t<T>;

	struct hash_struct_data_t
	{
		char pad_1[0x10];
		std::array<hash_fixed_struct_data, 256> m_list;
	};

	struct hash_allocated_data_t
	{
	public:
		std::array<hash_fixed_data_t, 128> get_list()
		{
			return this->m_list;
		}

	private:
		char pad_1[0x18];
		std::array<hash_fixed_data_t, 128> m_list;
	};

	template<typename dataT>
	struct hash_bucket_data_internal_t
	{
		dataT m_data;
		hash_fixed_data_internal_t<dataT>* m_next;
		key_type m_ui_key;
	};

	using hash_bucket_data_t = hash_bucket_data_internal_t<T>;

	struct hash_unallocated_data_t
	{
		hash_unallocated_data_t* m_next = nullptr;
		key_type m_6114;
		key_type m_ui_key;
		key_type m_i_unk_1;
		std::array<hash_bucket_data_t, 256> m_current_block_list;
	};

	struct hash_bucket_t
	{
		hash_struct_data_t* m_struct_data = nullptr;
		void* m_mutex_list = nullptr;
		hash_allocated_data_t* m_allocated_data = nullptr;
		hash_unallocated_data_t* m_unallocated_data = nullptr;
	};

	UtlMemoryPool m_entry_memory;
	hash_bucket_t m_buckets;
	bool m_needs_commit = false;
};

struct resource_binding {
	void* data;
	// uint32_t last_bind_frame;
	// uint32_t flags;
	// interlocked_int ref_count;
};

template <class T>
class strong_handle {
public:
	operator T* () noexcept {
		if (!binding)
			return nullptr;
		return (T*)binding->data;
	}
	T* operator->() noexcept {
		if (!binding)
			return nullptr;
		return (T*)binding->data;
	}
private:
	const resource_binding* binding;
};

class alignas(16) Transform {
public:
	alignas(16) Vec3 pos;
	alignas(16) Vec4 rot;

	void ToMatrix(Mat3x4& matrix) const noexcept;
};

template <class T>
class utl_memory {
public:
	T* memory;
	int allocation_count;
	int grow_size;

	T& operator[](int i) noexcept { return memory[i]; }
	const T& operator[](int i) const noexcept { return memory[i]; }
};

class util_string {
public:
	const char* get() const noexcept {
		return (const char*)memory.memory;
	}

	operator const char* () const noexcept { return get(); }

	utl_memory<uint8_t> memory;
	int actual_length;
};

static_assert(sizeof(util_string) == 0x18);

template <class T>
class util_vector {
public:
	int size;
	utl_memory<T> data;

	T& operator[](int i) noexcept { return data.memory[i]; }
	const T& operator[](int i) const noexcept { return data.memory[i]; }
};

static_assert(sizeof(util_vector<void*>) == 0x18);

#define INVALID_LLIST_IDX ((I)~0)

template <class T, class I = int>
class utl_linked_list {
public:
	inline I first() const noexcept { return head; }
	inline I previous(I i) const noexcept { return memory[i].previous; }
	inline I next(I i) const noexcept { return memory[i].next; }
	inline T& element(I i) noexcept { return memory[i].element; }
	inline const T& element(I i) const noexcept { return memory[i].element; }
	inline static I invalid_index() noexcept { return INVALID_LLIST_IDX; }

	struct alignas(8) list_elem {
		T element;
		I previous;
		I next;
	};

	utl_memory<list_elem> memory;
	I head;
	I tail;
	I first_free;
	I element_count;
	I total_elements;
	list_elem* elements;
};

struct game_tick {
	int32_t m_Value;
};

