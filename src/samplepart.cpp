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
#include "physics/collidercomponent.h"
#include "physics/physicscomponent.h"
#include "core/components/textcomponent.h"
#include "core/components/guicomponent.h"
#include "assets/gltfimporter.h"
#include "core/glyphloader.h"
#include "imgui/imgui.h"

#include "samplepart.h"
#include "paddlemovementsystem.h"
#include "damagesystem.h"
#include "scoringsystem.h"
#include "scorecomponent.h"
#include "healthcomponent.h"

using namespace Boiler;

SamplePart::SamplePart(Engine &engine) : Part("Sample", engine), logger("Sample Part")
{
	balls = 2;
}

void SamplePart::onStart()
{
	engine.getRenderer().setClearColor({0, 0, 0});

	engine.getEcs().getComponentSystems().registerSystem<PaddleMovementSystem>(SystemStage::USER_SIMULATION);
	engine.getEcs().getComponentSystems().registerSystem<DamageSystem>(SystemStage::USER_SIMULATION);
	ScoringSystem &scoringSystem = engine.getEcs().getComponentSystems().registerSystem<ScoringSystem>(SystemStage::USER_SIMULATION);

	EntityComponentSystem &ecs = engine.getEcs();

	Entity camera = ecs.newEntity("Camera");
	TransformComponent &transformComponent = ecs.createComponent<TransformComponent>(camera);
	transformComponent.setPosition(0, 0, 60);
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
			Entity brick = ecs.newEntity("Brick");
			importBrick1.createInstance(brick);
			TransformComponent &brickTransform = ecs.getComponentStore().retrieve<TransformComponent>(brick);
			brickTransform.setScale(brickTransform.getScale() * vec3(0.7f, 1, 1));
			brickTransform.setPosition(x, y, 0);
			x += 3.2f;
			ecs.createComponent<PhysicsComponent>(brick);
			ColliderComponent &brickCollider = ecs.getComponentStore().retrieve<ColliderComponent>(brick);
			brickCollider.normal = vec3(0, -1, 0);
			HealthComponent &health = ecs.createComponent<HealthComponent>(brick, 10);
			ecs.createComponent<ScoreComponent>(brick, 15);

		}
		y -= 2.5f;
	}

	GLTFImporter importPaddle(engine.getRenderer().getAssetSet(), engine, "data/breakout/paddle.gltf");
	Entity paddle = ecs.newEntity("Paddle");
	importPaddle.createInstance(paddle);
	TransformComponent &paddleTransform = ecs.getComponentStore().retrieve<TransformComponent>(paddle);
	paddleTransform.setPosition(0, -22, 0);
	InputComponent &inputComponent = ecs.createComponent<InputComponent>(paddle);
	MovementComponent &movementComponent = ecs.createComponent<MovementComponent>(paddle);
	PhysicsComponent &paddlePhysics = ecs.createComponent<PhysicsComponent>(paddle);
	paddlePhysics.speed = 20.0f;
	paddlePhysics.acceleration = 1.0f;
	paddlePhysics.mass = 999;
	ColliderComponent &paddleCollider = ecs.getComponentStore().retrieve<ColliderComponent>(paddle);
	paddleCollider.normal = vec3(0, 1, 0);
	paddleCollider.isDynamic = true;

	GLTFImporter importBall(engine.getRenderer().getAssetSet(), engine, "data/breakout/ball.gltf");
	ball = ecs.newEntity("Ball");
	importBall.createInstance(ball);
	TransformComponent &ballTransform = ecs.getComponentStore().retrieve<TransformComponent>(ball);
	ballTransform.setPosition(0, -15, 0);
	ballTransform.setScale(0.4f, 0.4f, 0.4f);
	PhysicsComponent &physicsComponent = ecs.createComponent<PhysicsComponent>(ball);
	physicsComponent.velocity = glm::normalize(vec3(-0.5f, 1.0f, 0)) * 25.0f;
	ColliderComponent &colliderComponent = ecs.getComponentStore().retrieve<ColliderComponent>(ball);
	colliderComponent.isDynamic = true;
	colliderComponent.damping = 1;
	colliderComponent.colliderType = ColliderType::Sphere;

	LightSource lightSource1({0, 0, 20}, {0.8, 0.8, 0.8});
	light1 = ecs.newEntity("Light 1");
	auto &lightComp = ecs.createComponent<LightingComponent>(light1, lightSource1);

	Entity zone = ecs.newEntity("Dead Zone");
	ColliderComponent &zoneCollider = ecs.createComponent<ColliderComponent>(zone);
	zoneCollider.colliderType = ColliderType::AABB;
	zoneCollider.min = vec3(-25, 0, -5);
	zoneCollider.max = vec3(25, 5, 5);
	TransformComponent &zoneTransform = ecs.createComponent<TransformComponent>(zone);
	zoneTransform.setPosition(vec3(0, -33, 0));
	PhysicsComponent &zonePhysics = ecs.createComponent<PhysicsComponent>(zone);

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
	ecs.createComponent<GUIComponent>(gui, [this, &scoringSystem] {
		ImGui::Begin("Breakout Clone");
		ImGui::Text("%s", fmt::format("Score: {}", scoringSystem.getScore()).c_str());
		ImGui::Text("%s", fmt::format("Balls: {}", balls).c_str());
		if (ImGui::Button("Reset Game"))
		{
			scoringSystem.resetScore();
			balls = 2;
		}
		ImGui::End();
	});
}

void SamplePart::update(const FrameInfo &frameInfo)
{
	TransformComponent &ballTransform = engine.getEcs().getComponentStore().retrieve<TransformComponent>(ball);
	LightingComponent &lightComp = engine.getEcs().getComponentStore().retrieve<LightingComponent>(light1);

	vec3 newLightPos = ballTransform.getPosition() + vec3(0, -5, 10);
	lightComp.source.position = vec4(newLightPos, 0);
	
	// TextComponent &textComp = engine.getEcs().getComponentStore().retrieve<TextComponent>(text1);
	// textComp.text = fmt::format("{}, {} fps", frameInfo.globalTime, frameInfo.frameCount / frameInfo.globalTime);
}
