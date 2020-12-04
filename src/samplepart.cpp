#include <fstream>
#include <sstream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <vector>

#include "samplepart.h"
#include "SDL_keycode.h"
#include "input/inputevent.h"
#include "core/entitycomponentsystem.h"
#include "assets/gltfimporter.h"

#include "core/components/lightingcomponent.h"


using namespace Boiler;

SamplePart::SamplePart(Engine &engine, const std::string &modelPath) : Part("Sample", engine), logger("Sample Part")
{
	this->modelPath = modelPath;
	moveLeft = false;
	moveRight = false;
	moveCloser = false;
	moveFurther = false;
	moveUp = false;
	moveDown = false;

	camPosition = {0, 10, 10};
	camDirection = {0, 0, -1.0f};
	camUp = {0, 1.0f, 0};
	prevXFactor = 0;
	prevYFactor = 0;
}

void SamplePart::onStart()
{
	engine.getRenderer().setClearColor({0, 0, 0});

	EntityComponentSystem &ecs = engine.getEcs();
	//LightSource light1({0, 50, 50}, {0.96f, 0.87f, 0.46f}); // candle light colour
	LightSource light1({0, 50, 50}, {1, 1, 1});
	Entity eLight1 = ecs.newEntity();
	auto lightComp = ecs.createComponent<LightingComponent>(eLight1, light1);

	Boiler::GLTFImporter gltfImporter;

	if (modelPath != "")
	{
		Boiler::ImportResult worldResult = gltfImporter.import(engine, modelPath);
	}
	else
	{
		Boiler::ImportResult worldResult = gltfImporter.import(engine, "data/blender/test-terrain.gltf");

		Boiler::ImportResult cubeResult = gltfImporter.import(engine, "data/glTF-Sample-Models-master/2.0/AnimatedCube/glTF/AnimatedCube.gltf");
		Entity animatedCube = cubeResult.entities[0];
		auto &cubePos = ecs.getComponentStore().retrieve<TransformComponent>(animatedCube);
		cubePos.setPosition(vec3(5, 1, 15));

		Boiler::ImportResult enemy1Result = gltfImporter.import(engine, "data/vedova_nera/scene.gltf");
		Entity renegade = enemy1Result.entities[0];
		auto &renPos = ecs.getComponentStore().retrieve<TransformComponent>(renegade);
		renPos.setScale(vec3(0.2f, 0.2f, 0.2f));
		renPos.setPosition(vec3(0, 0, 15));

		Boiler::ImportResult mechResult = gltfImporter.import(engine, "data/xyz_draft._tank/scene.gltf");
		Entity mech = mechResult.entities[0];
		auto &mechPos = ecs.getComponentStore().retrieve<TransformComponent>(mech);
		mechPos.setScale(vec3(0.2f, 0.2f, 0.2f));
		mechPos.setPosition(vec3(3, 0, 10));

		int x = -10;
		for (int i = 0; i < 3; ++i)
		{
			Boiler::ImportResult sorceressResult = gltfImporter.import(engine, "data/sorceress/scene.gltf");
			Entity sor = sorceressResult.entities[0];
			auto &pos = ecs.getComponentStore().retrieve<TransformComponent>(sor);
			pos.setScale(0.001f);
			pos.setPosition(vec3(x, 1, 10));
			x += 2;
		}
	}

	auto mouseListener = [this](const MouseMotionEvent &event)
	{
		// calculate mouse move factor based on current resolution
		const Size size = engine.getRenderer().getScreenSize();
		const float xFactorNew = event.xDistance / size.width;
		const float yFactorNew = event.yDistance / size.height;

		const float alpha = 0.5f;
		const float xFactor = prevXFactor + alpha * (xFactorNew - prevXFactor);
		const float yFactor = prevYFactor + alpha * (yFactorNew - prevYFactor);

		const float sensitivity = 1.5;
		const float xDiff = sensitivity * xFactor;
		const float yDiff = sensitivity * yFactor;

		camDirection = glm::rotate(camDirection, static_cast<float>(-xDiff), camUp);
		const glm::vec3 axis = glm::cross(camUp, camDirection);
		camDirection = glm::rotate(camDirection, static_cast<float>(yDiff), axis);

		prevXFactor = xFactor;
		prevYFactor = yFactor;

		mouseMotion = event;
	};

	engine.addMouseMotionListener(mouseListener);

    auto keyListener = [this](const KeyInputEvent &event)
	{
		if (event.keyCode == SDLK_a)
		{
			moveLeft = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_d)
		{
			moveRight = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_s)
		{
			moveCloser = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_w)
		{
			moveFurther = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_PAGEUP)
		{
			moveUp = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_PAGEDOWN)
		{
			moveDown = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_r && event.state == ButtonState::UP)
		{
			engine.getAnimator().resetTime();
		}
	};
	engine.addKeyInputListener(keyListener);
}

void SamplePart::update(double deltaTime)
{
	const float speed = 5.0f;
	EntityComponentSystem &ecs = engine.getEcs();
	if (moveLeft)
	{
		glm::vec3 moveAmount = glm::cross(camDirection, camUp);
		moveAmount *= deltaTime * speed;
		camPosition -= moveAmount;
	}
	else if (moveRight)
	{
		glm::vec3 moveAmount = glm::cross(camDirection, camUp);
		moveAmount *= deltaTime * speed;
		camPosition += moveAmount;
	}

	if (moveCloser)
	{
		glm::vec3 moveAmount = camDirection;
		moveAmount *= speed * deltaTime;
		camPosition -= moveAmount;
	}
	else if (moveFurther)
	{
		glm::vec3 moveAmount = camDirection;
		moveAmount *= speed * deltaTime;
		camPosition += moveAmount;
	}

	if (moveUp)
	{
		camPosition.y += speed * deltaTime;
	}
	else if (moveDown)
	{
		camPosition.y -= speed * deltaTime;
	}

	glm::mat4 view = glm::lookAt(camPosition, camPosition + camDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	engine.getRenderer().setViewMatrix(view);
	engine.getRenderer().setCameraPosition(camPosition);
}
