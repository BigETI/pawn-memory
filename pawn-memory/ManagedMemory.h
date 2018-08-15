#ifndef __PAWN_MEMORY_MANAGED_MEMORY_H__

#	define __PAWN_MEMORY_MANAGED_MEMORY_H__
#	define HAVE_STDINT_H

#	include <map>
#	include "SDK/plugin.h"
#	include "EMemoryResult.h"

/// <summary>
/// PAWN memory namespace
/// </summary>
namespace PAWNMemory
{
	/// <summary>
	/// Managed memory class
	/// </summary>
	class ManagedMemory
	{
	private:

		/// <summary>
		/// Pointers
		/// </summary>
		static std::map<cell *, cell> pointers;

		/// <summary>
		/// Last result
		/// </summary>
		static EMemoryResult lastResult;

		/// <summary>
		/// Default constructor
		/// </summary>
		ManagedMemory();

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name=""></param>
		ManagedMemory(const ManagedMemory &);

		/// <summary>
		/// Destructor
		/// </summary>
		~ManagedMemory();

		/// <summary>
		/// Assign operator
		/// </summary>
		/// <param name=""></param>
		/// <returns>This object</returns>
		ManagedMemory & operator = (const ManagedMemory &);

	public:

		/// <summary>
		/// New
		/// </summary>
		/// <param name="size">Size</param>
		/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
		static cell *New(cell size);

		/// <summary>
		/// New zero
		/// </summary>
		/// <param name="size">Size</param>
		/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
		static cell *NewZero(cell size);

		/// <summary>
		/// New value
		/// </summary>
		/// <param name="val">Value</param>
		/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
		static cell *NewValue(cell val);

		/// <summary>
		/// New array
		/// </summary>
		/// <param name="arr">Array</param>
		/// <param name="size">Size</param>
		/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
		static cell *NewArray(cell * arr, cell size);

		/// <summary>
		/// Clone
		/// </summary>
		/// <param name="ptr">Pointer</param>
		/// <returns>Pointer of allocated memory if successful, otherwise "nullptr"</returns>
		static cell *Clone(cell * ptr);

		/// <summary>
		/// Delete
		/// </summary>
		/// <param name="ptr">Pointer</param>
		static void Delete(cell * ptr);

		/// <summary>
		/// Is valid pointer
		/// </summary>
		/// <param name="ptr">Pointer</param>
		/// <returns>"true" if pointer is valid, otherwise "false"</returns>
		static bool IsValidPointer(cell * ptr);

		/// <summary>
		/// Get size
		/// </summary>
		/// <param name="ptr">Pointer</param>
		/// <returns>Size of allocated memory</returns>
		static cell GetSize(cell * ptr);

		/// <summary>
		/// Clear
		/// </summary>
		static void Clear();

		/// <summary>
		/// Get last result
		/// </summary>
		/// <returns>Last result</returns>
		static EMemoryResult GetLastResult();
	};
}
#endif
