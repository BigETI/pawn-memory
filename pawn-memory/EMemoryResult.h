#ifndef __PAWN_MEMORY_E_MEMORY_RESULT_H__
#	define __PAWN_MEMORY_E_MEMORY_RESULT_H__

// PAWN memory namespace
namespace PAWNMemory
{
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

		// Invalid index and size
		EMemoryResult_InvalidIndexSize,

		// Out of memory
		EMemoryResult_OutOfMemory
	};
}
#endif
