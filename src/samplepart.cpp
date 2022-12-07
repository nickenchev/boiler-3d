#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <SDL_keycode.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "core/entitycomponentsystem.h"
#include "display/skyboxloader.h"
#include "display/renderer.h"
#include "assets/maps/maploader.h"

#include "camera/cameracomponent.h"
#include "physics/movementcomponent.h"
#include "input/inputcomponent.h"
#include "core/components/rendercomponent.h"
#include "core/components/lightingcomponent.h"
#include "core/components/transformcomponent.h"
#include "physics/collisioncomponent.h"
#include "physics/physicscomponent.h"
#include "core/components/textcomponent.h"
#include "core/components/guicomponent.h"
#include "assets/gltfimporter.h"
#include "core/glyphloader.h"

#include "samplepart.h"
#include "paddlemovementsystem.h"

#include "imgui/imgui.h"

using namespace Boiler;

SamplePart::SamplePart(Engine &engine) : Part("Sample", engine), logger("Sample Part")
{
	engine.getEcs().getComponentSystems().registerSystem<PaddleMovementSystem>(SystemStage::USER_SIMULATION);
	score = 0;
	balls = 2;
}

void SamplePart::onStart()
{
	engine.getRenderer().setClearColor({0, 0, 0});

	//MapLoader mapLoader(engine.getRenderer().getAssetSet(), engine);
	//mapLoader.load("data/level.json");
	EntityComponentSystem &ecs = engine.getEcs();

	Entity camera = ecs.newEntity("Camera");
	TransformComponent &transformComponent = ecs.createComponent<TransformComponent>(camera);
	transformComponent.setPosition(0, 0, 50);
	CameraComponent &cameraComponent = ecs.createComponent<CameraComponent>(camera);
	cameraComponent.up = vec3(0, 1, 0);
	cameraComponent.direction = vec3(0, 0, -1);
	ecs.createComponent<MovementComponent>(camera);

	GLTFImporter importStage(engine.getRenderer().getAssetSet(), engine, "data/breakout/stage1/stage1.gltf");
	Entity stage = ecs.newEntity("Stage");
	importStage.createInstance(stage);
	TransformComponent &stageTransform = ecs.createComponent<TransformComponent>(stage);
	stageTransform.setScale(2.2f, 3.0f, 3.0f);
	stageTransform.setPosition(0, -40, 20);

	GLTFImporter importBrick1(engine.getRenderer().getAssetSet(), engine, "data/breakout/brick/brick1.gltf");
	cgfloat y = 15;
	for (int r = 0; r < 6; ++r)
	{
		cgfloat x = -19;
		for (int c = 0; c < 13; ++c)
		{
			Entity brick1 = ecs.newEntity("Brick");
			importBrick1.createInstance(brick1);
			TransformComponent &brickTransform = ecs.getComponentStore().retrieve<TransformComponent>(brick1);
			brickTransform.setScale(brickTransform.getScale() * vec3(0.7f, 1, 1));
			brickTransform.setPosition(x, y, 0);
			x += 3.2f;
			ecs.createComponent<PhysicsComponent>(brick1);
			CollisionComponent &brickCollision = ecs.getComponentStore().retrieve<CollisionComponent>(brick1);
			brickCollision.normal = vec3(0, -1, 0);
		}
		y -= 2.5f;
	}

	GLTFImporter importPaddle(engine.getRenderer().getAssetSet(), engine, "data/breakout/paddle.gltf");
	Entity paddle = ecs.newEntity("Paddle");
	importPaddle.createInstance(paddle);
	TransformComponent &paddleTransform = ecs.getComponentStore().retrieve<TransformComponent>(paddle);
	paddleTransform.setPosition(0, -18, 0);
	InputComponent &inputComponent = ecs.createComponent<InputComponent>(paddle);
	MovementComponent &movementComponent = ecs.createComponent<MovementComponent>(paddle);
	PhysicsComponent &paddlePhysics = ecs.createComponent<PhysicsComponent>(paddle);
	paddlePhysics.speed = 20.0f;
	paddlePhysics.acceleration = 1.0f;
	paddlePhysics.mass = 999;
	CollisionComponent &paddleCollision = ecs.getComponentStore().retrieve<CollisionComponent>(paddle);
	paddleCollision.normal = vec3(0, 1, 0);
	paddleCollision.isDynamic = true;

	GLTFImporter importBall(engine.getRenderer().getAssetSet(), engine, "data/breakout/ball.gltf");
	ball = ecs.newEntity("Ball");
	importBall.createInstance(ball);
	TransformComponent &ballTransform = ecs.getComponentStore().retrieve<TransformComponent>(ball);
	ballTransform.setPosition(0, -15, 0);
	ballTransform.setScale(0.4f, 0.4f, 0.4f);
	PhysicsComponent &physicsComponent = ecs.createComponent<PhysicsComponent>(ball);
	physicsComponent.velocity = glm::normalize(vec3(-0.5f, 1.0f, 0)) * 25.0f;
	CollisionComponent &collisionComponent = ecs.getComponentStore().retrieve<CollisionComponent>(ball);
	collisionComponent.isDynamic = true;
	collisionComponent.damping = 1;
	collisionComponent.colliderType = ColliderType::Sphere;

	LightSource lightSource1({0, 0, 20}, {0.8, 0.8, 0.8});
	light1 = ecs.newEntity("Light 1");
	auto &lightComp = ecs.createComponent<LightingComponent>(light1, lightSource1);

	// GlyphLoader glyphLoader(engine.getRenderer(), engine.getRenderer().getAssetSet());
	// AssetId glyphId = glyphLoader.loadFace("data/fonts/Retroville NC.ttf", 32);

	// text1 = ecs.newEntity("FPS counter");
	// ecs.createComponent<TextComponent>(text1, "", glyphId);
	// ecs.createComponent<TransformComponent>(text1, Rect(10, 35, Size(100, 50)));

	// skybox
	// SkyBoxLoader skyLoader(engine.getRenderer().getAssetSet(), engine.getRenderer(), ecs);
	// skyBox = skyLoader.load("data/skybox/opengltutorial/top.jpg", "data/skybox/opengltutorial/bottom.jpg",
	// 						"data/skybox/opengltutorial/left.jpg", "data/skybox/opengltutorial/right.jpg",
	// 						"data/skybox/opengltutorial/front.jpg", "data/skybox/opengltutorial/back.jpg");

	Entity gui = ecs.newEntity("gui");
	ecs.createComponent<GUIComponent>(gui, [this] {
		ImGui::Begin("Demo");
		ImGui::Text(fmt::format("Score: {}", score).c_str());
		ImGui::Text("Balls: 3");
		if (ImGui::Button("Reset Game"))
		{
			score = 0;
			balls = 2;
		}
		ImGui::End();
	});
}

void SamplePart::update(const FrameInfo &frameInfo)
{
	score += 10;
	TransformComponent &ballTransform = engine.getEcs().getComponentStore().retrieve<TransformComponent>(ball);
	LightingComponent &lightComp = engine.getEcs().getComponentStore().retrieve<LightingComponent>(light1);

	vec3 newLightPos = ballTransform.getPosition() + vec3(0, -5, 10);
	lightComp.source.position = vec4(newLightPos, 0);
	
	// TextComponent &textComp = engine.getEcs().getComponentStore().retrieve<TextComponent>(text1);
	// textComp.text = fmt::format("{}, {} fps", frameInfo.globalTime, frameInfo.frameCount / frameInfo.globalTime);
}
