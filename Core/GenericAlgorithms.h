/// GenericAlgorithms.h:
/// This file contains methods for functionality commonly used with 
/// generic associative containers such as those found in the Standard Template Library.

#pragma once

namespace Core
{
	/// <summary>
	/// Generic method to determine if a collection contains an element associated
	/// with the provided key.
	/// </summary>
	template <typename Key, typename Collection>
	bool Contains(const Key& key, const Collection& collection)
	{
		return (collection.find(key) != collection.end());
	}

}