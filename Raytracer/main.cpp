#include "Raytracer.h"
#include "CameraControl.h"
#include "StaticMeshInstance.h"
#include <MeshManager.h>
#include <iostream>
#include <cassert>
#include <SDL.h>
#include <GL\glew.h>
#include <GL\gl.h>

static bool quit = false;

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int main(int argc, char *argv[])
{
	using namespace std;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Unable to initialize SDL!" << endl;
		return 1;
	}
	
	auto window = SDL_CreateWindow("Ray tracer",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									1024,
									600,
									SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (nullptr == window)
	{
		cout << "SDL Error: Unable to create main window!" << endl;
		return 1;
	}

	// NOTE: Creeate your window, set your attributes, and then create the context!
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	auto context = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, context);

	glewExperimental = GL_TRUE;
	auto result = glewInit();
	assert(GLEW_OK == result);

	const unsigned char *version = glGetString(GL_VERSION);
	if (version == NULL)
	{
		printf("There was an error creating the OpenGL context!\n");
		return 1;
	}

	auto swapResult = SDL_GL_SetSwapInterval(1);

	SDL_Event event;

	CameraLib::Camera camera;
	camera.ResetCameraOrientation();
	camera.SetPosition(0.0f, 0.0f, 50.0f);
	camera.SetCameraYFov(60.0f);
	
	Raytracer::CameraControl cameraControl(camera);

	Raytracer::Raytracer raytracer;
	raytracer.ResizeViewport(1024, 600);
	raytracer.SetCamera(&camera);

	MathLib::quaternion identityQuaternion;
	MathLib::quaternion_setToIdentity(identityQuaternion);

	Assets::MeshManager::GetInstance().SetMeshDatabase("test.mdb");

	// Prepare scene geometry.
	raytracer.ClearSceneData();
	Raytracer::StaticMeshInstance meshInstance("monkey",
											   MathLib::vector4(0.0f, 0.0f, 0.0f, 1.0f),
											   identityQuaternion,
											   5.0f);
	raytracer.AppendStaticMeshInstanceToSceneData(meshInstance);
	raytracer.CreateNewSceneDataSSBO();

	float timeElapsed = 0.0f;
	float timerFrequency = (float)SDL_GetPerformanceFrequency();

	while (!quit)
	{
		auto startTime = SDL_GetPerformanceCounter();

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_w: cameraControl.SetMoveForward(true); break;
					case SDLK_s: cameraControl.SetMoveBackward(true); break;
					case SDLK_a: cameraControl.SetMoveLeft(true); break;
					case SDLK_d: cameraControl.SetMoveRight(true); break;
					case SDLK_SPACE: cameraControl.SetMoveUpward(true); break;
					case SDLK_LCTRL: cameraControl.SetMoveDownward(true); break;
					case SDLK_RIGHT: cameraControl.SetLookRight(true); break;
					case SDLK_LEFT: cameraControl.SetLookLeft(true); break;
					case SDLK_UP: cameraControl.SetLookUp(true); break;
					case SDLK_DOWN: cameraControl.SetLookDown(true); break;
					case SDLK_ESCAPE: quit = true; break;
				}
			}
			if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_w: cameraControl.SetMoveForward(false); break;
					case SDLK_s: cameraControl.SetMoveBackward(false); break;
					case SDLK_a: cameraControl.SetMoveLeft(false); break;
					case SDLK_d: cameraControl.SetMoveRight(false); break;
					case SDLK_SPACE: cameraControl.SetMoveUpward(false); break;
					case SDLK_LCTRL: cameraControl.SetMoveDownward(false); break;
					case SDLK_RIGHT: cameraControl.SetLookRight(false); break;
					case SDLK_LEFT: cameraControl.SetLookLeft(false); break;
					case SDLK_UP: cameraControl.SetLookUp(false); break;
					case SDLK_DOWN: cameraControl.SetLookDown(false); break;
				}
			}
		}

		cameraControl.Update(timeElapsed);
		raytracer.Render();

		SDL_GL_SwapWindow(window);

		timeElapsed = (float)(SDL_GetPerformanceCounter() - startTime) / timerFrequency;
	}

	SDL_DestroyWindow(window); 
	SDL_Quit(); 

	return 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus
