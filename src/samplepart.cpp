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
	turnLeft = false;
	turnRight = false;
	lookUp = false;
	lookDown = false;

	camPosition = {0, 0, 0};
	camDirection = {0, 0, -1.0f};
	camUp = {0, 1.0f, 0};
}

void SamplePart::onStart()
{
	engine.getRenderer().setClearColor({135, 206, 235});

	EntityComponentSystem &ecs = engine.getEcs();
	LightSource light1({0, 150, 0}, {1, 1, 1});
	Entity eLight1 = ecs.newEntity();
	auto lightComp = ecs.createComponent<LightingComponent>(eLight1, light1);

	Boiler::GLTFImporter gltfImporter;
	gltfImporter.import(engine, modelPath);

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
		else if (event.keyCode == SDLK_LEFT)
		{
			turnLeft = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_RIGHT)
		{
			turnRight = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_UP)
		{
			lookUp = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_DOWN)
		{
			lookDown = event.state == ButtonState::DOWN;
		}
	};
	engine.addKeyInputListener(keyListener);
}

void SamplePart::update(double deltaTime)
{
	const float speed = 50.0f;
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
		camPosition.y += 10 * deltaTime;
	}
	else if (moveDown)
	{
		camPosition.y -= 10 * deltaTime;
	}

	if (lookUp)
	{
		const glm::vec3 axis = glm::cross(camUp, camDirection);
		camDirection = glm::rotate(camDirection, static_cast<float>(-3 * deltaTime), axis);
		camUp = glm::cross(camDirection, axis);
	}
	else if (lookDown)
	{
		const glm::vec3 axis = glm::cross(camUp, camDirection);
		camDirection = glm::rotate(camDirection, static_cast<float>(3 * deltaTime), axis);
		camUp = glm::cross(camDirection, axis);
	}

	if (turnLeft)
	{
		camDirection = glm::rotate(camDirection, static_cast<float>(3 * deltaTime), camUp);
	}
	else if (turnRight)
	{
		camDirection = glm::rotate(camDirection, static_cast<float>(-3 * deltaTime), camUp);
	}

	glm::mat4 view = glm::lookAt(camPosition, camPosition + camDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	engine.getRenderer().setViewMatrix(view);
	engine.getRenderer().setCameraPosition(camPosition);
}
