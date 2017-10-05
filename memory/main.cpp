/*
	Memory access plugin made by BigETI © 2013
*/

#define SECURE_MEMORY_PLUGIN
#define HAVE_STDINT_H

#include <string.h>
#include <ctime>
#include <cstdlib>
#include "SDK/plugin.h"

typedef void(*logprintf_t)(char* format, ...);
logprintf_t logprintf;
extern void *pAMXFunctions;

class MemClass
{
private:
	cell *ptr;
	size_t len;
public:
	MemClass(size_t _len, bool zero = false)
	{
		ptr = zero ? ((cell *)calloc(_len, sizeof(cell))) : ((cell *)malloc(_len * sizeof(cell)));
		if (ptr == NULL) throw - 1;
		len = _len;
	}

	~MemClass()
	{
		free(ptr);
	}

	cell *realloc(size_t _len)
	{
		cell *ret = (cell *)::realloc(ptr, _len * sizeof(cell));
		if (ret)
		{
			ptr = ret;
			len = _len;
		}
		else throw - 1;
		return ret;
	}

	cell &operator[](size_t index)
	{
		if (index >= len) throw - 1;
		return ptr[index];
	}

	size_t count()
	{
		return len;
	}

	cell *get_ptr()
	{
		return ptr;
	}
};
typedef MemClass*	PMemClass;

#ifdef SECURE_MEMORY_PLUGIN
#	include "p_mem_class_ctr.h"
#endif

using namespace std;

#ifdef SECURE_MEMORY_PLUGIN
	#define list_foreach(_type,_list,_var)	for(list<_type>::iterator (_var)=(_list).begin();(_var)!=(_list).end();++(_var))

enum MEM_RESULT_ENUM
{
	MEM_RESULT_OK,
	MEM_RESULT_NO_ALLOC,
	MEM_RESULT_INVALID_PTR,
	MEM_RESULT_NEG_INDEX,
	MEM_RESULT_INVALID_INDEX,
	MEM_RESULT_NULL_ARR,
	MEM_RESULT_INVALID_SIZE,
	MEM_RESULT_INVALID_OPERATION
} mem_result;

static PMemClassCTR addresses;
#endif

#define SWAP_VAR(var1,var2)	(var1^=(var2^=(var1^=var2)))

enum MEM_SORT_TYPE_ENUM
{
	MEM_SORT_TYPE_DEFAULT,
	MEM_SORT_TYPE_REVERSE
};

// native Pointer:MEM_malloc(cells = 1);
cell AMX_NATIVE_CALL AMX_MEM_malloc(AMX *amx, cell *params)
{
#ifdef SECURE_MEMORY_PLUGIN
	PMemClass ret = NULL;
	cell *buf = NULL;
	try
	{
		ret = new MemClass(params[1]);
	}
	catch (...)
	{
		ret = NULL;
	}
	if (ret)
	{
		try
		{
			addresses.push_back(ret);
		}
		catch (...)
		{
			mem_result = MEM_RESULT_NO_ALLOC;
			delete ret;
			ret = NULL;
		}
	}
	else
	{
		mem_result = MEM_RESULT_NO_ALLOC;
		logprintf(" [Memory Plugin] MEM_malloc(): Failed to allocate memory: %d bytes", params[1] * sizeof(cell));
		return NULL;
	}
	return (cell)ret;
#else
	return (cell)malloc((size_t)params[1]*sizeof(cell));
#endif
}

// native Pointer:MEM_calloc(cells = 1);
cell AMX_NATIVE_CALL AMX_MEM_calloc(AMX *amx, cell *params)
{
#ifdef SECURE_MEMORY_PLUGIN
	MemClass *ret;
	try
	{
		ret = new MemClass(params[1], true);
	}
	catch (...)
	{
		ret = NULL;
	}
	if (ret)
	{
		try
		{
			addresses.push_back(ret);
		}
		catch (...)
		{
			mem_result = MEM_RESULT_NO_ALLOC;
			delete ret;
			ret = NULL;
		}
	}
	else
	{
		mem_result = MEM_RESULT_NO_ALLOC;
		logprintf(" [Memory Plugin] MEM_calloc(): Failed to allocate memory: %d bytes", params[1] * sizeof(cell));
		return NULL;
	}
	return (cell)ret;
#else
	return (cell)calloc((size_t)params[1], sizeof(cell));
#endif
}

// native Pointer:MEM_realloc(Pointer:pointer, cells = 1)
cell AMX_NATIVE_CALL AMX_MEM_realloc(AMX *amx, cell *params)
{
#ifdef SECURE_MEMORY_PLUGIN
	/*addr_temp.addr = realloc((void *)params[1], (addr_temp.len = (size_t)params[2])*sizeof(cell));
	if(addr_temp.addr == NULL)
	{
		mem_result = MEM_RESULT_NO_ALLOC;
		logprintf(" [Memory Plugin] MEM_realloc(): Failed to re-allocate memory from 0x%x: %d bytes", (size_t)params[1], addr_temp.len*sizeof(cell));
		return NULL;
	}
	addr_vect.push_back(addr_temp);
	return (cell)addr_temp.addr;*/
	MemClass *ret = (PMemClass)(params[1]);
	if (addresses.exists(ret))
	{
		if (ret->realloc(params[2]) == NULL) ret = NULL;
	}
	else
	{
		try
		{
			ret = new MemClass(params[2], true);
		}
		catch (...)
		{
			ret = NULL;
		}
		if (ret)
		{
			try
			{
				addresses.push_back(ret);
			}
			catch (...)
			{
				mem_result = MEM_RESULT_NO_ALLOC;
				delete ret;
				ret = NULL;
			}
		}
		else
		{
			mem_result = MEM_RESULT_NO_ALLOC;
			logprintf(" [Memory Plugin] MEM_realloc(): Failed to allocate memory: %d bytes", params[1] * sizeof(cell));
			ret = NULL;
		}
	}
	return (cell)ret;
#else
	return (cell)realloc((void *)params[1], (size_t)params[2]*sizeof(cell));
#endif
}

// native MEM_free(Pointer:pointer);
cell AMX_NATIVE_CALL AMX_MEM_free(AMX *amx, cell *params)
{
#ifdef SECURE_MEMORY_PLUGIN
	/*vector_foreach(ADDR_STRUCT, addr_vect, addr_it)
	{
		if(addr_it->addr != (void *)params[1]) continue;
		free(addr_it->addr);
		addr_vect.erase(addr_it);
		return 0;
	}*/
	MemClass *buf = (PMemClass)(params[1]);
	if (addresses.exists(buf))
	{
		addresses.erase(buf);
		delete buf;
	}
	else mem_result = MEM_RESULT_INVALID_PTR;
#else
	free((void *)(params[1]));
#endif
	return 0;
}

// native MEM_get_val(Pointer:pointer, index = 0);
cell AMX_NATIVE_CALL AMX_MEM_get_val(AMX *amx, cell *params)
{
#ifdef SECURE_MEMORY_PLUGIN
	cell ret = 0;
	PMemClass buf = (PMemClass)(params[1]);
	if (addresses.exists(buf))
	{
		try
		{
			ret = (*buf)[params[2]];
		}
		catch (...)
		{
			mem_result = (params[2] < 0) ? MEM_RESULT_NEG_INDEX : MEM_RESULT_INVALID_INDEX;
		}
	}
	else mem_result = MEM_RESULT_INVALID_PTR;
	return ret;
#else
	return *(cell *)((size_t)params[1]+(params[2]*sizeof(cell)));
#endif
}

// native Pointer:MEM_get_arr(Pointer:pointer, index = 0, arr[], arr_size = sizeof arr);
cell AMX_NATIVE_CALL AMX_MEM_get_arr(AMX *amx, cell *params)
{
	cell *ret;
#ifdef SECURE_MEMORY_PLUGIN
	PMemClass buf = (PMemClass)(params[1]);
	amx_GetAddr(amx, params[3], &ret);
	if (params[4] > 0)
	{
		if (addresses.exists(buf))
		{
			if (buf->count() >= (((size_t)(params[2])) + ((size_t)(params[4]))))
			{
				try
				{
					memcpy(ret, &((*buf)[params[2]]), params[4] * sizeof(cell));
				}
				catch (...)
				{
					mem_result = (params[2] < 0) ? MEM_RESULT_NEG_INDEX : MEM_RESULT_INVALID_INDEX;
				}
			}
			else mem_result = MEM_RESULT_INVALID_SIZE;
		}
		else mem_result = MEM_RESULT_INVALID_PTR;
	}
	else
	{
		mem_result = MEM_RESULT_NULL_ARR;
		ret = NULL;
	}
#else
	amx_GetAddr(amx, params[3], &ret);
	memcpy(ret, (void *)((size_t)params[1]+(size_t)params[2]*sizeof(cell)), params[4]*sizeof(cell));
#endif
	return (cell)ret;
}

// native MEM_set_val(Pointer:pointer, index = 0, value);
cell AMX_NATIVE_CALL AMX_MEM_set_val(AMX *amx, cell *params)
{
#ifdef SECURE_MEMORY_PLUGIN
	PMemClass buf = (PMemClass)(params[1]);
	if (addresses.exists(buf))
	{
		try
		{
			(*buf)[params[2]] = params[3];
		}
		catch (...)
		{
			mem_result = (params[2] < 0) ? MEM_RESULT_NEG_INDEX : MEM_RESULT_INVALID_INDEX;
		}
	}
	else mem_result = MEM_RESULT_INVALID_PTR;
	return params[3];
#endif
	return *(cell *)((size_t)params[1]+(params[2]*sizeof(cell))) = params[3];
}

// native Pointer:MEM_set_arr(Pointer:pointer, index = 0, const arr[], arr_size = sizeof arr);
cell AMX_NATIVE_CALL AMX_MEM_set_arr(AMX *amx, cell *params)
{
	cell *ret;
#ifdef SECURE_MEMORY_PLUGIN
	PMemClass buf = (PMemClass)(params[1]);
	amx_GetAddr(amx, params[3], &ret);
	if (params[4] > 0)
	{
		if (addresses.exists(buf))
		{
			if (buf->count() >= (((size_t)(params[2])) + ((size_t)(params[4]))))
			{
				try
				{
					memcpy(&((*buf)[params[2]]), ret, params[4] * sizeof(cell));
				}
				catch (...)
				{
					mem_result = (params[2] < 0) ? MEM_RESULT_NEG_INDEX : MEM_RESULT_INVALID_INDEX;
				}
			}
			else
			{
				mem_result = MEM_RESULT_INVALID_SIZE;
				ret = NULL;
			}
		}
		else
		{
			mem_result = MEM_RESULT_INVALID_PTR;
			ret = NULL;
		}
	}
	else
	{
		mem_result = MEM_RESULT_NULL_ARR;
		ret = NULL;
	}
#else
	amx_GetAddr(amx, params[3], &ret);
	memcpy((void *)((size_t)params[1]+(params[2]*sizeof(cell))), ret, params[4]*sizeof(cell));
#endif
	return (cell)ret;
}

// native Pointer:MEM_copy(Pointer:dest, Pointer:src, size, dest_index = 0, src_index = 0);
cell AMX_NATIVE_CALL AMX_MEM_copy(AMX *amx, cell *params)
{
#ifdef SECURE_MEMORY_PLUGIN
	PMemClass ret = NULL;
	PMemClass buf1 = (PMemClass)(params[1]), buf2 = (PMemClass)(params[2]);
	if (params[3] > 0)
	{
		if (addresses.exists(buf1) && addresses.exists(buf2))
		{
			if ((buf1->count() >= (((size_t)(params[3])) + ((size_t)(params[4])))) && (buf2->count() >= (((size_t)(params[3])) + ((size_t)(params[5])))))
			{
				try
				{
					memcpy(&((*buf1)[params[4]]), &((*buf2)[params[5]]), params[3] * sizeof(cell));
					ret = buf1;
				}
				catch (...)
				{
					mem_result = ((params[4] < 0) || (params[5] < 0)) ? MEM_RESULT_NEG_INDEX : MEM_RESULT_INVALID_INDEX;
				}
			}
			else mem_result = MEM_RESULT_INVALID_SIZE;
		}
		else mem_result = MEM_RESULT_INVALID_PTR;
	}
	else mem_result = MEM_RESULT_INVALID_SIZE;
	return (cell)ret;
#else
	return (cell)(memcpy((void *)((size_t)params[1]+((size_t)params[4]*sizeof(cell))), (void *)((size_t)params[2]+((size_t)params[5]*sizeof(cell))), (size_t)params[3]*sizeof(cell)));
#endif
}

// native MEM_zero(Pointer:pointer, size, index = 0);
cell AMX_NATIVE_CALL AMX_MEM_zero(AMX *amx, cell *params)
{
#ifdef SECURE_MEMORY_PLUGIN
	PMemClass ret = (PMemClass)(params[1]);
	if (params[2] > 0)
	{
		if (addresses.exists(ret))
		{
			if (ret->count() >= (((size_t)(params[2])) + ((size_t)(params[3]))))
			{
				try
				{
					memset(&((*ret)[params[3]]), 0, params[2] * sizeof(cell));
				}
				catch (...)
				{
					mem_result = (params[3] < 0) ? MEM_RESULT_NEG_INDEX : MEM_RESULT_INVALID_INDEX;
					ret = NULL;
				}
			}
			else
			{
				mem_result = MEM_RESULT_INVALID_SIZE;
				ret = NULL;
			}
		}
		else
		{
			mem_result = MEM_RESULT_INVALID_PTR;
			ret = NULL;
		}
	}
	else
	{
		mem_result = MEM_RESULT_INVALID_SIZE;
		logprintf(" [Memory Plugin] MEM_zero(): Less than 1 cell to set zero. Amount: %d", params[2]);
		return 0;
	}
	return (cell)ret;
	//if(!mem_is((void *)params[1], params[3]+(params[2]-1))) return 0;
#else
	return (cell)(memset((void *)((size_t)params[1]+((size_t)params[3]*sizeof(cell))), 0, (size_t)params[2]*sizeof(cell)));
#endif
}

// native Pointer:MEM_get_addr(&var);
cell AMX_NATIVE_CALL AMX_MEM_get_addr(AMX *amx, cell *params)
{
	cell *ret;
	amx_GetAddr(amx, params[1], &ret);
	return (cell)ret;
}

// native bool:MEM_sort(Pointer:pointer, index = 0, cells = 2, MEM_E_sort_type:sort = MEM_ENUM_sort_default);
cell AMX_NATIVE_CALL AMX_MEM_sort(AMX *amx, cell *params)
{
	cell ret = 0;
	bool allow_repeat = true;
	size_t i, size = ((size_t)params[3] - 1);
#ifdef SECURE_MEMORY_PLUGIN
	PMemClass buf = (PMemClass)(params[1]);
	cell *ptr = NULL;
#else
	cell *ptr = ((cell *)params[1] + (params[2] * sizeof(cell)));
#endif
	if (params[3] > 2)
	{
#ifdef SECURE_MEMORY_PLUGIN
		if (addresses.exists(buf))
		{
			if (buf->count() >= (((size_t)(params[2])) + ((size_t)(params[3]))))
			{
				if (params[2] >= 0)
				{
					ptr = buf->get_ptr();
#endif
					switch ((MEM_SORT_TYPE_ENUM)params[4])
					{
					case MEM_SORT_TYPE_DEFAULT:
						while (allow_repeat)
						{
							allow_repeat = false;
							for (i = 0; i < size; i++)
							{
								if (ptr[i] <= ptr[i + 1]) continue;
								allow_repeat = true;
								SWAP_VAR(ptr[i], ptr[i + 1]);
							}
						}
						break;
					case MEM_SORT_TYPE_REVERSE:
						while (allow_repeat)
						{
							allow_repeat = false;
							for (i = 0; i < size; i++)
							{
								if (ptr[i] >= ptr[i + 1]) continue;
								allow_repeat = true;
								SWAP_VAR(ptr[i], ptr[i + 1]);
							}
						}
						break;
					default:
						logprintf(" [Memory Plugin] MEM_sort(): Wrong sort type ID %d", params[4]);
						return 0;
					}
#if defined SECURE_MEMORY_PLUGIN
				}
				else
				{
					mem_result = MEM_RESULT_INVALID_INDEX;
				}
			}
			else
			{
				mem_result = MEM_RESULT_INVALID_SIZE;
			}
		}
		else
		{
			mem_result = MEM_RESULT_INVALID_PTR;
		}
#endif
	}
	else
	{
#ifdef SECURE_MEMORY_PLUGIN
		mem_result = MEM_RESULT_INVALID_OPERATION;
#endif
		logprintf(" [Memory Plugin] MEM_sort(): Less than 2 cells to sort. Amount: %d", params[3]);
	}
	return ret;
}

// native bool:MEM_mix(Pointer:pointer, index = 0, cells = 2, mix_times = 5);
cell AMX_NATIVE_CALL AMX_MEM_mix(AMX *amx, cell *params)
{
	cell ret = 0, *mix_arr = NULL, *addr = NULL, i, j, k, id;
#ifdef SECURE_MEMORY_PLUGIN
	PMemClass buf = (PMemClass)(params[1]);
#endif
	if (params[3] >= 2)
	{
		if (params[4] >= 2)
		{
#ifdef SECURE_MEMORY_PLUGIN
			if (params[2] > 0)
			{
				if (addresses.exists(buf))
				{
					if (buf->count() >= (((size_t)(params[2])) + ((size_t)(params[3]))))
					{
						addr = buf->get_ptr();
#else
						addr = (cell *)(params[1] + (params[2] * sizeof(cell)));
#endif
						try
						{
							mix_arr = new cell[params[3]];
						}
						catch (...)
						{
							mix_arr = NULL;
						}
						if (mix_arr)
						{

							for (i = 0; i < params[4]; i++)
							{
								for (j = 0; j < params[3]; j++) mix_arr[j] = addr[j];
								for (j = (params[3] - 1); j >= 0; j--)
								{
									addr[j] = mix_arr[id = (rand() % (j + 1))];
									for (k = id; k < (params[3] - 1); k++) mix_arr[k] = mix_arr[k + 1];
								}
							}
							delete[] mix_arr;
							ret = 1;
						}
						else
						{
#ifdef SECURE_MEMORY_PLUGIN
							mem_result = MEM_RESULT_INVALID_OPERATION;
#endif
							logprintf(" [Memory Plugin] MEM_mix(): Failed to mix memory due to lack of memory. %d bytes", params[3] * sizeof(cell));
						}
#ifdef SECURE_MEMORY_PLUGIN
					}
					else mem_result = MEM_RESULT_INVALID_SIZE;
				}
				else mem_result = MEM_RESULT_INVALID_PTR;
			}
			else mem_result = MEM_RESULT_NEG_INDEX;
#endif
		}
		else logprintf(" [Memory Plugin] MEM_mix(): Less than 1 time to mix. Times: %d", params[4]);
	}
	else logprintf(" [Memory Plugin] MEM_mix(): Less than 2 cells to mix. Amount: %d", params[3]);
	return ret;
}

// native Pointer:MEM_amx_ptr();
cell AMX_NATIVE_CALL AMX_MEM_amx_ptr(AMX *amx, cell *params)
{
	return (cell)amx;
}

#ifdef SECURE_MEMORY_PLUGIN
// native MEM_is(Pointer:pointer, index = 0);
cell AMX_NATIVE_CALL AMX_MEM_is(AMX *amx, cell *params)
{
	return addresses.exists((PMemClass)(params[1])) ? ((params[2] < 0) ? 0 : ((cell)(((PMemClass)(params[1]))->count() > (size_t)(params[2])))) : 0;
}

// native MEM_len(Pointer:pointer);
cell AMX_NATIVE_CALL AMX_MEM_len(AMX *amx, cell *params)
{
	cell ret = 0;
	if (addresses.exists((PMemClass)(params[1]))) ret = ((PMemClass)(params[1]))->count();
	return ret;
}

// native MEM_E_res:MEM_result(bool:free_result = true);
cell AMX_NATIVE_CALL AMX_MEM_result(AMX *amx, cell *params)
{
	cell ret = (cell)mem_result;
	if(params[1]) mem_result = MEM_RESULT_OK;
	return ret;
}
#endif

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION|SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	srand((unsigned int)time(NULL));
	logprintf("========================");
	logprintf("= Memory access plugin =");
	logprintf("=       Made by BigETI =");
	logprintf("= Loaded!              =");
	logprintf("========================");
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
#ifdef SECURE_MEMORY_PLUGIN
	list_foreach(PMemClass, addresses._list, it) delete (*it);
	addresses.clear();
#endif
	logprintf("========================");
	logprintf("= Memory access plugin =");
	logprintf("=       Made by BigETI =");
	logprintf("= Unloaded!            =");
	logprintf("========================");
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{"MEM_malloc", AMX_MEM_malloc},
	{"MEM_calloc", AMX_MEM_calloc},
	{"MEM_realloc", AMX_MEM_realloc},
	{"MEM_free", AMX_MEM_free},
	{"MEM_get_val", AMX_MEM_get_val},
	{"MEM_get_arr", AMX_MEM_get_arr},
	{"MEM_set_val", AMX_MEM_set_val},
	{"MEM_set_arr", AMX_MEM_set_arr},
	{"MEM_copy", AMX_MEM_copy},
	{"MEM_zero", AMX_MEM_zero},
	{"MEM_get_addr", AMX_MEM_get_addr},
	{"MEM_sort", AMX_MEM_sort},
	{"MEM_mix", AMX_MEM_mix},
	{"MEM_amx_ptr", AMX_MEM_amx_ptr},
#ifdef SECURE_MEMORY_PLUGIN
	{"MEM_is", AMX_MEM_is},
	{"MEM_len", AMX_MEM_len},
	{"MEM_result", AMX_MEM_result},
#endif
	{0, 0}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	return AMX_ERR_NONE;
}
