#include <stdio.h>
#include <gtest\gtest.h>
#include <Camera.h>
#include <MeshManager.h>
#include <MathLib.h>
#include "FrameBuffer.h"
#include "BasicRaytracer.h"
#include "KdTreeGeometry.h"
#include "GeometryInstance.h"
#include "BasicScene.h"
#include "LineDrawing.h"
#include "DebugManager.h"
#include "GeometryCollection.h"

using namespace Assets;
using namespace Raytracer;
using namespace CameraLib;
using namespace Debugging;

GeometryCollection<KdTreeGeometry> g_GeometryCollection;

int DoUnitTests(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	getchar();

	return result;
}

void PrepareScene(IScene& scene)
{
	auto& meshManager = MeshManager::GetInstance();
	meshManager.SetMeshDatabase("test.mdb");

	Basis basis;
	basis.m_Position.setXYZW(0.0f, -2.0f, -12.0f, 1.0f);
	quaternion_setToIdentity(basis.m_Orientation);
	basis.m_Scale.setXYZ(4.0f, 2.0f, 4.0f);

	auto monkeyInstance1 = new GeometryInstance(g_GeometryCollection["monkey"], basis);
	scene.AddTraceable(*monkeyInstance1);

	basis.m_Position.x = -3.0f;
	basis.m_Scale.x = 1.0f;
	basis.m_Scale.y = 1.0f;
	basis.m_Scale.z = 1.0f;

	auto monkeyInstance2 = new GeometryInstance(g_GeometryCollection["monkey"], basis);
	scene.AddTraceable(*monkeyInstance2);
}

int DoScene(int argc, char** argv)
{
	auto frameBuffer = new FrameBuffer(1280, 720);

	auto camera = new Camera;
	camera->SetPosition(-5.0f, 7.0f, 0.0f);
	camera->SetCameraYFov(60.0f);
	camera->ResetCameraOrientation();
	camera->RotateXAxis(-30.0f);
	camera->RotateYAxis(-30.0f);
	camera->SetNearClipPlaneDistance(0.01f);
	camera->Update();
	
	// Initialize the debug manager.
	auto& debugManager = DebugManager::GetInstance();
	{
		debugManager.SetFrameBuffer(frameBuffer);

		matrix4x4 worldViewProjectionTransform;
		MathLib::matrix4x4_mul(camera->GetPerspectiveProjectionMatrix(frameBuffer->GetWidth(), frameBuffer->GetHeight()),
							   camera->GetWorldSpaceToCameraSpaceTransform(),
							   worldViewProjectionTransform);
		debugManager.SetWorldViewProjectionTransform(worldViewProjectionTransform);

		matrix4x4 worldSpaceIdentity;
		worldSpaceIdentity.loadIdentity();

		debugManager.AddTransform(worldSpaceIdentity);
	}

	BasicScene scene;

	auto raytracer = new BasicRaytracer(frameBuffer, camera, &scene);
	PrepareScene(scene);
	raytracer->Raytrace();
	debugManager.Process();
	frameBuffer->SaveAsTga("kdTree.tga");
	delete frameBuffer;

	return 0;
}

int main(int argc, char** argv)
{
	bool doTests = false;

	if (doTests)
		return DoUnitTests(argc, argv);
	else
		return DoScene(argc, argv);
}
