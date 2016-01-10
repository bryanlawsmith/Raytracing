#pragma once

#include "IScene.h"
#include <vector>

namespace Raytracer
{
	/// <summary>
	/// Basic implementation of the IScene interface which does not partition scene elements 
	/// using some form of spatial subdivision.
	/// </summary>
	class BasicScene : public IScene
	{
	public:

		BasicScene();

		bool Trace(const ray& intersectionRay, float* t, float* results) const override;

		void AddTraceable(ITraceable& traceable) override;

		void Clear() override;

	protected:

		std::vector<ITraceable*> m_Elements;
	};
}