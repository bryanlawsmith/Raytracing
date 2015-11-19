#include <stdio.h>
#include <gtest\gtest.h>
#include <Camera.h>
#include <MeshManager.h>
#include <MathLib.h>
#include "FrameBuffer.h"
#include "RaytracerNaive.h"
#include "RaytracerKdTree.h"
#include "IScene.h"
#include "LineDrawing.h"
#include "DebugManager.h"

using namespace Assets;
using namespace Raytracer;
using namespace CameraLib;
using namespace Debugging;

int DoUnitTests(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	getchar();

	return result;
}

void PrepareScene(IScene& scene)
{
	Basis basis;
	basis.m_Position.setXYZW(0.0f, -2.0f, -12.0f, 1.0f);
	quaternion_setToIdentity(basis.m_Orientation);
	basis.m_Scale.setXYZ(4.0f, 4.0f, 4.0f);
	scene.AddStaticMeshInstance("monkey", basis);
}

int DoScene(int argc, char** argv)
{
	auto& meshManager = MeshManager::GetInstance();
	meshManager.SetMeshDatabase("test.mdb");

	auto frameBuffer = new FrameBuffer(256, 256);

	auto camera = new Camera;
	camera->SetPosition(-5.0f, 7.0f, 0.0f);
	camera->SetCameraYFov(60.0f);
	camera->ResetCameraOrientation();
	camera->RotateXAxis(-30.0f);
	camera->RotateYAxis(-30.0f);
	camera->SetNearClipPlaneDistance(0.01f);
	camera->Update();

	auto raytracer = new RaytracerNaive();
	

	//auto raytracer = new RaytracerNaive;
	//raytracer->SetCamera(camera);
	//raytracer->SetFrameBuffer(frameBuffer);
	//PrepareScene(raytracer->GetScene());
	//raytracer->Raytrace();
	//frameBuffer->SaveAsTga("naive.tga");
	//delete frameBuffer;

	auto frameBufferKdTree = new FrameBuffer(1280, 720);

	// Initialize the debug manager.
	auto& debugManager = DebugManager::GetInstance();
	{
		debugManager.SetFrameBuffer(frameBufferKdTree);

		matrix4x4 worldViewProjectionTransform;
		MathLib::matrix4x4_mul(camera->GetPerspectiveProjectionMatrix(frameBufferKdTree->GetWidth(), frameBufferKdTree->GetHeight()),
							   camera->GetWorldSpaceToCameraSpaceTransform(),
							   worldViewProjectionTransform);
		debugManager.SetWorldViewProjectionTransform(worldViewProjectionTransform);

		matrix4x4 worldSpaceIdentity;
		worldSpaceIdentity.loadIdentity();

		debugManager.AddTransform(worldSpaceIdentity);
	}

	auto kdTreeRaytracer = new RaytracerKdTree;
	kdTreeRaytracer->SetCamera(camera);
	kdTreeRaytracer->SetFrameBuffer(frameBufferKdTree);
	PrepareScene(kdTreeRaytracer->GetScene());
	kdTreeRaytracer->Raytrace();
	debugManager.Process();
	frameBufferKdTree->SaveAsTga("kdTree.tga");
	delete frameBufferKdTree;

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
