#include "damagesystem.h"
#include "damagecomponent.h"
#include "healthcomponent.h"
#include "core/entitycomponentsystem.h"
#include "core/components/rendercomponent.h"
#include "physics/collidercomponent.h"
#include "scorecomponent.h"

using namespace Boiler;

DamageSystem::DamageSystem() : System("Damage System")
{
	expects<DamageComponent>();
	expects<HealthComponent>();
	expects<RenderComponent>();
	expects<ColliderComponent>();
}

void DamageSystem::update(Renderer &renderer, AssetSet &assetSet, const FrameInfo &frameInfo, EntityComponentSystem &ecs)
{
	for (Entity entity : getEntities())
	{
		HealthComponent &health = ecs.retrieve<HealthComponent>(entity);
		DamageComponent &damage = ecs.retrieve<DamageComponent>(entity);

		logger.log("Applying {} damage to {}", damage.amount, ecs.nameOf(entity));
		health.current -= damage.amount;

		if (health.current <= 0)
		{
			ecs.removeComponent<RenderComponent>(entity);
			ecs.removeComponent<ColliderComponent>(entity);
		}

		// TODO: This will mess up iterators
		ecs.removeComponent<DamageComponent>(entity);
	}
}
