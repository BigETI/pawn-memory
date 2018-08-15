#include "ManagedMemory.h"
#include <string.h>

using namespace std;
using namespace PAWNMemory;

/// <summary>
/// Pointers
/// </summary>
map<cell *, cell> ManagedMemory::pointers;

/// <summary>
/// Last result
/// </summary>
EMemoryResult ManagedMemory::lastResult(EMemoryResult_OK);

/// <summary>
/// New
/// </summary>
/// <param name="size">Size</param>
/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
cell * ManagedMemory::New(cell size)
{
	cell *ret(nullptr);
	if (size > 0)
	{
		try
		{
			ret = new cell[size];
			if (ret)
			{
				pointers[ret] = size;
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
	return ret;
}

/// <summary>
/// New zero
/// </summary>
/// <param name="size">Size</param>
/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
cell * ManagedMemory::NewZero(cell size)
{
	cell *ret(nullptr);
	if (size > 0)
	{
		try
		{
			ret = new cell[size];
			if (ret)
			{
				pointers[ret] = size;
				memset(ret, 0, size * sizeof(cell));
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
	return ret;
}

/// <summary>
/// New value
/// </summary>
/// <param name="val">Value</param>
/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
cell * ManagedMemory::NewValue(cell val)
{
	cell *ret(nullptr);
	try
	{
		ret = new cell[1];
		if (ret)
		{
			pointers[ret] = 1;
			ret[0] = val;
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
	return ret;
}

/// <summary>
/// New array
/// </summary>
/// <param name="arr">Array</param>
/// <param name="size">Size</param>
/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
cell * ManagedMemory::NewArray(cell * arr, cell size)
{
	cell *ret(nullptr);
	if (size > 0)
	{
		try
		{
			ret = new cell[size];
			if (ret)
			{
				pointers[ret] = size;
				memcpy(ret, arr, size * sizeof(cell));
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
	return ret;
}

/// <summary>
/// Clone
/// </summary>
/// <param name="ptr">Pointer</param>
/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
cell * ManagedMemory::Clone(cell * ptr)
{
	cell *ret(nullptr);
	map<cell *, cell>::iterator it(pointers.find(ptr));
	if (it != pointers.end())
	{
		try
		{
			ret = new cell[it->second];
			if (ret)
			{
				pointers[ret] = it->second;
				memcpy(ret, ptr, it->second * sizeof(cell));
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
		lastResult = EMemoryResult_InvalidPointer;
	}
	return ret;
}

/// <summary>
/// Delete
/// </summary>
/// <param name="ptr">Pointer</param>
void ManagedMemory::Delete(cell * ptr)
{
	map<cell *, cell>::iterator it(pointers.find(ptr));
	if (it != pointers.end())
	{
		delete[] it->first;
		pointers.erase(ptr);
		lastResult = EMemoryResult_OK;
	}
	else
	{
		lastResult = EMemoryResult_InvalidPointer;
	}
}

/// <summary>
/// Is valid pointer
/// </summary>
/// <param name="ptr">Pointer</param>
/// <returns>"true" if pointer is valid, otherwise "false"</returns>
bool ManagedMemory::IsValidPointer(cell * ptr)
{
	lastResult = EMemoryResult_OK;
	return (pointers.find(ptr) != pointers.end());
}

/// <summary>
/// Get size
/// </summary>
/// <param name="ptr">Pointer</param>
/// <returns>Size of allocated memory</returns>
cell ManagedMemory::GetSize(cell * ptr)
{
	cell ret(0);
	map<cell *, cell>::iterator it(pointers.find(ptr));
	if (it != pointers.end())
	{
		ret = it->second;
		lastResult = EMemoryResult_OK;
	}
	else
	{
		lastResult = EMemoryResult_InvalidSize;
	}
	return ret;
}

/// <summary>
/// Clear
/// </summary>
void ManagedMemory::Clear()
{
	for (pair<cell *, cell> pointer : pointers)
	{
		delete[] pointer.first;
	}
	pointers.clear();
	lastResult = EMemoryResult_OK;
}

/// <summary>
/// Get last result
/// </summary>
/// <returns>Last result</returns>
EMemoryResult ManagedMemory::GetLastResult()
{
	EMemoryResult ret(lastResult);
	lastResult = EMemoryResult_OK;
	return ret;
}
