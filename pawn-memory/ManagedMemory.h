#ifndef __PAWN_MEMORY_MANAGED_MEMORY_H__

#	define __PAWN_MEMORY_MANAGED_MEMORY_H__
#	define HAVE_STDINT_H

#	include <map>
#	include "SDK/plugin.h"
#	include "EMemoryResult.h"

// PAWN memory namespace
namespace PAWNMemory
{
	// Managed memory class
	class ManagedMemory
	{
	private:

		// Pointers
		static std::map<cell *, cell> pointers;

		// Last result
		static EMemoryResult lastResult;

		// Default constructor
		ManagedMemory();

		// Copy constructor
		ManagedMemory(const ManagedMemory &);

		// Destructor
		~ManagedMemory();

		// Assign operator
		ManagedMemory & operator = (const ManagedMemory &);

	public:

		// New
		static cell *New(cell size);

		// New zero
		static cell *NewZero(cell size);

		// New value
		static cell *NewValue(cell val);

		// New array
		static cell *NewArray(cell * arr, cell size);

		// Clone
		static cell *Clone(cell * ptr);

		// Delete
		static void Delete(cell * ptr);

		// Is valid pointer
		static bool IsValidPointer(cell * ptr);

		// Get size
		static cell GetSize(cell * ptr);

		// Clear
		static void Clear();

		// Get last result
		static EMemoryResult GetLastResult();
	};
}
#endif
