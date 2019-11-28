#include <array>
#include <glm/glm.hpp>
#include "SDL_keycode.h"
#include "input/inputevent.h"
#include "samplepart.h"
#include "core/entitycomponentsystem.h"
#include "video/vertexdata.h"
#include "core/components/rendercomponent.h"
#include "core/components/positioncomponent.h"
#include "core/components/spritecomponent.h"

using namespace Boiler;

SamplePart::SamplePart() : Part("Sample")
{
	turnLeft = false;
	turnRight = false;
}

void SamplePart::onStart(Engine &engine)
{
	tex = engine.getImageLoader().loadImage("data/diskette.png");
	SpriteSheetFrame sheetFrame(tex, nullptr);

	std::array<float, 108> verts = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	VertexData vertData(verts);

	this->engine = &engine;
	engine.getRenderer().setClearColor({0, 0, 0});

	EntityComponentSystem &ecs = engine.getEcs();
	object = ecs.newEntity();
	auto renderComp = ecs.createComponent<RenderComponent>(object, engine.getRenderer().loadModel(vertData), sheetFrame);
	renderComp->colour = Colour::fromRGBA(252, 171, 20, 255);
	auto renderPos = ecs.createComponent<PositionComponent>(object, Rect(0, 0, 0, 0));
	renderPos->scale *= 0.7f;
	renderPos->rotationAxis = glm::vec3(0, 1, 0);

    auto keyListener = [this, &engine](const KeyInputEvent &event)
	{
		const int maxTileSize = 100;
		EntityComponentSystem &ecs = engine.getEcs();

		if (event.state == ButtonState::DOWN)
		{
			if (event.keyCode == SDLK_a)
			{
				turnLeft = true;
			}
			else if (event.keyCode == SDLK_d)
			{
				turnRight = true;
			}
		}
		else if (event.state == ButtonState::UP)
		{
			if (event.keyCode == SDLK_a)
			{
				turnLeft = false;
			}
			else if (event.keyCode == SDLK_d)
			{
				turnRight = false;
			}
		}
	};
	engine.addKeyInputListener(keyListener);
}

void SamplePart::update(double deltaTime)
{
	EntityComponentSystem &ecs = engine->getEcs();
	PositionComponent &pos = ecs.getComponentStore().retrieve<PositionComponent>(object);
	if (turnLeft)
	{
		pos.rotationAngle += 35.0f * deltaTime;
	}
	else if (turnRight)
	{
		pos.rotationAngle -= 35.0f * deltaTime;
	}
}
