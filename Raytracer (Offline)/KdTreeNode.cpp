#include "KdTreeNode.h"

namespace Raytracer
{
	KdTreeNode::KdTreeNode()
	{
		m_NumTriangles = 0;
		m_TriangleList = nullptr;

		m_Children[0] = nullptr;
		m_Children[1] = nullptr;
	}

	KdTreeNode::~KdTreeNode()
	{
		if (nullptr != m_Children[0])
			delete m_Children[0];

		if (nullptr != m_Children[1])
			delete m_Children[1];

		if (nullptr != m_TriangleList)
			delete[] m_TriangleList;
	}

	const vector4& KdTreeNode::GetBoundingMin() const
	{
		return m_BoundingMin;
	}

	const vector4& KdTreeNode::GetBoundingMax() const
	{
		return m_BoundingMax;
	}

	const plane& KdTreeNode::GetSplittingPlane() const
	{
		return m_SplittingPlane;
	}

	KdTreeNode* const * KdTreeNode::GetChildren() const
	{
		return m_Children;
	}

	bool KdTreeNode::IsChild() const
	{
		return (nullptr == m_Children[0]) && (nullptr == m_Children[1]);
	}

	unsigned int KdTreeNode::GetNumTriangles() const
	{
		return m_NumTriangles;
	}

	const unsigned int* KdTreeNode::GetTriangleList() const
	{
		return m_TriangleList;
	}
}