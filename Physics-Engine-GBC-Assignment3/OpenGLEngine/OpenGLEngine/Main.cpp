//#define STB_IMAGE_IMPLEMENTATION
#include "RenderingSystem.h"
#include "RenderingSystemV2.h"
#include "InputEventSystem.h"
#include "RotateSystem.h"
#include "ParticleSystem.h"
#include "ParticleSpawnerSystem.h"
#include "GravityForceGeneratorSystem.h"
#include "FixedSpringForceGeneratorSystem.h"
#include "ForceAccumulatorSystem.h"
#include "PairedSpringForceGeneratorSystem.h"
#include "SphereContactGeneratorSystem.h"
#include "ParticleContactResolutionSystem.h"
#include "CableComponentSystem.h"
#include "RodSystem.h"
#include "ForceAndTorqueAccumulatorSystem.h"
#include "RigidBodySystem.h"
#include "MoveInBoundsSystem.h"
#include "FPSControlSystem.h"
#include "DynamicDirectionalLightSystem.h"
#include "DynamicPointLightSystem.h"
#include "DynamicSpotLightSystem.h"
#include "FlightSimulatorSystem.h"
#include "FollowCameraSystem.h"
#include "InfiniteSpawnSystem.h"
#include "InfiniteSpawnTargetSystem.h"
#include "AeroControlSystem.h"
#include "SetAerodynamicTensorSystem.h"
#include "AeroSystem.h"
#include "CameraLookSystem.h"
#include "LifeTimeSystem.h"
#include "Buoyancy.h"
#include "BuoyancySystem.h"
#include "BoatSystem.h"
#include "BoatComponent.h"

#include <string>
#include <stdlib.h>     
#include <time.h>      

#define DEBUG_LOG_LEVEL 3

using namespace Reality;

void LoadShaders(ECSWorld& world);
void LoadModels(ECSWorld& world);
void MakeABunchaObjects(ECSWorld& world);
void MakeABunchaSprings(ECSWorld& world);
void MakeABunchaSpheres(ECSWorld& world);
void MakeACable(ECSWorld& world);
void MakeCablesAndRods(ECSWorld& world);
void MakeFlight(ECSWorld& world);
void TestContacts(ECSWorld& world);
void TestCollision(ECSWorld& world);
void SetupLights(ECSWorld& world);

int main()
{
	ECSWorld world;

	world.data.InitRendering();

	world.data.renderUtil->camera.Position = Vector3(0, 15.0f, 100.0f);
	world.data.renderUtil->SetFOV(60);

	auto e = world.createEntity();
	e.addComponent<FPSControlComponent>();

	SetupLights(world);
	MakeFlight(world);

	world.getSystemManager().addSystem<RenderingSystem>();
	world.getSystemManager().addSystem<RenderingSystemV2>();
	world.getSystemManager().addSystem<InputEventSystem>();
	world.getSystemManager().addSystem<RotateSystem>();
	world.getSystemManager().addSystem<ParticleSystem>();
	world.getSystemManager().addSystem<ParticleSpawnerSystem>();
	world.getSystemManager().addSystem<GravityForceGeneratorSystem>();
	world.getSystemManager().addSystem<FixedSpringForceGeneratorSystem>();
	world.getSystemManager().addSystem<PairedSpringForceGeneratorSystem>();
	world.getSystemManager().addSystem<SphereContactGeneratorSystem>();
	world.getSystemManager().addSystem<CableComponentSystem>();
	world.getSystemManager().addSystem<RodSystem>();
	world.getSystemManager().addSystem<ParticleContactResolutionSystem>();
	world.getSystemManager().addSystem<ForceAccumulatorSystem>();
	world.getSystemManager().addSystem<ForceAndTorqueAccumulatorSystem>();
	world.getSystemManager().addSystem<FlightSimulatorSystem>();
	world.getSystemManager().addSystem<FPSControlSystem>();
	world.getSystemManager().addSystem<FollowCameraSystem>();
	world.getSystemManager().addSystem<CameraLookSystem>();
	world.getSystemManager().addSystem<InfiniteSpawnSystem>();
	world.getSystemManager().addSystem<InfiniteSpawnTargetSystem>();
	world.getSystemManager().addSystem<AeroControlSystem>();
	world.getSystemManager().addSystem<SetAerodynamicTensorSystem>();
	world.getSystemManager().addSystem<AeroSystem>();
	world.getSystemManager().addSystem<LifeTimeSystem>();
	world.getSystemManager().addSystem<MoveInBoundsSystem>();
	world.getSystemManager().addSystem<DynamicDirectionalLightSystem>();
	world.getSystemManager().addSystem<DynamicPointLightSystem>();
	world.getSystemManager().addSystem<DynamicSpotLightSystem>();
	world.getSystemManager().addSystem<BuoyancySystem>();
	world.getSystemManager().addSystem<BoatSystem>();

	rp3d::CollisionWorld rp3dWorld;
	world.getSystemManager().addSystem<RigidBodySystem>(rp3dWorld);
	world.getSystemManager().addSystem<RigidbodyGravityForceGeneratorSystem>();

	float time = glfwGetTime();
	float stepTime = glfwGetTime();
	float deltaTime = 0;
	float elapsedDeltaTime = 0;
	float logicDelta = 0;
	float debugDelta = 0;

	LoadShaders(world);
	bool shadersLoaded = false;
	bool modelsLoadStarted = false;

	while (!glfwWindowShouldClose(world.data.renderUtil->window->glfwWindow))
	{
		float current = glfwGetTime();
		deltaTime = current - time;
		deltaTime = 1 / 60.0f;
		time = glfwGetTime();

		world.update();

		glfwPollEvents();

		world.data.renderUtil->ClearDisplay(world.data.renderUtil->window->glfwWindow);

		if (!shadersLoaded)
		{
			shadersLoaded = world.data.assetLoader->ShadersLoaded();
		}
		if(shadersLoaded && !modelsLoadStarted)
		{
			LoadModels(world);
			modelsLoadStarted = true;
		}

		float fixedDeltaTime = 1 / 60.0f;
		world.data.renderUtil->UpdateViewMatrix();
		world.getSystemManager().getSystem<InputEventSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<FPSControlSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RotateSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<ParticleSpawnerSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<MoveInBoundsSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<FlightSimulatorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<FollowCameraSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<CameraLookSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<InfiniteSpawnTargetSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<InfiniteSpawnSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<AeroControlSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<SetAerodynamicTensorSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<LifeTimeSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<BoatSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<BuoyancySystem>().Update(deltaTime);
		world.getSystemManager().getSystem<AeroSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<GravityForceGeneratorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<FixedSpringForceGeneratorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<PairedSpringForceGeneratorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ForceAccumulatorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ParticleSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<RigidbodyGravityForceGeneratorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ForceAndTorqueAccumulatorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<RigidBodySystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<SphereContactGeneratorSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<CableComponentSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<RodSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ParticleContactResolutionSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<DynamicDirectionalLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicPointLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicSpotLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RenderingSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RenderingSystemV2>().Update(deltaTime);

		elapsedDeltaTime = glfwGetTime() - time;
		logicDelta = elapsedDeltaTime - world.data.renderUtil->GetRenderDelta();
		stepTime = glfwGetTime();

		if (DEBUG_LOG_LEVEL > 0)
		{
			world.data.renderUtil->RenderText("FPS : " + std::to_string((int)round(1.0f / deltaTime)), 1810.0f, 1060.0f, 0.5f, Color(0, 1, 1, 1));
		}
		if (DEBUG_LOG_LEVEL > 1)
		{
			int logic = (int)round(logicDelta * 100.0f / deltaTime);
			std::string logicString = logic < 10 ? " " + std::to_string(logic) : std::to_string(logic);
			int render = (int)round(world.data.renderUtil->GetRenderDelta() * 100.0f / deltaTime);
			std::string renderString = logic < 10 ? " " + std::to_string(render) : std::to_string(render);
			int debug = (int)round(debugDelta * 100.0f / deltaTime);
			std::string debugString = logic < 10 ? " " + std::to_string(debug) : std::to_string(debug);
			
			world.data.renderUtil->RenderText("Logic : " + logicString + "%" +
				+ " | Rendering : " + renderString + "%" +
				+ " | Debug : " + debugString + "%"
				, 1680.0f, 1040.0f, 0.25f, Color(0, 1, 1, 1));
		}
		if (DEBUG_LOG_LEVEL > 2)
		{
			world.data.renderUtil->RenderText("Draw Calls : " + std::to_string(world.data.renderUtil->GetDrawCalls())
				+ " | Verts : " + std::to_string(world.data.renderUtil->GetVerts())
				+ " | Tris : " + std::to_string(world.data.renderUtil->GetTris())
				+ " | Lines : " + std::to_string(world.data.renderUtil->GetLines())
				, 1610.0f, 1020.0f, 0.25f, Color(0, 1, 1, 1));
		}

		debugDelta = glfwGetTime() - stepTime;
		stepTime = glfwGetTime();

		world.data.renderUtil->SwapBuffers(world.data.renderUtil->window->glfwWindow);

	}

	glfwTerminate();
	return 0;
}

void LoadShaders(ECSWorld& world)
{
	world.data.assetLoader->StartShaderLoading({ {"Shaders/Lighting_Maps.vs", "Shaders/Lighting_Maps.fs"} });
}
void LoadModels(ECSWorld& world)
{
	world.data.assetLoader->StartModelLoading({
		ModelData(
			"Resources/Models/supermarine-spitfire/spitfire.fbx",
			{{"spitfire_d.png"}}
		)
	});
}

void MakeABunchaObjects(ECSWorld& world)
{
	auto e = world.createEntity();
	e.addComponent<TransformComponent>(Vector3(4, 10.0f, 48), Vector3(0.10f, 0.1f, 0.1f), Vector3(-90, 180, 0));
	e.addComponent<ModelComponent>("Resources/Models/supermarine-spitfire/spitfire.fbx");
	e.addComponent<RotateComponent>(0, 40, 0);

	e = world.createEntity();
	e.addComponent<TransformComponent>(Vector3(4, 10.0f, -62), Vector3(0.1f, 0.1f, 0.1f), Vector3(-90, 0, 0));
	e.addComponent<ModelComponent>("Resources/Models/supermarine-spitfire/spitfire.fbx");
	e.addComponent<RotateComponent>(0, 40, 0);
}

void MakeABunchaSprings(ECSWorld& world)
{
	auto e = world.createEntity();
	float yOffset = 30;
	e.addComponent<TransformComponent>(Vector3(-2.5f, -5 + yOffset, -3), Vector3(1.0f, 1.0f, 1.0f));
	e.addComponent<ParticleComponent>();
	e.addComponent<ModelComponent>("Resources/Models/nanosuit/nanosuit.obj");

	auto springEntinty = world.createEntity();
	springEntinty.addComponent<TransformComponent>(Vector3(-2.5f, 0 + yOffset, 3));
	springEntinty.addComponent<FixedSpringComponent>(8, 2, e);

	auto e2 = world.createEntity();
	e2.addComponent<TransformComponent>(Vector3(2.5f, -5 + yOffset, -1), Vector3(1.0f, 1.0f, 1.0f));
	e2.addComponent<ParticleComponent>();
	e2.addComponent<ModelComponent>("Resources/Models/nanosuit/nanosuit.obj");

	auto springEntinty2 = world.createEntity();
	springEntinty2.addComponent<TransformComponent>(Vector3(2.5f, 0 + yOffset, 1));
	springEntinty2.addComponent<FixedSpringComponent>(5, 5, e2);

	auto pairedSpring = world.createEntity();
	pairedSpring.addComponent<PairedSpringComponent>(100, 5.0f, e, e2);

	auto e3 = world.createEntity();
	e3.addComponent<TransformComponent>(Vector3(-7.5f, -7.5f + yOffset, 1), Vector3(1.0f, 1.0f, 1.0f));
	e3.addComponent<ParticleComponent>();
	e3.addComponent<ModelComponent>("Resources/Models/nanosuit/nanosuit.obj");

	auto springEntinty3 = world.createEntity();
	springEntinty3.addComponent<TransformComponent>(Vector3(-7.5f, -10 + yOffset, -1));
	springEntinty3.addComponent<FixedSpringComponent>(7, 7, e3);

	auto e4 = world.createEntity();
	e4.addComponent<TransformComponent>(Vector3(7.5f, -7.5f + yOffset, 3), Vector3(1.0f, 1.0f, 1.0f));
	e4.addComponent<ParticleComponent>();
	e4.addComponent<ModelComponent>("Resources/Models/nanosuit/nanosuit.obj");

	auto springEntinty4 = world.createEntity();
	springEntinty4.addComponent<TransformComponent>(Vector3(7.5f, -10 + yOffset, -3));
	springEntinty4.addComponent<FixedSpringComponent>(5, 0, e4);

	auto pairedSpring2 = world.createEntity();
	pairedSpring2.addComponent<PairedSpringComponent>(100, 5.2f, e, e3);

	auto pairedSpring3 = world.createEntity();
	pairedSpring3.addComponent<PairedSpringComponent>(100, 5.2f, e2, e4);

	auto pairedSpring4 = world.createEntity();
	pairedSpring4.addComponent<PairedSpringComponent>(100, 10.0f, e3, e4);
}

void MakeABunchaSpheres(ECSWorld& world)
{
	for (int i = 0; i < 30; i++)
	{
		auto e = world.createEntity();

		e.addComponent<TransformComponent>(Vector3(RANDOM_FLOAT(-15.0f, 15.0f), RANDOM_FLOAT(6.0f, 34.0f), RANDOM_FLOAT(-15.0f, 15.0f)));
		e.addComponent<ParticleComponent>(1, Vector3(RANDOM_FLOAT(-5, 5), RANDOM_FLOAT(-5, 5), RANDOM_FLOAT(-5, 5)));
		e.addComponent<SphereComponent>(1);
		Color col = Color(0, RANDOM_FLOAT(0.0f, 1.0f), RANDOM_FLOAT(0.0f, 1.0f));
	}

	auto ref = world.createEntity();
	ref.addComponent<TransformComponent>(Vector3(0, 20, 0), Vector3(0.3f, 0.3f, 0.3f), Vector3(0, 180, 0));
	ref.addComponent<ModelComponent>("Resources/Models/nanosuit/nanosuit.obj");
	ref.addComponent<RotateComponent>(0, 40, 0);
}

void MakeACable(ECSWorld& world)
{
	auto e1 = world.createEntity();
	e1.addComponent<TransformComponent>(Vector3(0, 40, 0));

	auto e2 = world.createEntity();
	e2.addComponent<TransformComponent>(Vector3(0, 30, 0));
	e2.addComponent<ParticleComponent>(1);
	
	auto e = world.createEntity();
	e.addComponent<CableComponent>(e1, e2, 20);
}

void MakeCablesAndRods(ECSWorld& world)
{
	auto eFixed = world.createEntity();
	eFixed.addComponent<TransformComponent>(Vector3(10, 40, 0));

	auto eFixed2 = world.createEntity();
	eFixed2.addComponent<TransformComponent>(Vector3(20, 10, 0));

	auto eFixed3 = world.createEntity();
	eFixed3.addComponent<TransformComponent>(Vector3(-20, 10, 0));

	auto e1 = world.createEntity();
	e1.addComponent<TransformComponent>(Vector3(0, 30, 0));
	e1.addComponent<ParticleComponent>(10);

	auto e2 = world.createEntity();
	e2.addComponent<TransformComponent>(Vector3(-10, 20, 0));
	e2.addComponent<ParticleComponent>(10);

	auto e3 = world.createEntity();
	e3.addComponent<TransformComponent>(Vector3(0, 10, 0));
	e3.addComponent<ParticleComponent>(10);

	auto e4 = world.createEntity();
	e4.addComponent<TransformComponent>(Vector3(10, 20, 0));
	e4.addComponent<ParticleComponent>(10);

	auto eCable = world.createEntity();
	eCable.addComponent<CableComponent>(eFixed, e1, 20);

	auto eCable2 = world.createEntity();
	eCable2.addComponent<PairedSpringComponent>(1000, 20, eFixed2, e4);

	auto eCable3 = world.createEntity();
	eCable3.addComponent<PairedSpringComponent>(1000, 20, eFixed3, e2);

	auto eRod1 = world.createEntity();
	eRod1.addComponent<RodComponent>(e1, e2, 10 * sqrt(2));
	auto eRod2 = world.createEntity();
	eRod2.addComponent<RodComponent>(e2, e3, 10 * sqrt(2));
	auto eRod3 = world.createEntity();
	eRod3.addComponent<RodComponent>(e3, e4, 10 * sqrt(2));
	auto eRod4 = world.createEntity();
	eRod4.addComponent<RodComponent>(e4, e1, 10 * sqrt(2));

	auto eRodDiagonal1 = world.createEntity();
	eRodDiagonal1.addComponent<RodComponent>(e1, e3, 20);
	auto eRodDiagonal2 = world.createEntity();
	eRodDiagonal2.addComponent<RodComponent>(e2, e4, 20);
}

void MakeFlight(ECSWorld& world)
{
	auto e = world.createEntity();
	e.addComponent<TransformComponentV2>(Vector3(0.0f, 480.0f, 0.0f), Vector3(0.10f, 0.1f, 0.1f));
	e.addComponent<ModelComponent>("Resources/Models/supermarine-spitfire/spitfire.fbx", Vector3(0, -50, 20), Vector3(-90, 0, 0));
	e.addComponent<RigidBodyComponent>(25.0f ,0.5f, 0.6f);
	e.addComponent<FollowCameraComponent>(Vector3(0.0f, 15.0f, 40.0f));
	e.addComponent<CameraLookComponent>();
	e.addComponent<InfiniteSpawnTargetComponent>();
	e.addComponent<BoatComponent>();
	e.addComponent<Buoyancy>();


	for (int i = -40; i <= 40; i++)
	{
		auto WaterSurface = world.createEntity();
		WaterSurface.addComponent<TransformComponentV2>(Vector3(0.0f,440.0f, 50.0f * i));
		WaterSurface.addComponent<InfiniteSpawnComponent>(RANDOM_FLOAT(1.0f, 0.1f));

		auto WaterSurface2 = world.createEntity();
		WaterSurface2.addComponent<TransformComponentV2>(Vector3(50.0f, 440.0f, 50.0f * i));
		WaterSurface2.addComponent<InfiniteSpawnComponent>(RANDOM_FLOAT(1.0f, 0.1f));

		auto WaterSurface3 = world.createEntity();
		WaterSurface3.addComponent<TransformComponentV2>(Vector3(-50.0f, 440.0f, 50.0f * i));
		WaterSurface3.addComponent<InfiniteSpawnComponent>(RANDOM_FLOAT(1.0f, 0.1f));
	}
}

void TestContacts(ECSWorld& world)
{
	for (int i = 0; i < 30; i++)
	{
		auto e = world.createEntity();
		e.addComponent<TransformComponentV2>(Vector3(RANDOM_FLOAT(-200.0f, 200.0f), RANDOM_FLOAT(-200.0f, 200.0f), RANDOM_FLOAT(-200.0f, 200.0f)),
			Vector3(1, 1, 1),
			Vector3(RANDOM_FLOAT(-180.0f, 180.0f), RANDOM_FLOAT(-180.0f, 180.0f), RANDOM_FLOAT(-180.0f, 180.0f)));
		e.addComponent<RigidBodyComponent>();
		e.addComponent<MoveInBoundsComponent>(Vector3(RANDOM_FLOAT(-10.0f, 10.0f), RANDOM_FLOAT(-10.0f, 10.0f), RANDOM_FLOAT(-10.0f, 10.0f)),
			Vector3(200, 200, 200));
		auto col = world.createEntity();
		
	}
}

void TestCollision(ECSWorld& world)
{
	auto floor1 = world.createEntity();
	floor1.addComponent<TransformComponentV2>(Vector3(0, -50, 0), Vector3(1, 1, 1), Vector3(0, 0, 0));
	floor1.addComponent<RigidBodyComponent>(100000.0f, 0.4f, 0.3f, Vector3(0, 0, 0), Vector3(0, 0, 0), 0);
	auto floorCol1 = world.createEntity();

	{
		auto object2 = world.createEntity();
		object2.addComponent<TransformComponentV2>(Vector3(RANDOM_FLOAT(-50.0f, 50.0f), 50, RANDOM_FLOAT(-50.0f, 50.0f)), Vector3(1, 1, 1), Vector3(RANDOM_FLOAT(0, 180), RANDOM_FLOAT(0, 180), RANDOM_FLOAT(0, 180)));
		object2.addComponent<RigidBodyComponent>(10.0f, 0.1f, 0.1f, Vector3(0, 0, 0), Vector3(0, 0, 0), 5);
		auto objectCol2 = world.createEntity();
	}
}

void SetupLights(ECSWorld& world)
{
	auto l = world.createEntity();
	l.addComponent<TransformComponent>(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(90, 0, 0));
	l.addComponent<DynamicDirectionalLightComponent>(Color(0.00, 0.0, 0), Color::White, Color::Orange);

	auto pl1 = world.createEntity();
	pl1.addComponent<TransformComponent>(Vector3(25, 14, 48.5f));
	pl1.addComponent<DynamicPointLightComponent>(100.0f, Color(0.1, 0, 0), Color(1.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f));
	pl1.addComponent<ParticleComponent>();

	auto hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(23, 15, 48.0f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl1);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22, 13.5f, 50.5f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl1);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(21, 12.5f, 47.5f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl1);

	auto pl2 = world.createEntity();
	pl2.addComponent<TransformComponent>(Vector3(-14.5f, 14, 49.0f));
	pl2.addComponent<DynamicPointLightComponent>(100.0f, Color(0, 0, 0.1f), Color(0.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 1.0f));
	pl2.addComponent<ParticleComponent>();
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f + 1, 14 - 1, 49.0f - 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl2);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 0.5f, 14 + 1, 49.0f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl2);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f, 14 - 1, 49.0f + 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl2);
	
	auto pl3 = world.createEntity();
	pl3.addComponent<TransformComponent>(Vector3(22, 14, -62.0f));
	pl3.addComponent<DynamicPointLightComponent>(100.0f, Color(0, 0.1f, 0), Color(0.0f, 1.0f, 0.0f), Color(0.0f, 1.0f, 0.0f));
	pl3.addComponent<ParticleComponent>();
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22 - 1, 14 - 1, -62.0f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl3);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22, 14 + 0.5f, -62.0f - 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl3);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22 + 1, 14, -62.0f + 0.5f));
	hook.addComponent<FixedSpringComponent>(5, 1, pl3);

	auto pl4 = world.createEntity();
	pl4.addComponent<TransformComponent>(Vector3(-14.5f, 14, -61.5f));
	pl4.addComponent<DynamicPointLightComponent>(100.0f, Color(0.1, 0.05, 0), Color(1.0f, 0.55f, 0.0f), Color(1.0f, 0.55f, 0.0f));
	pl4.addComponent<ParticleComponent>();
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 1, 14, -61.5f -1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl4);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 0.25f, 14 - 0.5f, -61.5f + 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl4);
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f + 0.5f, 14+ 1, -61.5f + 1));
	hook.addComponent<FixedSpringComponent>(5, 1, pl4);

	std::vector<Color> cols = { Color(1,0,0), Color(0,1,0), Color(0,0,1), Color(0.7f,0.55f,0) };
	for (int i = 1; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pl1 = world.createEntity();
			pl1.addComponent<TransformComponent>(Vector3((i % 2 == 0 ? 8 : -1), 85, 49.5f - 37 * j), Vector3(1, 1, 1), Vector3(180, 0, 0));
			pl1.addComponent<DynamicSpotLightComponent>(10.0f, 100, Color(0, 0, 0), cols[3 - j], cols[3 - j], 5);
			pl1.addComponent<RotateComponent>((i % 2 == 0 ? 1 : -1) * 100,100,100);
		}
	}
}