#include <glm/glm.hpp>
#include "samplepart.h"
#include "core/entitycomponentsystem.h"
#include "video/vertexdata.h"
#include "core/components/rendercomponent.h"
#include "core/components/positioncomponent.h"

using namespace Boiler;

SamplePart::SamplePart() : Part("Sample")
{
}

void SamplePart::onStart(Engine &engine)
{
	/*
	VertexData vertData({
		{ -0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f,  0.5f, -0.5f },
		{ 0.5f,  0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },

		{ -0.5f, -0.5f,  0.5f },
		{ 0.5f, -0.5f,  0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },

		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },

		{ 0.5f,  0.5f,  0.5f },
		{ 0.5f,  0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f,  0.5f },
		{ 0.5f,  0.5f,  0.5f },

		{ -0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f,  0.5f },
		{ 0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f, -0.5f },

		{ -0.5f,  0.5f, -0.5f },
		{ 0.5f,  0.5f, -0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f, -0.5f }
	});
	*/

	this->engine = &engine;
	engine.getRenderer().setClearColor({0, 0, 0});

	VertexData vertData({
		{ -0.5f, -0.5f, 0 },
		{ 0.5f, -0.5f, 0 },
		{ 0.5f,  0.5f, 0 },
		{ 0.5f,  0.5f, 0 },
		{ -0.5f,  0.5f, 0 },
		{ -0.5f, -0.5f, 0 },
	});

	EntityComponentSystem &ecs = engine.getEcs();
	object = ecs.newEntity();
	auto renderComp = ecs.createComponent<RenderComponent>(object, engine.getRenderer().loadModel(vertData));
	renderComp->colour = Colour::fromRGBA(252, 171, 20, 255);
	auto renderPos = ecs.createComponent<PositionComponent>(object, Rect(0, 0, 0, 0));
	renderPos->rotationAxis = glm::vec3(0, 1, 0);
}

void SamplePart::update(double deltaTime)
{
	EntityComponentSystem &ecs = engine->getEcs();

	PositionComponent &pos = ecs.getComponentStore().retrieve<PositionComponent>(object);
	pos.rotationAngle += 25.0f * deltaTime;
}
