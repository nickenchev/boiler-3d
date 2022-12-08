#include "multiplelifesystem.h"
#include "healthcomponent.h"
#include "extralivescomponent.h"
#include "core/entitycomponentsystem.h"

using namespace Boiler;

MultipleLifeSystem::MultipleLifeSystem() : Boiler::System("Multi Life System")
{
	expects<HealthComponent>();
	expects<ExtraLivesComponent>();
}


void MultipleLifeSystem::update(Renderer &renderer, AssetSet &assetSet, const FrameInfo &frameInfo, EntityComponentSystem &ecs)
{
	for (Entity entity : getEntities())
	{
		HealthComponent &health = ecs.retrieve<HealthComponent>(entity);
		ExtraLivesComponent &extraLives = ecs.retrieve<ExtraLivesComponent>(entity);

		if (health.current <= 0 && extraLives.count > 0)
		{
			health.current = health.maxHealth;
			extraLives.count--;
		}
	}
}
