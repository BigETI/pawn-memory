#ifndef __PAWN_MEMORY_E_MEMORY_RESULT_H__
#	define __PAWN_MEMORY_E_MEMORY_RESULT_H__

/// <summary>
/// PAWN memory namespace
/// </summary>
namespace PAWNMemory
{
	/// <summary>
	/// Memory result enumerator
	/// </summary>
	enum EMemoryResult
	{
		/// <summary>
		/// OK
		/// </summary>
		EMemoryResult_OK,

		/// <summary>
		/// Invalid size
		/// </summary>
		EMemoryResult_InvalidSize,

		/// <summary>
		/// Invalid pointer
		/// </summary>
		EMemoryResult_InvalidPointer,

		/// <summary>
		/// Invalid index
		/// </summary>
		EMemoryResult_InvalidIndex,

		/// <summary>
		/// Invalid index and size
		/// </summary>
		EMemoryResult_InvalidIndexSize,

		/// <summary>
		/// Out of memory
		/// </summary>
		EMemoryResult_OutOfMemory
	};
}
#endif
