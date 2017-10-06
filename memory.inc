#if defined _MEMORY_INCLUDED_
	#endinput
#endif
#define _MEMORY_INCLUDED_
#if defined MEM_NULL_EX
	#define MEM_MACR_NULL				(0)
#else
	#if !defined NULL
		#define NULL					(0)
	#endif
#endif
#define MEM_struct						enum _:
#define MEM_MACR_SIZEOF_CELL			(4)
#define MEM_MACR_get_addr%2(%0->%1)		(MEM_get_addr(%0)+Pointer:((%1)*MEM_MACR_SIZEOF_CELL))
#define MEM_MACR_get_val%2(%0->%1)		MEM_get_val(%0,%1)
#define MEM_MACR_get_ptr%2(%0->%1)		(%0+Pointer:((%1)*MEM_MACR_SIZEOF_CELL))
#define MEM_MACR_foreach%2(%0,%1)		for(new %1=0;(%1)<(%0);%1++)
#define MEM_MACR_foreach_rev%2(%0,%1)	for(new %1=((%0)-MEM_MACR_SIZEOF_CELL);(%1)>=0;%1--)
#define MEM_MACR_increment_ptr%1(%0)	(%0+=MEM_MACR_SIZEOF_CELL)
#define MEM_MACR_decrement_ptr%1(%0)	(%0-=MEM_MACR_SIZEOF_CELL)
enum MEM_E_sort_type
{
	MEM_E_sort_default,
	MEM_E_sort_reverse
}
native Pointer:MEM_malloc(cells = 1);
native Pointer:MEM_calloc(cells = 1);
native Pointer:MEM_realloc(Pointer:pointer, cells = 1);
native MEM_free(Pointer:pointer);
native MEM_get_val(Pointer:pointer, index = 0);
native Pointer:MEM_get_arr(Pointer:pointer, index = 0, arr[], arr_size = sizeof arr);
native MEM_set_val(Pointer:pointer, index = 0, value);
native Pointer:MEM_set_arr(Pointer:pointer, index = 0, const data_arr[], arr_size = sizeof data_arr);
native Pointer:MEM_copy(Pointer:dest, Pointer:src, len, dest_index = 0, src_index = 0);
//native MEM_compare(Pointer:lhs, Pointer:rhs, size, lhs_offset = 0, rhs_offset = 0);
native Pointer:MEM_zero(Pointer:pointer, size, index = 0);
native Pointer:MEM_get_addr(&var);
native bool:MEM_sort(Pointer:pointer, index = 0, cells = 2, MEM_E_sort_type:sort = MEM_E_sort_default);
native bool:MEM_mix(Pointer:pointer, index = 0, cells = 2, mix_times = 5);
native Pointer:MEM_amx_ptr();
#if defined SECURE_MEMORY_PLUGIN
enum MEM_E_res
{
	MEM_E_res_ok,
	MEM_E_res_no_alloc,
	MEM_E_res_inv_ptr,
	MEM_E_res_neg_index,
	MEM_E_res_inv_index,
	MEM_E_res_null_arr,
	MEM_E_res_inv_size,
	MEM_E_res_inv_op
}
native bool:MEM_is(Pointer:pointer, index = 0);
native MEM_len(Pointer:pointer);
native MEM_E_res:MEM_result(bool:free_result = true);
#endif
#define MEM::							MEM_
#define MEMORY::						MEM_
#define MEM_EX::						MEM_MACR_
#define MEMORY_EX::						MEM_MACR_
#define MEM_MACR::						MEM_MACR_
#define MEMORY_MACRO::					MEM_MACR_
#define MEM_E::							MEM_E_
#define MEM_ENUM::						MEM_E_
#define MEMORY_ENUM::					MEM_E_