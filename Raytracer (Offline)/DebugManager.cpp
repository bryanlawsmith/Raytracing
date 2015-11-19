#include "DebugManager.h"
#include "DebugNodeTransform.h"
#include "DebugNodeLine.h"
#include <cassert>

namespace Debugging
{
	DebugManager* DebugManager::s_Instance = nullptr;
	DebugManager& DebugManager::GetInstance()
	{
		if (nullptr == s_Instance)
		{
			s_Instance = new DebugManager;
			assert(nullptr != s_Instance);
		}

		return *s_Instance;
	}

	DebugManager::DebugManager()
	{
		m_FrameBuffer = nullptr;

		m_ObjectToWorldTransform.loadIdentity();
		m_WorldViewProjectionTransform.loadIdentity();

		m_Enabled = false;
	}

	DebugManager::~DebugManager()
	{
	}

	void DebugManager::AddTransform(const matrix4x4& transform)
	{
		m_Nodes.push_back(new DebugNodeTransform(transform));
	}

	void DebugManager::AddLine(const GeometryLib::Line& line, const Color& color)
	{
		auto lineNode = new DebugNodeLine(line, color);
		m_Nodes.push_back(lineNode);
	}

	void DebugManager::Process()
	{
		assert(nullptr != m_FrameBuffer);

		for (auto currentNode : m_Nodes)
		{
			currentNode->Process();
			delete currentNode;
		}

		m_Nodes.clear();
	}

	bool DebugManager::GetEnabled() const
	{
		return m_Enabled;
	}

	void DebugManager::SetEnabled(bool enabled)
	{
		m_Enabled = enabled;
	}

	const matrix4x4& DebugManager::GetObjectToWorldTransform() const
	{
		return m_ObjectToWorldTransform;
	}

	void DebugManager::SetObjectToWorldTransform(const matrix4x4& transform)
	{
		MathLib::matrix4x4_copy(m_ObjectToWorldTransform, transform);
	}
	
	const matrix4x4& DebugManager::GetWorldViewProjectionTransform() const
	{
		return m_WorldViewProjectionTransform;
	}

	void DebugManager::SetWorldViewProjectionTransform(const matrix4x4& transform)
	{
		MathLib::matrix4x4_copy(m_WorldViewProjectionTransform, transform);
	}

	FrameBuffer* DebugManager::GetFrameBuffer() const
	{
		return m_FrameBuffer;
	}

	void DebugManager::SetFrameBuffer(FrameBuffer* frameBuffer)
	{
		m_FrameBuffer = frameBuffer;
	}
}