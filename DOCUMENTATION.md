# PAWN memory access plugin

## Important
`MEM_delete` or `MEM_UM_delete` should be used to prevent memory leaks. Especially unmanaged memory allocation is unsafe, and should only be used if speed is prefered. Managed pointer access is still fast though. It is not recommended to write using foreign pointers.

Use `MEM_get_last_result` to check why a function has failed.

## Documentation (Functions)

### MEM_new
#### Description
Allocate new memory. Delete with `MEM_delete`
#### Returns
Pointer of the allocated memory if successful, otherwise `MEM_NULLPTR`.
#### Syntax
```C
Pointer:MEM_new(cells = 1)
```
#### Example
```C
new Pointer:pointer = MEM_new();
```

### MEM_new_zero
#### Description
Allocate new zeroed memory. Delete with `MEM_delete`
#### Returns
Pointer of the allocated memory if successful, otherwise `MEM_NULLPTR`.
#### Syntax
```C
Pointer:MEM_new_zero(cells = 1)
```
#### Example
```C
new Pointer:pointer = MEM_new_zero();
```

### MEM_new_val
#### Description
Allocate new value. Delete with `MEM_delete`
#### Returns
Pointer of the allocated memory if successful, otherwise `MEM_NULLPTR`.
#### Syntax
```C
Pointer:MEM_new_val(value)
```
#### Example
```C
new Pointer:pointer = Pointer:MEM_new_val(1);
```

### MEM_new_arr
#### Description
Allocate new array. Delete with `MEM_delete`
#### Returns
Pointer of the allocated memory if successful, otherwise `MEM_NULLPTR`.
#### Syntax
```C
Pointer:MEM_new_arr(const arr[], arr_size = sizeof arr)
```
#### Example
```C
new arr[10] = { 100, ... }, Pointer:pointer = MEM_new_arr(arr);
```

### MEM_clone
#### Description
Clone memory. Delete with `MEM_delete`
#### Returns
Pointer of the allocated memory if successful, otherwise `MEM_NULLPTR`.
#### Syntax
```C
Pointer:MEM_clone(Pointer:pointer)
```
#### Example
```C
new Pointer:pointer = MEM_new_value(1), Pointer:clone = MEM_clone(pointer);
```

### MEM_delete
#### Description
Delete allocated memory
#### Syntax
```C
MEM_delete(Pointer:pointer)
```
#### Example
```C
new Pointer:pointer = MEM_new_val(1);
MEM_delete(pointer);
pointer = MEM_NULLPTR;
```

### MEM_is_valid_ptr
#### Description
Is valid pointer
#### Returns
`true` if valid, otherwise `false`.
#### Syntax
```C
bool:MEM_is_valid_ptr(Pointer:pointer)
```
#### Example
```C
new Pointer:pointer = MEM_new_val(1);
printf("\"pointer\" is %s", (MEM_is_valid_ptr(pointer) ? ("valid") : ("not valid")));
```

### MEM_get_size
#### Description
Get size
#### Returns
Size of the allocated data in cells if valid, otherwise `0`.
#### Syntax
```C
MEM_get_size(Pointer:pointer)
```
#### Example
```C
new Pointer:pointer = MEM_new_val(1);
printf("Size of \"pointer\" is %d", MEM_get_size(pointer));
```

### MEM_get_val
#### Description
Get value
#### Returns
Value in that allocated memory if valid, otherwise `0`.
#### Syntax
```C
MEM_get_val(Pointer:pointer, index = 0)
```
#### Example
```C
new Pointer:pointer = MEM_new_val(1);
printf("Value: %d", MEM_get_val(pointer));
```

### MEM_get_arr
#### Description
Get array
#### Returns
Pointer to the `arr` variable if successful, otherwise `MEM_NULLFEPTR`.
#### Syntax
```C
ForeignPointer:MEM_get_arr(Pointer:pointer, index = 0, arr[], arr_size = sizeof arr)
```
#### Example
```C
new arr[10] = { 100, ... }, Pointer:pointer = MEM_new_arr(arr), another_arr[sizeof arr];
MEM_get_arr(pointer, _, another_arr);
```

### MEM_set_val
#### Description
Set value
#### Returns
Value of `value` if successful, otherwise `0`.
#### Syntax
```C
MEM_set_val(Pointer:pointer, index = 0, value)
```
#### Example
```C
new Pointer:pointer = MEM_new();
MEM_set_val(pointer, _, 1);
```

### MEM_set_arr
#### Description
Set array
#### Returns
Pointer of the `arr` variable if successful, otherwise `MEM_NULLFEPTR`.
#### Syntax
```C
ForeignPointer:MEM_set_arr(Pointer:pointer, index = 0, const arr[], arr_size = sizeof arr)
```
#### Example
```C
new arr[10] = { 100, ... }, Pointer:pointer = MEM_new(sizeof arr);
MEM_set_arr(pointer, _, arr);
```

### MEM_copy
#### Description
Copy memory
#### Returns
Value of `dest` if successful, otherwise `MEM_NULLPTR`.
#### Syntax
```C
Pointer:MEM_copy(Pointer:dest, Pointer:src, size, dest_index = 0, src_index = 0)
```
#### Example
```C
new Pointer:src_pointer = MEM_new_value(1), Pointer:dest_pointer = MEM_new();
MEM_copy(dest_pointer, src_pointer, MEM_get_size(dest_pointer));
```

### MEM_zero
#### Description
Zero memory
#### Returns
Value of `pointer` if successful, otherwise `MEM_NULLPTR`.
#### Syntax
```C
Pointer:MEM_zero(Pointer:pointer, size, index = 0)
```
#### Example
```C
new Pointer:pointer = MEM_new();
MEM_zero(pointer, MEM_get_size(pointer));
```

### MEM_get_last_result
#### Description
Get last result
#### Returns
Last result. See `EMemoryResult` for more information
#### Syntax
```C
EMemoryResult:MEM_get_last_result()
```
#### Example
```C
new Pointer:pointer = MEM_new();
printf("Last result: %d", MEM_get_last_result());
```

### MEM_UM_new
#### Description
Allocate new unmanaged memory. Delete with `MEM_UM_delete`
#### Returns
Pointer of the allocated and unmanaged memory if successful, otherwise `MEM_NULLUMPTR`.
#### Syntax
```C
UnmanagedPointer:MEM_UM_new(cells = 1)
```
#### Example
```C
new UnmanagedPointer:pointer = MEM_UM_new();
if (pointer)
{
	// Successful
}
```

### MEM_UM_new_zero
#### Description
Allocate new zeroed unmanaged memory. Delete with `MEM_UM_delete`
#### Returns
Pointer of the allocated and unmanaged memory if successful, otherwise `MEM_NULLUMPTR`.
#### Syntax
```C
UnmanagedPointer:MEM_UM_new_zero(cells = 1)
```
#### Example
```C
new UnmanagedPointer:pointer = MEM_UM_new_zero();
if (pointer)
{
	// Successful
}
```

### MEM_UM_new_val
#### Description
Allocate new unmanaged value. Delete with `MEM_UM_delete`
#### Returns
Pointer of the allocated and unmanaged memory if successful, otherwise `MEM_NULLUMPTR`.
#### Syntax
```C
UnmanagedPointer:MEM_UM_new_val(value)
```
#### Example
```C
new UnmanagedPointer:pointer = MEM_UM_new_val(1);
if (pointer)
{
	// Successful
}
```

### MEM_UM_new_arr
#### Description
Allocate new unmanaged array. Delete with `MEM_UM_delete`
#### Returns
Pointer of the allocated and unmanaged memory if successful, otherwise `MEM_NULLUMPTR`.
#### Syntax
```C
UnmanagedPointer:MEM_UM_new_arr(const arr[], arr_size = sizeof arr)
```
#### Example
```C
new arr[10] = { 100, ... }, UnmanagedPointer:pointer = MEM_UM_new_arr(arr);
if (pointer)
{
	// Successful
}
```

### MEM_UM_clone
#### Description
Clone unmanaged memory. Delete with `MEM_UM_delete`
#### Returns
Pointer of the allocated and unmanaged memory if successful, otherwise `MEM_NULLUMPTR`.
#### Syntax
```C
UnmanagedPointer:MEM_UM_clone(AnyPointer:pointer, index = 0, cells)
```
#### Example
```C
new UnmanagedPointer:pointer = MEM_UM_new_value(1);
if (pointer)
{
	new UnmanagedPointer:clone = MEM_UM_clone(pointer);
	if (clone)
	{
		// Successful
	}
}
```

### MEM_UM_delete
#### Description
Delete unmanaged memory
#### Syntax
```C
MEM_UM_delete(UnmanagedPointer:pointer)
```
#### Example
```C
new Pointer:pointer = MEM_UM_new();
if (pointer)
{
	MEM_UM_delete(pointer);
}
```

### MEM_UM_get_val
#### Description
Get unmanaged value
#### Returns
Unmanaged value if successful, otherwise `0`.
#### Syntax
```C
MEM_UM_get_val(AnyPointer:pointer, index = 0)
```
#### Example
```C
new UnmanagedPointer:pointer = MEM_UM_new_value(1);
if (pointer)
{
	printf("Value: %d", MEM_UM_get_val(pointer));
}
```

### MEM_UM_get_arr
#### Description
Get unmanaged array
#### Returns
Pointer of the `arr` variable if successful, otherwise `MEM_NULLFEPTR`.
#### Syntax
```C
ForeignPointer:MEM_UM_get_arr(AnyPointer:pointer, index = 0, arr[], arr_size = sizeof arr)
```
#### Example
```C
new arr[10] = { 100, ... }, UnmanagedPointer:pointer = MEM_UM_new_arr(arr), another_arr[sizeof arr];
if (pointer)
{
	MEM_UM_get_arr(pointer, _, another_arr);
}
```

### MEM_UM_set_val
#### Description
Set unmanaged value
#### Returns
Value from `value` if successful, otherwise `0`.
#### Syntax
```C
MEM_UM_set_val(UnmanagedPointer:pointer, index = 0, value)
```
#### Example
```C
new UnmanagedPointer:pointer = MEM_UM_new();
if (pointer)
{
	MEM_UM_set_val(pointer, _, 1);
}
```

### MEM_UM_set_arr
#### Description
Set unmanaged array
#### Returns
Pointer of the `arr` variable if successful, otherwise `MEM_NULLFEPTR`.
#### Syntax
```C
ForeignPointer:MEM_UM_set_arr(UnmanagedPointer:pointer, index = 0, const arr[], arr_size = sizeof arr)
```
#### Example
```C
new arr[10] = { 100, ... }, UnmanagedPointer:pointer = MEM_UM_new(sizeof arr);
if (pointer)
{
	MEM_UM_set_arr(pointer, _, arr);
}
```

### MEM_UM_copy
#### Description
Copy unmanaged memory
#### Returns
Value of `dest` if successful, otherwise `MEM_NULLUMPTR`.
#### Syntax
```C
UnmanagedPointer:MEM_UM_copy(UnmanagedPointer:dest, AnyPointer:src, size, dest_index = 0, src_index = 0)
```
#### Example
```C
new UnmanagedPointer:src_pointer = MEM_UM_new_value(1);
if (src_pointer)
{
	new UnmanagedPointer:dest_pointer = MEM_UM_new();
	if (dest_pointer)
	{
		MEM_UM_copy(dest_pointer, src_pointer, 1);
	}
}
```

### MEM_UM_zero
#### Description
Zero unmanaged memory
#### Returns

#### Syntax
```C
UnmanagedPointer:MEM_UM_zero(UnmanagedPointer:pointer, size, index = 0)
```
#### Example
```C
new UnmanagedPointer:pointer = MEM_UM_new();
if (pointer)
{
	MEM_UM_zero(pointer, 1);
}
```

### MEM_UM_get_addr
#### Description
Get pointer of variable
#### Returns
Pointer 
#### Syntax
```C
ForeignPointer:MEM_UM_get_addr(&var)
```
#### Example
```C
new foo;
printf("Pointer of \"foo\": 0x%x", MEM_UM_get_addr(foo));
```

### MEM_UM_get_amx_ptr
#### Description
Get AMX pointer
#### Returns
AMX pointer
#### Syntax
```C
ForeignPointer:MEM_UM_get_amx_ptr()
```
#### Example
```C
printf("AMX pointer: 0x%x", MEM_UM_get_amx_ptr());
```
## Documentation (Enumerators)
### EMemoryResult
#### Description
Memory result enumerator
#### Items
##### OK
`EMemoryResult_OK`
##### Invalid size
`EMemoryResult_InvalidSize`
##### Invalid pointer
`EMemoryResult_InvalidPointer`
##### Invalid index
`EMemoryResult_InvalidIndex`
##### Invalid index size
`EMemoryResult_InvalidIndexSize`
##### Out of memory
`EMemoryResult_OutOfMemory`

## Documentation (Definitions)

### MEM_NULLPTR
#### Description
Null pointer of type `Pointer`

### MEM_NULLUMPTR
#### Description
Null pointer of type `UnmanagedPointer`

### MEM_NULLFEPTR
#### Description
Null pointer of type `ForeignPointer`

### AnyPointer
#### Description
A set of tags containing `UnmanagedPointer`, `ForeignPointer` and `Pointer`

### MEM_struct
#### Description
Tagless PAWN enumerator, which is used to structure data

### MEM_SIZEOF_CELL
#### Description
Cell size in bytes

### MEM_foreach
#### Description
A method to traverse through allocated memory
#### Syntax
```C
MEM_foreach(index : Pointer:pointer)
```
#### Example
```C
new arr[10] = { 100, ... }, Pointer:pointer = MEM_new_arr(arr);
MEM_foreach(index : pointer)
{
	printf("Value at index %d: %d", index, MEM_get_val(pointer, index));
}
```

### MEM_foreach_rev
#### Description
A method to traverse through allocated memory in reverse
#### Syntax
```C
MEM_foreach_rev(index : Pointer:pointer)
```
#### Example
```C
new arr[10] = { 100, ... }, Pointer:pointer = MEM_new_arr(arr);
MEM_foreach_rev(index : pointer)
{
	printf("Value at index %d: %d", index, MEM_get_val(pointer, index));
}
```

### MEM_foreach_ex
#### Description
A method to traverse through allocated memory
#### Syntax
```C
MEM_foreach_ex(index, size : Pointer:pointer)
```
#### Example
```C
new arr[10] = { 100, ... }, Pointer:pointer = MEM_new_arr(arr);
MEM_foreach(index, size : pointer)
{
	printf("[%d] Value at index %d: %d", size, index, MEM_get_val(pointer, index));
}
```

### MEM_foreach_rev_ex
#### Description
A method to traverse through allocated memory in reverse
#### Syntax
```C
MEM_foreach_rev_ex(index, size : Pointer:pointer)
```
#### Example
```C
new arr[10] = { 100, ... }, Pointer:pointer = MEM_new_arr(arr);
MEM_foreach_rev_ex(index, size : pointer)
{
	printf("[%d] Value at index %d: %d", size, index, MEM_get_val(pointer, index));
}
```

### MEM_MACR_comp_ptr
#### Description
Compute pointer
#### Returns
Computed foreign pointer
#### Syntax
```C
ForeignPointer:MEM_MACR_comp_ptr(AnyPointer:pointer, index)
```
#### Example
```C
new arr[2], Pointer:pointer = MEM_UM_get_addr(arr[0]), ForeignPointer:next_pointer = MEM_MACR_comp_ptr(pointer, 1);
```

### MEM_MACR_increment_ptr
#### Description
Increment pointer
#### Returns
Incremented foreign pointer
#### Syntax
```C
ForeignPointer:MEM_MACR_increment_ptr(AnyPointer:pointer)
```
#### Example
```C
new arr[2], Pointer:pointer = MEM_UM_get_addr(arr[0]), ForeignPointer:next_pointer = MEM_MACR_increment_ptr(pointer);
```

### MEM_MACR_decrement_ptr
#### Description
Decrement pointer
#### Returns
Decremented foreign pointer
#### Syntax
```C
ForeignPointer:MEM_MACR_decrement_ptr(AnyPointer:pointer)
```
#### Example
```C
new arr[2], Pointer:pointer = MEM_UM_get_addr(arr[1]), ForeignPointer:prev_pointer = MEM_MACR_decrement_ptr(pointer);
```
