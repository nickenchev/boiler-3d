#include "damagesystem.h"
#include "physics/collisioncomponent.h"
#include "healthcomponent.h"
#include "core/entitycomponentsystem.h"
#include "core/components/rendercomponent.h"
#include "physics/collidercomponent.h"

using namespace Boiler;

DamageSystem::DamageSystem() : System("Damage System")
{
	expects<CollisionComponent>();
	expects<HealthComponent>();
}

void DamageSystem::update(Renderer &renderer, AssetSet &assetSet, const FrameInfo &frameInfo, EntityComponentSystem &ecs)
{
	for (int i = 0; i < getEntities().size(); ++i)
	{
		Entity entity = getEntities()[i];
		HealthComponent &health = ecs.getComponentStore().retrieve<HealthComponent>(entity);
		health.current -= 10;
		logger.log("{} HP: {}", ecs.nameOf(entity), health.current);

		if (health.current <= 0)
		{
			ecs.removeComponent<RenderComponent>(entity);
			ecs.removeComponent<ColliderComponent>(entity);
		}
	}
}
