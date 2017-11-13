#include <a_samp>
#include <memory>

// Required by assert() and assertf()
#pragma tabsize 0

#define assert%2(%0,%1)%3; \
if (!(%0)) \
{ \
    print(%1); \
    return -1; \
}

#define assertf%3(%0,%1,%2)%4; \
if (!(%0)) \
{ \
    printf(%1,%2); \
    return -1; \
}

// On filter script init
public OnFilterScriptInit()
{
    new Pointer:pointers[5], EMemoryResult:result, arr[10] = { 100, ... }, arr_copy[sizeof arr], another_arr[sizeof arr] = { 200, ... }, sz, val, UnmanagedPointer:unmanaged_pointers[5], ForeignPointer:foreign_pointer;
    print("[MEMORYTEST] Test 1");
    pointers[0] = MEM_new(sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"pointers[0] = MEM_new(sizeof arr);\": %d", _:result);
    pointers[1] = MEM_new_zero(20);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"pointers[1] = MEM_new_zero(20);\": %d", _:result);
    pointers[2] = MEM_new_val(30);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"pointers[2] = MEM_new_val(30);\": %d", _:result);
    pointers[3] = MEM_new_arr(arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"pointers[3] = MEM_new_arr(arr);\": %d", _:result);
    pointers[4] = MEM_clone(pointers[3]);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"pointers[4] = MEM_clone(pointers[3]);\": %d", _:result);
    print("[MEMORYTEST] Test 2");
    for (new i; i < sizeof pointers; i++)
    {
        assertf(MEM_is_valid_ptr(pointers[i]), "[MEMORYTEST] pointers[%d] (0x%x) is not valid. Result: %d", _:(pointers[i]), i, _:MEM_get_last_result());
    }
    assertf(pointers[3] != pointers[4], "[MEMORYTEST] pointers[4] (0x%x) is not a copy of pointers[3] (0x%x), instead a reference. Result: %d", _:(pointers[4]), _:(pointers[3]), _:MEM_get_last_result());
    print("[MEMORYTEST] Test 3");
    sz = MEM_get_size(pointers[0]);
    assertf(sz == sizeof arr, "[MEMORYTEST] pointers[0] (0x%x) size is invalid. Expected %d, returned %d. Result: %d", _:(pointers[0]), sizeof arr, sz, _:MEM_get_last_result());
    sz = MEM_get_size(pointers[1]);
    assertf(sz == 20, "[MEMORYTEST] pointers[1] (0x%x) size is invalid. Expected 20, returned %d. Result: %d", _:(pointers[1]), sz, _:MEM_get_last_result());
    sz = MEM_get_size(pointers[2]);
    assertf(sz == 1, "[MEMORYTEST] pointers[2] (0x%x) size is invalid. Expected 1, returned %d. Result: %d", _:(pointers[2]), sz, _:MEM_get_last_result());
    sz = MEM_get_size(pointers[3]);
    assertf(sz == sizeof arr, "[MEMORYTEST] pointers[3] (0x%x) size is invalid. Expected %d, returned %d. Result: %d", _:(pointers[3]), sizeof arr, sz, _:MEM_get_last_result());
    sz = MEM_get_size(pointers[4]);
    assertf(sz == sizeof arr, "[MEMORYTEST] pointers[4] (0x%x) size is invalid. Expected %d, returned %d. Result: %d", _:(pointers[4]), sizeof arr, sz, _:MEM_get_last_result());
    print("[MEMORYTEST] Test 4");
    val = MEM_get_val(pointers[1]);
    assertf(val == 0, "[MEMORYTEST] pointers[1] (0x%x) value is %d, not 0. Result: %d", _:(pointers[1]), val, _:MEM_get_last_result());
    MEM_set_val(pointers[1], _, 10);
    val = MEM_get_val(pointers[1]);
    assertf(val == 10, "[MEMORYTEST] pointers[1] (0x%x) value is %d, not 10. Result: %d", _:(pointers[1]), val, _:MEM_get_last_result());
    val = MEM_get_val(pointers[2]);
    assertf(val == 30, "[MEMORYTEST] pointers[2] (0x%x) value is %d, not 30. Result: %d", _:(pointers[2]), val, _:MEM_get_last_result());
    MEM_set_val(pointers[2], _, 40);
    val = MEM_get_val(pointers[2]);
    assertf(val == 40, "[MEMORYTEST] pointers[2] (0x%x) value is %d, not 40. Result: %d", _:(pointers[2]), val, _:MEM_get_last_result());
    MEM_get_arr(pointers[3], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_get_arr(pointers[3], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(arr[i] == arr_copy[i], "[MEMORYTEST] arr[%d] (%d) and arr_copy[%d] (%d) from original are not equal. Result: %d", i, arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_set_arr(pointers[3], _, another_arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_set_arr(pointers[3], _, another_arr);\": %d", _:result);
    MEM_get_arr(pointers[3], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_get_arr(pointers[3], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(another_arr[i] == arr_copy[i], "[MEMORYTEST] another_arr[%d] (%d) and arr_copy[%d] (%d) from original are not equal. Result: %d", i, another_arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_get_arr(pointers[4], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_get_arr(pointers[4], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(arr[i] == arr_copy[i], "[MEMORYTEST] arr[%d] (%d) and arr_copy[%d] (%d) from copy are not equal. Result: %d", i, arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_set_arr(pointers[4], _, another_arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_set_arr(pointers[4], _, another_arr);\": %d", _:result);
    MEM_get_arr(pointers[4], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_get_arr(pointers[4], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(another_arr[i] == arr_copy[i], "[MEMORYTEST] another_arr[%d] (%d) and arr_copy[%d] (%d) from original are not equal. Result: %d", i, another_arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_copy(pointers[0], pointers[3], sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_copy(pointers[0], pointers[3], sizeof arr);\": %d", _:result);
    MEM_get_arr(pointers[0], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_get_arr(pointers[0], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(another_arr[i] == arr_copy[i], "[MEMORYTEST] another_arr[%d] (%d) and arr_copy[%d] (%d) are not equal. Result: %d", i, another_arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_set_arr(pointers[3], _, arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_set_arr(pointers[3], _, arr);\": %d", _:result);
    MEM_copy(pointers[0], pointers[3], sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_copy(pointers[0], pointers[3], sizeof arr);\": %d", _:result);
    MEM_get_arr(pointers[0], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_get_arr(pointers[0], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(arr[i] == arr_copy[i], "[MEMORYTEST] arr[%d] (%d) and arr_copy[%d] (%d) are not equal. Result: %d", i, arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_zero(pointers[0], sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_zero(pointers[0], sizeof arr);\": %d", _:result);
    MEM_get_arr(pointers[0], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_get_arr(pointers[0], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(arr_copy[i] == 0, "[MEMORYTEST] arr_copy[%d] (%d) is not 0. Result: %d", i, arr_copy[i], _:MEM_get_last_result());
    }
    print("[MEMORYTEST] Test 5");
    for (new i; i < sizeof pointers; i++)
    {
        MEM_delete(pointers[i]);
        result = MEM_get_last_result();
        assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_delete(pointers[i]);\": %d", _:result);
        assertf(!MEM_is_valid_ptr(pointers[i]), "[MEMORYTEST] pointers[%d] (0x%x) is still valid. Result: %d", _:(pointers[i]), i, _:MEM_get_last_result());
        result = MEM_get_last_result();
        assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_is_valid_ptr(pointers[i])\": %d", _:result);
        pointers[i] = MEM_NULLPTR;
        assertf(!MEM_is_valid_ptr(pointers[i]), "[MEMORYTEST] pointers[%d] (0x%x) is still valid. Result: %d", _:(pointers[i]), i, _:MEM_get_last_result());
    }
    print("[MEMORYTEST] Test 6");
    unmanaged_pointers[0] = MEM_UM_new(sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"unmanaged_pointers[0] = MEM_UM_new(sizeof arr);\": %d", _:result);
    unmanaged_pointers[1] = MEM_UM_new_zero(20);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"unmanaged_pointers[1] = MEM_UM_new_zero(20);\": %d", _:result);
    unmanaged_pointers[2] = MEM_UM_new_val(30);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"unmanaged_pointers[2] = MEM_UM_new_val(30);\": %d", _:result);
    unmanaged_pointers[3] = MEM_UM_new_arr(arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"unmanaged_pointers[3] = MEM_UM_new_arr(arr);\": %d", _:result);
    unmanaged_pointers[4] = MEM_UM_clone(unmanaged_pointers[3], _, sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"unmanaged_pointers[4] = MEM_UM_clone(unmanaged_pointers[3]);\": %d", _:result);
    print("[MEMORYTEST] Test 7");
    for (new i; i < sizeof unmanaged_pointers; i++)
    {
        assertf(unmanaged_pointers[i] != MEM_NULLUMPTR, "[MEMORYTEST] unmanaged_pointers[%d] (0x%x) is not valid. Result: %d", _:(unmanaged_pointers[i]), i, _:MEM_get_last_result());
    }
    assertf(unmanaged_pointers[3] != unmanaged_pointers[4], "[MEMORYTEST] unmanaged_pointers[4] (0x%x) is not a copy of unmanaged_pointers[3] (0x%x), instead a reference. Result: %d", _:(unmanaged_pointers[4]), _:(unmanaged_pointers[3]), _:MEM_get_last_result());
    print("[MEMORYTEST] Test 8");
    val = MEM_UM_get_val(unmanaged_pointers[1]);
    assertf(val == 0, "[MEMORYTEST] unmanaged_pointers[1] (0x%x) value is %d, not 0. Result: %d", _:(unmanaged_pointers[1]), val, _:MEM_get_last_result());
    MEM_UM_set_val(unmanaged_pointers[1], _, 10);
    val = MEM_UM_get_val(unmanaged_pointers[1]);
    assertf(val == 10, "[MEMORYTEST] unmanaged_pointers[1] (0x%x) value is %d, not 10. Result: %d", _:(unmanaged_pointers[1]), val, _:MEM_get_last_result());
    val = MEM_UM_get_val(unmanaged_pointers[2]);
    assertf(val == 30, "[MEMORYTEST] unmanaged_pointers[2] (0x%x) value is %d, not 30. Result: %d", _:(unmanaged_pointers[2]), val, _:MEM_get_last_result());
    MEM_UM_set_val(unmanaged_pointers[2], _, 40);
    val = MEM_UM_get_val(unmanaged_pointers[2]);
    assertf(val == 40, "[MEMORYTEST] unmanaged_pointers[2] (0x%x) value is %d, not 40. Result: %d", _:(unmanaged_pointers[2]), val, _:MEM_get_last_result());
    MEM_UM_get_arr(unmanaged_pointers[3], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_get_arr(unmanaged_pointers[3], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(arr[i] == arr_copy[i], "[MEMORYTEST] arr[%d] (%d) and arr_copy[%d] (%d) from original are not equal. Result: %d", i, arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_UM_set_arr(unmanaged_pointers[3], _, another_arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_set_arr(unmanaged_pointers[3], _, another_arr);\": %d", _:result);
    MEM_UM_get_arr(unmanaged_pointers[3], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_get_arr(unmanaged_pointers[3], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(another_arr[i] == arr_copy[i], "[MEMORYTEST] another_arr[%d] (%d) and arr_copy[%d] (%d) from original are not equal. Result: %d", i, another_arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_UM_get_arr(unmanaged_pointers[4], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_get_arr(unmanaged_pointers[4], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(arr[i] == arr_copy[i], "[MEMORYTEST] arr[%d] (%d) and arr_copy[%d] (%d) from copy are not equal. Result: %d", i, arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_UM_set_arr(unmanaged_pointers[4], _, another_arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_set_arr(unmanaged_pointers[4], _, another_arr);\": %d", _:result);
    MEM_UM_get_arr(unmanaged_pointers[4], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_get_arr(unmanaged_pointers[4], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(another_arr[i] == arr_copy[i], "[MEMORYTEST] another_arr[%d] (%d) and arr_copy[%d] (%d) from original are not equal. Result: %d", i, another_arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_UM_copy(unmanaged_pointers[0], unmanaged_pointers[3], sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_copy(unmanaged_pointers[0], unmanaged_pointers[3], sizeof arr);\": %d", _:result);
    MEM_UM_get_arr(unmanaged_pointers[0], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_get_arr(unmanaged_pointers[0], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(another_arr[i] == arr_copy[i], "[MEMORYTEST] another_arr[%d] (%d) and arr_copy[%d] (%d) are not equal. Result: %d", i, another_arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_UM_set_arr(unmanaged_pointers[3], _, arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_set_arr(unmanaged_pointers[3], _, arr);\": %d", _:result);
    MEM_UM_copy(unmanaged_pointers[0], unmanaged_pointers[3], sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_copy(unmanaged_pointers[0], unmanaged_pointers[3], sizeof arr);\": %d", _:result);
    MEM_UM_get_arr(unmanaged_pointers[0], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_get_arr(unmanaged_pointers[0], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(arr[i] == arr_copy[i], "[MEMORYTEST] arr[%d] (%d) and arr_copy[%d] (%d) are not equal. Result: %d", i, arr[i], i, arr_copy[i], _:MEM_get_last_result());
    }
    MEM_UM_zero(unmanaged_pointers[0], sizeof arr);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_zero(unmanaged_pointers[0], sizeof arr);\": %d", _:result);
    MEM_UM_get_arr(unmanaged_pointers[0], _, arr_copy);
    result = MEM_get_last_result();
    assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_get_arr(unmanaged_pointers[0], _, arr_copy);\": %d", _:result);
    for (new i; i < sizeof arr; i++)
    {
        assertf(arr_copy[i] == 0, "[MEMORYTEST] arr_copy[%d] (%d) is not 0. Result: %d", i, arr_copy[i], _:MEM_get_last_result());
    }
    for (new i; i < sizeof unmanaged_pointers; i++)
    {
        MEM_UM_delete(unmanaged_pointers[i]);
        result = MEM_get_last_result();
        assertf(result == EMemoryResult_OK, "[MEMORYTEST] Result for \"MEM_UM_delete(unmanaged_pointers[i]);\": %d", _:result);
        unmanaged_pointers[i] = MEM_NULLUMPTR;
    }
    print("[MEMORYTEST] Test 9");
    foreign_pointer = MEM_UM_get_addr(_:result);
    assert(foreign_pointer != MEM_NULLFEPTR, "[MEMORYTEST] foreign_pointer is null.");
    printf("[MEMORYTEST] Result pointer: 0x%x", _:foreign_pointer);
    foreign_pointer = MEM_UM_get_amx_ptr();
    assert(foreign_pointer != MEM_NULLFEPTR, "[MEMORYTEST] foreign_pointer is null.");
    printf("[MEMORYTEST] AMX pointer: 0x%x", _:foreign_pointer);
    print("[MEMORYTEST] Test completed.");
    print("\r\n==========================================");
    print("= Memory access plugin test filterscript =");
    print("=                         Made by BigETI =");
    print("= Loaded!                                =");
    print("==========================================\r\n");
    return 1;
}

// On filter script exit
public OnFilterScriptExit()
{
    print("\r\n==========================================");
    print("= Memory access plugin test filterscript =");
    print("=                         Made by BigETI =");
    print("= Unloaded!                              =");
    print("==========================================\r\n");
    return 1;
}