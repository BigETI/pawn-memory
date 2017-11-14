#include "ManagedMemory.h"

using namespace PAWNMemory;
using namespace std;

map<cell *, cell> ManagedMemory::pointers;

EMemoryResult ManagedMemory::lastResult(EMemoryResult_OK);

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

bool ManagedMemory::IsValidPointer(cell * ptr)
{
	lastResult = EMemoryResult_OK;
	return (pointers.find(ptr) != pointers.end());
}

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

void ManagedMemory::Clear()
{
	for (pair<cell *, cell> pointer : pointers)
	{
		delete[] pointer.first;
	}
	pointers.clear();
	lastResult = EMemoryResult_OK;
}

EMemoryResult ManagedMemory::GetLastResult()
{
	EMemoryResult ret(lastResult);
	lastResult = EMemoryResult_OK;
	return ret;
}
