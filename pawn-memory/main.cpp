/*
	Memory access plugin made by BigETI © 2013 - 2017
*/

#include "ManagedMemory.h"
#include <string.h>

using namespace std;
using namespace PAWNMemory;

typedef void(*logprintf_t)(char *format, ...);
logprintf_t logprintf;
extern void *pAMXFunctions;

/// <summary>
/// Last result
/// </summary>
static EMemoryResult lastResult(EMemoryResult_OK);

/// <summary>
/// Number of arguments are equal
/// </summary>
/// <param name="params">Params</param>
/// <param name="expectedCount">Expected count of params</param>
/// <param name="functionName">FUnction name</param>
/// <returns>"true if number of params match with expected count of params, otherwise "false"</returns>
inline bool NumArgsEqual(cell *params, cell expectedCount, const char *functionName)
{

	bool ret((params[0] / sizeof(cell)) == expectedCount);
	if (!ret)
	{
		logprintf("[MEMORY] [ERROR] Number of arguments for \"%s\" is invalid. Expected %d, found %d.\r\n", functionName, expectedCount, params[0] / sizeof(cell));
	}
	return ret;
}

/// <summary>
/// Allocate new memory. Delete with "MEM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLPTR"</returns>
/// <remarks>Pointer:MEM_new(cells = 1)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_new(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 1, "MEM_new"))
	{
		ret = ManagedMemory::New(params[1]);
		lastResult = ManagedMemory::GetLastResult();
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Allocate new zeroed memory. Delete with "MEM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLPTR"</returns>
/// <remarks>Pointer:MEM_new_zero(cells = 1)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_new_zero(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 1, "MEM_new_zero"))
	{
		ret = ManagedMemory::NewZero(params[1]);
		lastResult = ManagedMemory::GetLastResult();
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Allocate new value. Delete with "MEM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLPTR"</returns>
/// <remarks>Pointer:MEM_new_val(value)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_new_val(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 1, "MEM_new_val"))
	{
		ret = ManagedMemory::NewValue(params[1]);
		lastResult = ManagedMemory::GetLastResult();
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Allocate new array. Delete with "MEM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLPTR"</returns>
/// <remarks>Pointer:MEM_new_arr(const arr[], arr_size = sizeof arr)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_new_arr(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 2, "MEM_new_arr"))
	{
		cell *arr(nullptr);
		amx_GetAddr(amx, params[1], &arr);
		ret = ManagedMemory::NewArray(arr, params[2]);
		lastResult = ManagedMemory::GetLastResult();
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Clone memory. Delete with "MEM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLPTR"</returns>
/// <remarks>Pointer:MEM_clone(Pointer:pointer)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_clone(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 1, "MEM_clone"))
	{
		ret = ManagedMemory::Clone(reinterpret_cast<cell *>(params[1]));
		lastResult = ManagedMemory::GetLastResult();
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Delete allocated memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Nothing</returns>
/// <remarks>MEM_delete(Pointer:pointer)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_delete(AMX *amx, cell *params)
{
	if (NumArgsEqual(params, 1, "MEM_delete"))
	{
		ManagedMemory::Delete(reinterpret_cast<cell *>(params[1]));
		lastResult = ManagedMemory::GetLastResult();
	}
	return 0;
}

/// <summary>
/// Is valid pointer
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>"true" if pointer is valid, otherwise "false"</returns>
/// <remarks>bool:MEM_is_valid_ptr(Pointer:pointer)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_is_valid_ptr(AMX *amx, cell *params)
{
	bool ret(false);
	if (NumArgsEqual(params, 1, "MEM_is_valid_ptr"))
	{
		ret = ManagedMemory::IsValidPointer(reinterpret_cast<cell *>(params[1]));
		lastResult = ManagedMemory::GetLastResult();
	}
	return ret;
}

/// <summary>
/// Get size
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Size in cells of allocated memory if successful, otherwise "0"</returns>
/// <remarks>MEM_get_size(Pointer:pointer)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_get_size(AMX *amx, cell *params)
{
	cell ret(0);
	if (NumArgsEqual(params, 1, "MEM_size"))
	{
		ret = ManagedMemory::GetSize(reinterpret_cast<cell *>(params[1]));
		lastResult = ManagedMemory::GetLastResult();
	}
	return ret;
}

/// <summary>
/// Get value
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Value from allocated memory if successful, otherwise "0"</returns>
/// <remarks>MEM_get_val(Pointer:pointer, index = 0)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_get_val(AMX *amx, cell *params)
{
	cell ret(0);
	if (NumArgsEqual(params, 2, "MEM_get_val"))
	{
		cell *ptr(reinterpret_cast<cell *>(params[1]));
		cell size(ManagedMemory::GetSize(ptr));
		if (size > 0)
		{
			if ((params[2] >= 0) && (params[2] < size))
			{
				ret = ptr[params[2]];
				lastResult = EMemoryResult_OK;
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = ManagedMemory::GetLastResult();
		}
	}
	return ret;
}

/// <summary>
/// Get array
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of array if successful, otherwise "MEM_NULLFEPTR"</returns>
/// <remarks>ForeignPointer:MEM_get_arr(Pointer:pointer, index = 0, arr[], arr_size = sizeof arr)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_get_arr(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 4, "MEM_get_arr"))
	{
		cell size(ManagedMemory::GetSize(reinterpret_cast<cell *>(params[1])));
		if (size > 0)
		{
			if (params[2] >= 0)
			{
				if (params[4] > 0)
				{
					if ((params[4] + params[2]) <= size)
					{
						amx_GetAddr(amx, params[3], &ret);
						memcpy(ret, reinterpret_cast<cell *>(params[1] + (params[2] * sizeof(cell))), params[4] * sizeof(cell));
						lastResult = EMemoryResult_OK;
					}
					else
					{
						lastResult = EMemoryResult_InvalidIndexSize;
					}
				}
				else
				{
					lastResult = EMemoryResult_InvalidSize;
				}
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = ManagedMemory::GetLastResult();
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Set value
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Value if successful, otherwise "0"</returns>
/// <remarks>MEM_set_val(Pointer:pointer, index = 0, value)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_set_val(AMX *amx, cell *params)
{
	cell ret(0);
	if (NumArgsEqual(params, 3, "MEM_set_val"))
	{
		cell *ptr(reinterpret_cast<cell *>(params[1]));
		cell size(ManagedMemory::GetSize(ptr));
		if (size > 0)
		{
			if ((params[2] >= 0) && (params[2] < size))
			{
				ptr[params[2]] = params[3];
				ret = params[3];
				lastResult = EMemoryResult_OK;
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = ManagedMemory::GetLastResult();
		}
	}
	return ret;
}

/// <summary>
/// Set array
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of array if successful, otherwise "MEM_NULLFEPTR"</returns>
/// <remarks>ForeignPointer:MEM_set_arr(Pointer:pointer, index = 0, const arr[], arr_size = sizeof arr)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_set_arr(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 4, "MEM_set_arr"))
	{
		cell size(ManagedMemory::GetSize(reinterpret_cast<cell *>(params[1])));
		if (size > 0)
		{
			if ((params[2] >= 0) && (params[2] < params[4]))
			{
				if (params[4] > 0)
				{
					if ((params[4] + params[2]) <= size)
					{
						amx_GetAddr(amx, params[3], &ret);
						memcpy(reinterpret_cast<cell *>(params[1] + (params[2] * sizeof(cell))), ret, params[4] * sizeof(cell));
						lastResult = EMemoryResult_OK;
					}
					else
					{
						lastResult = EMemoryResult_InvalidIndexSize;
					}
				}
				else
				{
					lastResult = EMemoryResult_InvalidSize;
				}
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = ManagedMemory::GetLastResult();
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Copy memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of destination if successful, otherwise "MEM_NULLPTR"</returns>
/// <remarks>Pointer:MEM_copy(Pointer:dest, Pointer:src, size, dest_index = 0, src_index = 0)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_copy(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 5, "MEM_copy"))
	{
		cell dest_size(ManagedMemory::GetSize(reinterpret_cast<cell *>(params[1])));
		if (dest_size > 0)
		{
			cell src_size(ManagedMemory::GetSize(reinterpret_cast<cell *>(params[2])));
			if (src_size > 0)
			{
				if (params[3] > 0)
				{
					if ((dest_size >= (params[3] + params[4])) && (dest_size >= (params[3] + params[5])))
					{
						ret = reinterpret_cast<cell *>(memcpy(reinterpret_cast<cell *>(params[1] + (params[4] * sizeof(cell))), reinterpret_cast<cell *>(params[2] + (params[5] * sizeof(cell))), params[3] * sizeof(cell)));
						lastResult = EMemoryResult_OK;
					}
					else
					{
						lastResult = EMemoryResult_InvalidIndexSize;
					}
				}
				else
				{
					lastResult = EMemoryResult_InvalidSize;
				}
			}
			else
			{
				lastResult = ManagedMemory::GetLastResult();
			}
		}
		else
		{
			lastResult = ManagedMemory::GetLastResult();
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Zero memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of destination if successful, otherwise "MEM_NULLPTR"</returns>
/// <remarks>Pointer:MEM_zero(Pointer:pointer, size, index = 0)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_zero(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 3, "MEM_zero"))
	{
		cell size(ManagedMemory::GetSize(reinterpret_cast<cell *>(params[1])));
		if (size > 0)
		{
			if (params[2] > 0)
			{
				if ((params[3] >= 0) && (params[3] < size))
				{
					if ((params[2] + params[3]) <= size)
					{
						ret = reinterpret_cast<cell *>(memset(reinterpret_cast<cell *>(params[1] + (params[3] * sizeof(cell))), 0, params[2] * sizeof(cell)));
						lastResult = EMemoryResult_OK;
					}
					else
					{
						lastResult = EMemoryResult_InvalidIndexSize;
					}
				}
				else
				{
					lastResult = EMemoryResult_InvalidIndex;
				}
			}
			else
			{
				lastResult = EMemoryResult_InvalidSize;
			}
		}
		else
		{
			lastResult = ManagedMemory::GetLastResult();
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Get last result
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Last result</returns>
/// <remarks>EMemoryResult:MEM_get_last_result()</remarks>
cell AMX_NATIVE_CALL AMX_MEM_get_last_result(AMX *amx, cell *params)
{
	EMemoryResult ret(lastResult);
	lastResult = EMemoryResult_OK;
	return ret;
}

/// <summary>
/// Allocate new unmanaged memory. Delete with "MEM_UM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLUMPTR"</returns>
/// <remarks>UnmanagedPointer:MEM_UM_new(cells = 1)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_new(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 1, "MEM_UM_new"))
	{
		if (params[1] > 0)
		{
			try
			{
				ret = new cell[params[1]];
				lastResult = (ret ? EMemoryResult_OK : EMemoryResult_OutOfMemory);
			}
			catch (...)
			{
				ret = nullptr;
				lastResult = EMemoryResult_OutOfMemory;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidSize;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Allocate new zeroed unmanaged memory. Delete with "MEM_UM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLUMPTR"</returns>
/// <remarks>UnmanagedPointer:MEM_UM_new_zero(cells = 1)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_new_zero(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 1, "MEM_UM_new_zero"))
	{
		if (params[1] > 0)
		{
			try
			{
				ret = new cell[params[1]];
				if (ret)
				{
					memset(ret, 0, params[1] * sizeof(cell));
					lastResult = EMemoryResult_OK;
				}
				else
				{
					lastResult = EMemoryResult_OutOfMemory;
				}
			}
			catch (...)
			{
				ret = nullptr;
				lastResult = EMemoryResult_OutOfMemory;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidSize;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Allocate new unmanaged value. Delete with "MEM_UM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLUMPTR"</returns>
/// <remarks>UnmanagedPointer:MEM_UM_new_val(value)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_new_val(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 1, "MEM_UM_new_val"))
	{
		try
		{
			ret = new cell[1];
			if (ret)
			{
				ret[0] = params[1];
				lastResult = EMemoryResult_OK;
			}
			else
			{
				lastResult = EMemoryResult_OutOfMemory;
			}
		}
		catch (...)
		{
			ret = nullptr;
			lastResult = EMemoryResult_OutOfMemory;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Allocate new unmanaged array. Delete with "MEM_UM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLUMPTR"</returns>
/// <remarks>UnmanagedPointer:MEM_UM_new_arr(const arr[], arr_size = sizeof arr)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_new_arr(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 2, "MEM_UM_new_arr"))
	{
		cell *arr(nullptr);
		amx_GetAddr(amx, params[1], &arr);
		if (params[2] > 0)
		{
			try
			{
				ret = new cell[params[2]];
				if (ret)
				{
					memcpy(ret, arr, params[2] * sizeof(cell));
					lastResult = EMemoryResult_OK;
				}
				else
				{
					lastResult = EMemoryResult_OutOfMemory;
				}
			}
			catch (...)
			{
				ret = nullptr;
				lastResult = EMemoryResult_OutOfMemory;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidSize;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Clone unmanaged memory. Delete with "MEM_UM_delete"
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of allocated memory if successful, otherwise "MEM_NULLUMPTR"</returns>
/// <remarks>UnmanagedPointer:MEM_UM_clone(AnyPointer:pointer, index = 0, cells)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_clone(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 3, "MEM_UM_clone"))
	{
		if (reinterpret_cast<cell *>(params[1]))
		{
			if (params[2] >= 0)
			{
				if (params[3] > 0)
				{
					try
					{
						ret = new cell[params[3]];
						if (ret)
						{
							memcpy(ret, reinterpret_cast<cell *>(params[1] + (params[2] * sizeof(cell))), params[3] * sizeof(cell));
							lastResult = EMemoryResult_OK;
						}
						else
						{
							lastResult = EMemoryResult_OutOfMemory;
						}
					}
					catch (...)
					{
						ret = nullptr;
						lastResult = EMemoryResult_OutOfMemory;
					}
				}
				else
				{
					lastResult = EMemoryResult_InvalidSize;
				}
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidPointer;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Delete unmanaged memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Nothing</returns>
/// <remarks>MEM_UM_delete(UnmanagedPointer:pointer)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_delete(AMX *amx, cell *params)
{
	if (NumArgsEqual(params, 1, "MEM_UM_delete"))
	{
		delete[] reinterpret_cast<cell *>(params[1]);
		lastResult = EMemoryResult_OK;
	}
	return 0;
}

/// <summary>
/// Get value from unmanaged memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Value if successful, otherwise "0"</returns>
/// <remarks>MEM_UM_get_val(AnyPointer:pointer, index = 0)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_get_val(AMX *amx, cell *params)
{
	cell ret(0);
	if (NumArgsEqual(params, 2, "MEM_UM_get_val"))
	{
		cell *ptr(reinterpret_cast<cell *>(params[1]));
		if (ptr)
		{
			if (params[2] >= 0)
			{
				ret = ptr[params[2]];
				lastResult = EMemoryResult_OK;
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidPointer;
		}
	}
	return ret;
}

/// <summary>
/// Get array from unmanaged memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of array if successful, otherwise "MEM_NULLFEPTR"</returns>
/// <remarks>UnmanagedPointer:MEM_UM_get_arr(AnyPointer:pointer, index = 0, arr[], arr_size = sizeof arr)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_get_arr(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 4, "MEM_UM_get_arr"))
	{
		if (reinterpret_cast<cell *>(params[1]))
		{
			if (params[2] >= 0)
			{
				if (params[4] > 0)
				{
					cell *arr(nullptr);
					amx_GetAddr(amx, params[3], &arr);
					ret = reinterpret_cast<cell *>(memcpy(arr, reinterpret_cast<cell *>(params[1] + (params[2] * sizeof(cell))), params[4] * sizeof(cell)));
					lastResult = EMemoryResult_OK;
				}
				else
				{
					lastResult = EMemoryResult_InvalidSize;
				}
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidPointer;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Set value at unmanaged memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Value if successful, otherwise "0"</returns>
/// <remarks>MEM_UM_set_val(UnmanagedPointer:pointer, index = 0, value)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_set_val(AMX *amx, cell *params)
{
	cell ret(0);
	if (NumArgsEqual(params, 3, "MEM_UM_set_val"))
	{
		if (reinterpret_cast<cell *>(params[1]))
		{
			if (params[2] >= 0)
			{
				reinterpret_cast<cell *>(params[1] + (params[2] * sizeof(cell)))[0] = params[3];
				ret = params[3];
				lastResult = EMemoryResult_OK;
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidPointer;
		}
	}
	return ret;
}

/// <summary>
/// Set array at unmanaged memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of array if successful, otherwise "MEM_NULLFEPTR"</returns>
/// <remarks>ForeignPointer:MEM_UM_set_arr(UnmanagedPointer:pointer, index = 0, const arr[], arr_size = sizeof arr)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_set_arr(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 4, "MEM_UM_set_arr"))
	{
		if (reinterpret_cast<cell *>(params[1]))
		{
			if (params[2] >= 0)
			{
				if (params[4] > 0)
				{
					cell *arr(nullptr);
					amx_GetAddr(amx, params[3], &arr);
					ret = reinterpret_cast<cell *>(memcpy(reinterpret_cast<cell *>(params[1] + (params[2] * sizeof(cell))), arr, params[4] * sizeof(cell)));
					lastResult = EMemoryResult_OK;
				}
				else
				{
					lastResult = EMemoryResult_InvalidSize;
				}
			}
			else
			{
				lastResult = EMemoryResult_InvalidIndex;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidPointer;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Copy unmanaged memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of destination if successful, otherwise "MEM_NULLUMPTR"</returns>
/// <remarks>UnmanagedPointer:MEM_UM_copy(UnmanagedPointer:dest, AnyPointer:src, size, dest_index = 0, src_index = 0)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_copy(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 5, "MEM_UM_copy"))
	{
		if (reinterpret_cast<cell *>(params[1]) && reinterpret_cast<cell *>(params[2]))
		{
			if (params[3] > 0)
			{
				if ((params[4] >= 0) && (params[5] >= 0))
				{
					ret = reinterpret_cast<cell *>(memcpy(reinterpret_cast<cell *>(params[1] + (params[4] * sizeof(cell))), reinterpret_cast<cell *>(params[2] + (params[5] * sizeof(cell))), params[3] * sizeof(cell)));
					lastResult = EMemoryResult_OK;
				}
				else
				{
					lastResult = EMemoryResult_InvalidIndex;
				}
			}
			else
			{
				lastResult = EMemoryResult_InvalidSize;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidPointer;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Zero unmanaged memory
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of destination if successful, otherwise "MEM_NULLUMPTR"</returns>
/// <remarks>UnmanagedPointer:MEM_UM_zero(UnmanagedPointer:pointer, size, index = 0)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_zero(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 3, "MEM_UM_zero"))
	{
		if (reinterpret_cast<cell *>(params[1]))
		{
			if (params[2] > 0)
			{
				if (params[3] >= 0)
				{
					ret = reinterpret_cast<cell *>(memset(reinterpret_cast<cell *>(params[1] + (params[3] * sizeof(cell))), 0, params[2] * sizeof(cell)));
					lastResult = EMemoryResult_OK;
				}
				else
				{
					lastResult = EMemoryResult_InvalidIndex;
				}
			}
			else
			{
				lastResult = EMemoryResult_InvalidSize;
			}
		}
		else
		{
			lastResult = EMemoryResult_InvalidPointer;
		}
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Get pointer of variable
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>Pointer of variable if successful, otherwise "MEM_NULLFEPTR"</returns>
/// <remarks>ForeignPointer:MEM_UM_get_addr(&var)</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_get_addr(AMX *amx, cell *params)
{
	cell *ret(nullptr);
	if (NumArgsEqual(params, 1, "MEM_UM_get_addr"))
	{
		amx_GetAddr(amx, params[1], &ret);
		lastResult = EMemoryResult_OK;
	}
	return reinterpret_cast<cell>(ret);
}

/// <summary>
/// Get AMX pointer
/// </summary>
/// <param name="amx">AMX</param>
/// <param name="params">Params</param>
/// <returns>AMX pointer</returns>
/// <remarks>ForeignPointer:MEM_UM_get_amx_ptr()</remarks>
cell AMX_NATIVE_CALL AMX_MEM_UM_get_amx_ptr(AMX *amx, cell *params)
{
	lastResult = EMemoryResult_OK;
	return reinterpret_cast<cell>(amx);
}

/// <summary>
/// Plugin supports
/// </summary>
/// <returns>Plugin supports flags</returns>
PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

/// <summary>
/// Load plugin
/// </summary>
/// <param name="ppData">Plugin data</param>
/// <returns>"true" if successful, otherwise "false"</returns>
PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);
	logprintf("========================");
	logprintf("= Memory access plugin =");
	logprintf("=       Made by BigETI =");
	logprintf("= Loaded!              =");
	logprintf("========================");
	return true;
}

/// <summary>
/// Plugin unload
/// </summary>
PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	ManagedMemory::Clear();
	logprintf("========================");
	logprintf("= Memory access plugin =");
	logprintf("=       Made by BigETI =");
	logprintf("= Unloaded!            =");
	logprintf("========================");
}

/// <summary>
/// Plugin natives
/// </summary>
AMX_NATIVE_INFO PluginNatives[] =
{
	{ "MEM_new", AMX_MEM_new },
	{ "MEM_new_zero", AMX_MEM_new_zero },
	{ "MEM_new_val", AMX_MEM_new_val },
	{ "MEM_new_arr", AMX_MEM_new_arr },
	{ "MEM_clone", AMX_MEM_clone },
	{ "MEM_delete", AMX_MEM_delete },
	{ "MEM_is_valid_ptr", AMX_MEM_is_valid_ptr },
	{ "MEM_get_size", AMX_MEM_get_size },
	{ "MEM_get_val", AMX_MEM_get_val },
	{ "MEM_get_arr", AMX_MEM_get_arr },
	{ "MEM_set_val", AMX_MEM_set_val },
	{ "MEM_set_arr", AMX_MEM_set_arr },
	{ "MEM_copy", AMX_MEM_copy },
	{ "MEM_zero", AMX_MEM_zero },
	{ "MEM_get_last_result", AMX_MEM_get_last_result },

	{ "MEM_UM_new", AMX_MEM_UM_new },
	{ "MEM_UM_new_zero", AMX_MEM_UM_new_zero },
	{ "MEM_UM_new_val", AMX_MEM_UM_new_val },
	{ "MEM_UM_new_arr", AMX_MEM_UM_new_arr },
	{ "MEM_UM_clone", AMX_MEM_UM_clone },
	{ "MEM_UM_delete", AMX_MEM_UM_delete },
	{ "MEM_UM_get_val", AMX_MEM_UM_get_val },
	{ "MEM_UM_get_arr", AMX_MEM_UM_get_arr },
	{ "MEM_UM_set_val", AMX_MEM_UM_set_val },
	{ "MEM_UM_set_arr", AMX_MEM_UM_set_arr },
	{ "MEM_UM_copy", AMX_MEM_UM_copy },
	{ "MEM_UM_zero", AMX_MEM_UM_zero },
	{ "MEM_UM_get_addr", AMX_MEM_UM_get_addr },
	{ "MEM_UM_get_amx_ptr", AMX_MEM_UM_get_amx_ptr },

	{0, 0}
};

/// <summary>
/// AMX load
/// </summary>
/// <param name="amx">AMX</param>
/// <returns>Return value of "amx_Register"</returns>
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	return amx_Register(amx, PluginNatives, -1);
}

/// <summary>
/// AMX unload
/// </summary>
/// <param name="amx">AMX</param>
/// <returns>Error code</returns>
PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	return AMX_ERR_NONE;
}
