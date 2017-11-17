#if defined __MEMORY_INCLUDED__
	#endinput
#endif
	#define __MEMORY_INCLUDED__
	#define MEM_NULLPTR						(Pointer:0)
	#define MEM_NULLUMPTR					(UnmanagedPointer:0)
	#define MEM_NULLFEPTR					(ForeignPointer:0)
	#define AnyPointer						{UnmanagedPointer, ForeignPointer, Pointer}
	#define MEM_struct						enum _:
	#define MEM_SIZEOF_CELL					(4)
	#define MEM_foreach_ex%2(%0,%1:%2)		for (new %1 = MEM_get_size(%2), %0; (%0) < (%1); %0++)
	#define MEM_foreach_rev_ex%2(%0,%1:%2)	for (new %1 = MEM_get_size(%2), %0 = (%1) - 1; (%0) >= 0; %0--)
	#define MEM_foreach%2(%0:%1)			MEM_foreach_ex%2(%0,__size__:%1)
	#define MEM_foreach_rev%2(%0:%1)		MEM_foreach_rev_ex%2(%0,__size__:%1)
	#define MEM_MACR_comp_ptr%2(%0,%1)		(ForeignPointer:(%0) + ForeignPointer:((%1) * MEM_MACR_SIZEOF_CELL))
	#define MEM_MACR_increment_ptr%1(%0)	(ForeignPointer:(%0 += MEM_MACR_SIZEOF_CELL))
	#define MEM_MACR_decrement_ptr%1(%0)	(ForeignPointer:(%0 -= MEM_MACR_SIZEOF_CELL))
	

// Memory result enumerator
enum EMemoryResult
{
	// OK
	EMemoryResult_OK,

	// Invalid size
	EMemoryResult_InvalidSize,

	// Invalid pointer
	EMemoryResult_InvalidPointer,

	// Invalid index
	EMemoryResult_InvalidIndex,

	// Invalid index size
	EMemoryResult_InvalidIndexSize,

	// Out of memory
	EMemoryResult_OutOfMemory
}

// Allocate new memory. Delete with `MEM_delete`
native Pointer:MEM_new(cells = 1);

// Allocate new zeroed memory. Delete with `MEM_delete`
native Pointer:MEM_new_zero(cells = 1);

// Allocate new value. Delete with `MEM_delete`
native Pointer:MEM_new_val(value);

// Allocate new array. Delete with `MEM_delete`
native Pointer:MEM_new_arr(const arr[], arr_size = sizeof arr);

// Clone memory. Delete with `MEM_delete`
native Pointer:MEM_clone(Pointer:pointer);

// Delete allocated memory
native MEM_delete(Pointer:pointer);

// Is valid pointer
native bool:MEM_is_valid_ptr(Pointer:pointer);

// Get size
native MEM_get_size(Pointer:pointer);

// Get value
native MEM_get_val(Pointer:pointer, index = 0);

// Get array
native ForeignPointer:MEM_get_arr(Pointer:pointer, index = 0, arr[], arr_size = sizeof arr);

// Set value
native MEM_set_val(Pointer:pointer, index = 0, value);

// Set array
native ForeignPointer:MEM_set_arr(Pointer:pointer, index = 0, const arr[], arr_size = sizeof arr);

// Copy memory
native Pointer:MEM_copy(Pointer:dest, Pointer:src, size, dest_index = 0, src_index = 0);

// Zero memory
native Pointer:MEM_zero(Pointer:pointer, size, index = 0);

// Get last result
native EMemoryResult:MEM_get_last_result();

// Allocate new unmanaged memory. Delete with `MEM_UM_delete`
native UnmanagedPointer:MEM_UM_new(cells = 1);

// Allocate new zeroed unmanaged memory. Delete with `MEM_UM_delete`
native UnmanagedPointer:MEM_UM_new_zero(cells = 1);

// Allocate new unmanaged value. Delete with `MEM_UM_delete`
native UnmanagedPointer:MEM_UM_new_val(value);

// Allocate new unmanaged array. Delete with `MEM_UM_delete`
native UnmanagedPointer:MEM_UM_new_arr(const arr[], arr_size = sizeof arr);

// Clone unmanaged memory. Delete with `MEM_UM_delete`
native UnmanagedPointer:MEM_UM_clone(AnyPointer:pointer, index = 0, cells);

// Delete unmanaged memory
native MEM_UM_delete(UnmanagedPointer:pointer);

// Get unmanaged value
native MEM_UM_get_val(AnyPointer:pointer, index = 0);

// Get unmanaged array
native ForeignPointer:MEM_UM_get_arr(AnyPointer:pointer, index = 0, arr[], arr_size = sizeof arr);

// Set unmanaged value
native MEM_UM_set_val(UnmanagedPointer:pointer, index = 0, value);

// Set unmanaged array
native ForeignPointer:MEM_UM_set_arr(UnmanagedPointer:pointer, index = 0, const arr[], arr_size = sizeof arr);

// Copy unmanaged memory
native UnmanagedPointer:MEM_UM_copy(UnmanagedPointer:dest, AnyPointer:src, size, dest_index = 0, src_index = 0);

// Zero unmanaged memory
native UnmanagedPointer:MEM_UM_zero(UnmanagedPointer:pointer, size, index = 0);

// Get pointer of variable
native ForeignPointer:MEM_UM_get_addr(&var);

// Get AMX pointer
native ForeignPointer:MEM_UM_get_amx_ptr();
