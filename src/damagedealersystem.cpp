#include "damagedealersystem.h"
#include "physics/collisioncomponent.h"
#include "healthcomponent.h"
#include "core/entitycomponentsystem.h"
#include "damagedealercomponent.h"
#include "damagecomponent.h"

using namespace Boiler;

DamageDealerSystem::DamageDealerSystem() : System("Damage Dealer System")
{
	expects<CollisionComponent>();
	expects<DamageDealerComponent>();
}

void DamageDealerSystem::update(Renderer &renderer, AssetSet &assetSet, const FrameInfo &frameInfo, EntityComponentSystem &ecs)
{
	for (Entity entity : getEntities())
	{
		DamageDealerComponent &damageDealer = ecs.retrieve<DamageDealerComponent>(entity);
		CollisionComponent &collision = ecs.retrieve<CollisionComponent>(entity);

		// generate damage from the collision
		ecs.createComponent<DamageComponent>(collision.target, damageDealer.amount);
	}
}
