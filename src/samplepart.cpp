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
#include "video/skyboxloader.h"

#include "core/components/rendercomponent.h"
#include "core/components/lightingcomponent.h"
#include "core/components/transformcomponent.h"

using namespace Boiler;

SamplePart::SamplePart(Engine &engine) : Part("Sample", engine), logger("Sample Part")
{
	moveLeft = false;
	moveRight = false;
	moveCloser = false;
	moveFurther = false;
	moveUp = false;
	moveDown = false;

	camPosition = {5, 5, 0};
	camDirection = {0, 0, -1.0f};
	camUp = {0, 1.0f, 0};
	prevXFactor = 0;
	prevYFactor = 0;
}

void SamplePart::onStart()
{
	engine.getRenderer().setClearColor({0, 0, 0});

	EntityComponentSystem &ecs = engine.getEcs();
	LightSource lightSource1({0, 50, 50}, {1, 1, 1});
	light1 = ecs.newEntity();
	auto lightComp = ecs.createComponent<LightingComponent>(light1, lightSource1);

	// skybox
	SkyBoxLoader skyLoader(engine.getRenderer(), ecs);
	skyBox = skyLoader.load("data/skybox/opengltutorial/top.jpg", "data/skybox/opengltutorial/bottom.jpg",
							"data/skybox/opengltutorial/left.jpg", "data/skybox/opengltutorial/right.jpg",
							"data/skybox/opengltutorial/front.jpg", "data/skybox/opengltutorial/back.jpg");

	//Boiler::GLTFImporter groundGltf(engine, "data/blender/ground.gltf");
	Boiler::GLTFImporter groundGltf(engine, "data/littlest_tokyo/glTF/littlest_tokyo.gltf");
	Entity ground = ecs.newEntity();
	groundGltf.createInstance(ground);
	//ecs.getComponentStore().retrieve<TransformComponent>(ground).setScale(0.1);

	auto mouseListener = [this](const MouseMotionEvent &event)
	{
		mouseMotion = event; // moving this to the bottom allows for interpolation

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
			//engine.getAnimator().resetTime();
		}
		else if (event.keyCode == SDLK_l && event.state == ButtonState::UP)
		{
			std::string path;
			std::cout << "boiler: ";
			std::cin >> path;
		}
	};
	engine.addKeyInputListener(keyListener);
}

void SamplePart::update(Boiler::Time deltaTime)
{
	const float speed = 20.0f;
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

	ecs.getComponentStore().retrieve<TransformComponent>(skyBox).setPosition(camPosition);
}
