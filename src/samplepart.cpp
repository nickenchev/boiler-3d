#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <SDL_keycode.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "samplepart.h"
#include "core/entitycomponentsystem.h"
#include "display/skyboxloader.h"
#include "display/renderer.h"
#include "assets/maps/maploader.h"

#include "core/components/rendercomponent.h"
#include "core/components/lightingcomponent.h"
#include "core/components/transformcomponent.h"
#include "physics/collisioncomponent.h"
#include "core/components/textcomponent.h"
#include "core/components/guicomponent.h"

#include "core/glyphloader.h"

using namespace Boiler;

SamplePart::SamplePart(Engine &engine) : Part("Sample", engine), logger("Sample Part")
{
}

void SamplePart::onStart()
{
	engine.getRenderer().setClearColor({0, 0, 0});

	EntityComponentSystem &ecs = engine.getEcs();
	LightSource lightSource1({-30, 20, 0}, {0.8, 0.8, 0.8});
	light1 = ecs.newEntity("light 1");
	auto lightComp = ecs.createComponent<LightingComponent>(light1, lightSource1);

	MapLoader mapLoader(engine.getRenderer().getAssetSet(), engine);
	mapLoader.load("data/level.json");

	/*
	GlyphLoader glyphLoader(engine.getRenderer(), engine.getRenderer().getAssetSet());
	AssetId glyphId = glyphLoader.loadFace("data/fonts/Retroville NC.ttf", 32);

	text1 = ecs.newEntity();
	ecs.createComponent<TextComponent>(text1, "", glyphId);
	ecs.createComponent<TransformComponent>(text1, Rect(10, 35, Size(100, 50)));
	*/

	// skybox
	SkyBoxLoader skyLoader(engine.getRenderer().getAssetSet(), engine.getRenderer(), ecs);
	skyBox = skyLoader.load("data/skybox/opengltutorial/top.jpg", "data/skybox/opengltutorial/bottom.jpg",
							"data/skybox/opengltutorial/left.jpg", "data/skybox/opengltutorial/right.jpg",
							"data/skybox/opengltutorial/front.jpg", "data/skybox/opengltutorial/back.jpg");

	Entity gui = ecs.newEntity("gui");
	ecs.createComponent<GUIComponent>(gui, [] {
	});
}

void SamplePart::update(const FrameInfo &frameInfo)
{
	// TextComponent &textComp = engine.getEcs().getComponentStore().retrieve<TextComponent>(text1);
	// textComp.text = fmt::format("{}, {} fps", frameInfo.globalTime, frameInfo.frameCount / frameInfo.globalTime);
}
