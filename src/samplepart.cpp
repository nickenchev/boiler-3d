#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <SDL2/SDL_keycode.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "samplepart.h"
#include "input/inputevent.h"
#include "core/entitycomponentsystem.h"
#include "display/skyboxloader.h"
#include "assets/maps/maploader.h"

#include "physics/collisionsystem.h"
#include "physics/collisioncomponent.h"
#include "core/components/rendercomponent.h"
#include "core/components/lightingcomponent.h"
#include "core/components/transformcomponent.h"
#include "input/inputcomponent.h"
#include "camera/cameracomponent.h"
#include "physics/movementcomponent.h"

using namespace Boiler;

SamplePart::SamplePart(Engine &engine) : Part("Sample", engine), logger("Sample Part")
{
}

void SamplePart::onStart()
{
	engine.getRenderer().setClearColor({0, 0, 0});

	EntityComponentSystem &ecs = engine.getEcs();
	ecs.getComponentSystems().registerSystem<CollisionSystem>()
		.expects<CollisionComponent>()
		.expects<TransformComponent>();

	LightSource lightSource1({-30, 20, 0}, {0.8, 0.8, 0.8});
	light1 = ecs.newEntity();
	auto lightComp = ecs.createComponent<LightingComponent>(light1, lightSource1);

	MapLoader mapLoader(engine);
	mapLoader.load("data/level.json");

	// skybox
	SkyBoxLoader skyLoader(engine.getRenderer(), ecs);
	skyBox = skyLoader.load("data/skybox/opengltutorial/top.jpg", "data/skybox/opengltutorial/bottom.jpg",
							"data/skybox/opengltutorial/left.jpg", "data/skybox/opengltutorial/right.jpg",
							"data/skybox/opengltutorial/front.jpg", "data/skybox/opengltutorial/back.jpg");

	Entity camera = ecs.newEntity();
	ecs.createComponent<InputComponent>(camera);
	auto transformComp = ecs.createComponent<TransformComponent>(camera);
	transformComp->setPosition(0, 2.0f, 5.0f);
	auto camComp = ecs.createComponent<CameraComponent>(camera);
	camComp->direction = {0, 0, -1.0f};
	camComp->up = {0, 1.0f, 0};
	ecs.createComponent<MovementComponent>(camera);
}

void SamplePart::update(Boiler::Time deltaTime)
{
	/*
	const float speed = 10;
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
	*/
}
