#pragma once

namespace Debugging
{
	/// <summary>
	/// Base class for all DebugNodes. 
	/// DebugNodes contain some information needed for debugging purposes.
	/// </summary>
	class DebugNode
	{
	public:

		/// <summary>
		/// Performs all actions related to processing this node.
		/// </summary>
		virtual void Process() = 0;

	protected:
	};
}