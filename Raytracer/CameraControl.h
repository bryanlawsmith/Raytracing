#pragma once

#include <Camera.h>

namespace Raytracer
{
	class CameraControl
	{
	public:

		CameraControl(CameraLib::Camera& cameraHandle);

		void SetMoveForward(bool value);
		void SetMoveBackward(bool value);
		void SetMoveLeft(bool value);
		void SetMoveRight(bool value);
		void SetMoveUpward(bool value);
		void SetMoveDownward(bool value);
		
		void SetLookLeft(bool value);
		void SetLookRight(bool value);
		void SetLookUp(bool value);
		void SetLookDown(bool value);

		void Update(float timeElapsed);

	protected:

		CameraLib::Camera& m_CameraHandle;

		bool m_MoveForward;
		bool m_MoveBackward;
		bool m_MoveLeft;
		bool m_MoveRight;
		bool m_MoveUpward;
		bool m_MoveDownward;

		bool m_LookLeft;
		bool m_LookRight;
		bool m_LookUp;
		bool m_LookDown;
	};
}