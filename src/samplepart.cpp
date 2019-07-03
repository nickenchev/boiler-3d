#include "samplepart.h"
#include "core/entitycomponentsystem.h"

using namespace Boiler;

SamplePart::SamplePart() : Part("Sample")
{
}

void SamplePart::onStart()
{
	EntityComponentSystem &ecs = Engine::getInstance().getEcs();

	Entity object = ecs.newEntity();
}

void SamplePart::update(double deltaTime)
{
}
