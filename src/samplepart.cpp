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

#include "core/components/rendercomponent.h"
#include "core/components/lightingcomponent.h"
#include "core/components/transformcomponent.h"
#include "input/inputcomponent.h"
#include "camera/cameracomponent.h"
#include "physics/movementcomponent.h"
#include "physics/collisioncomponent.h"

using namespace Boiler;

SamplePart::SamplePart(Engine &engine) : Part("Sample", engine), logger("Sample Part")
{
}

void SamplePart::onStart()
{
	engine.getRenderer().setClearColor({0, 0, 0});

	EntityComponentSystem &ecs = engine.getEcs();
	LightSource lightSource1({-30, 20, 0}, {0.8, 0.8, 0.8});
	light1 = ecs.newEntity();
	auto lightComp = ecs.createComponent<LightingComponent>(light1, lightSource1);

	MapLoader mapLoader(assetSet, engine);
	mapLoader.load("data/terrain.json");

	// skybox
	SkyBoxLoader skyLoader(assetSet, engine.getRenderer(), ecs);
	skyBox = skyLoader.load("data/skybox/opengltutorial/top.jpg", "data/skybox/opengltutorial/bottom.jpg",
							"data/skybox/opengltutorial/left.jpg", "data/skybox/opengltutorial/right.jpg",
							"data/skybox/opengltutorial/front.jpg", "data/skybox/opengltutorial/back.jpg");
}

void SamplePart::update(const FrameInfo &frameInfo)
{
}
