#include "CameraControl.h"

namespace Raytracer
{
	CameraControl::CameraControl(CameraLib::Camera& camera) :
		m_CameraHandle(camera)
	{
		m_MoveForward = false;
		m_MoveBackward = false;
		m_MoveLeft = false;
		m_MoveRight = false;
		m_MoveUpward = false;
		m_MoveDownward = false;

		m_LookLeft = false;
		m_LookRight = false;
		m_LookUp = false;
		m_LookDown = false;
	}

	void CameraControl::Update(float timeElapsed)
	{
		const float cameraVelocity = 10.0f;
		const float cameraTurningVelocity = 100.0f;

		if (m_MoveForward) m_CameraHandle.MoveZAxis(-cameraVelocity * timeElapsed);
		if (m_MoveBackward) m_CameraHandle.MoveZAxis(cameraVelocity * timeElapsed);
		if (m_MoveLeft) m_CameraHandle.MoveXAxis(-cameraVelocity * timeElapsed);
		if (m_MoveRight) m_CameraHandle.MoveXAxis(cameraVelocity * timeElapsed);
		if (m_MoveUpward) m_CameraHandle.MoveYAxis(cameraVelocity * timeElapsed);
		if (m_MoveDownward) m_CameraHandle.MoveYAxis(-cameraVelocity * timeElapsed);

		if (m_LookLeft) m_CameraHandle.RotateYAxis(cameraTurningVelocity * timeElapsed);
		if (m_LookRight) m_CameraHandle.RotateYAxis(-cameraTurningVelocity * timeElapsed);
		if (m_LookUp) m_CameraHandle.RotateXAxis(cameraTurningVelocity * timeElapsed);
		if (m_LookDown) m_CameraHandle.RotateXAxis(-cameraTurningVelocity * timeElapsed);

		m_CameraHandle.Update();
	}

	void CameraControl::SetMoveForward(bool value)
	{
		m_MoveForward = value;
	}

	void CameraControl::SetMoveBackward(bool value)
	{
		m_MoveBackward = value;
	}

	void CameraControl::SetMoveLeft(bool value)
	{
		m_MoveLeft = value;
	}

	void CameraControl::SetMoveRight(bool value)
	{
		m_MoveRight = value;
	}

	void CameraControl::SetMoveUpward(bool value)
	{
		m_MoveUpward = value;
	}

	void CameraControl::SetMoveDownward(bool value)
	{
		m_MoveDownward = value;
	}

	void CameraControl::SetLookLeft(bool value)
	{
		m_LookLeft = value;
	}

	void CameraControl::SetLookRight(bool value)
	{
		m_LookRight = value;
	}

	void CameraControl::SetLookUp(bool value)
	{
		m_LookUp = value;
	}

	void CameraControl::SetLookDown(bool value)
	{
		m_LookDown = value;
	}
}