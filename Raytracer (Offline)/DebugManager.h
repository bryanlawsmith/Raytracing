#pragma once

#include "FrameBuffer.h"
#include <MathLib.h>
#include <vector>
#include <Line.h>
#include <Color.h>

using namespace CommonCode;
using namespace Raytracer;
using namespace MathLib;
using namespace std;

namespace Debugging
{
	class DebugNode;

	/// <summary>
	/// The DebugManager is the hub for all debugging commands to be sent to.
	/// </summary>
	class DebugManager
	{
	public:

		/// <summary>Returns the singleton instance of the DebugManager.</summary>
		static DebugManager& GetInstance();

		/// <summary>Frees all resources under the scope of this object.</summary>
		~DebugManager();
		DebugManager(const DebugManager& other) = delete;
		DebugManager(DebugManager&& other) = delete;
		DebugManager& operator=(const DebugManager& other) = delete;
		DebugManager& operator=(DebugManager&& other) = delete;

		/// <summary>Adds an object space to world space transform node to the manager list.</summary>
		void AddTransform(const matrix4x4& transform);
		
		/// <summary>Adds a line node to the manager list.</summary>
		void AddLine(const GeometryLib::Line& line, const Color& color);

		/// <summary>
		/// Processes the list of DebugNodes given to the manager.
		/// NOTE: This will remove all of the nodes from the DebugManager.
		/// </summary>
		void Process();

		/// <summary>Returns a flag specifying whether debugging is active or not.</summary>
		bool GetEnabled() const;

		/// <summary>Sets the flag specifying whether debugging is active or not.</summary>
		void SetEnabled(bool enabled);

		/// <summary>Returns the current object to world space transform.</summary>
		const matrix4x4& GetObjectToWorldTransform() const;

		/// <summary>Sets the current object to world space transform for points being processed by this debug manager.</summary>
		void SetObjectToWorldTransform(const matrix4x4& transform);

		/// <summary>Gets the current world to clip space transform.</summary>
		const matrix4x4& GetWorldViewProjectionTransform() const;

		/// <summary>Sets the current world to clip space transform for points being processed by this debug manager.</summary>
		void SetWorldViewProjectionTransform(const matrix4x4& transform);

		/// <summary>Returns a pointer to the DebugManager's FrameBuffer object.</summary>
		FrameBuffer* GetFrameBuffer() const;

		/// <summary>Sets the frame buffer target of the DebugManager.</summary>
		void SetFrameBuffer(FrameBuffer* frameBuffer);

	protected:

		bool m_Enabled;

		FrameBuffer* m_FrameBuffer;

		vector<DebugNode*> m_Nodes;

		matrix4x4 m_ObjectToWorldTransform;
		matrix4x4 m_WorldViewProjectionTransform;

		static DebugManager* s_Instance;

		/// <summary>Performs all construction tasks when a DebugManager object is instantiated.</summary>
		DebugManager();
	};
}
