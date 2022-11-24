#include "core/entitycomponentsystem.h"
#include "paddlemovementsystem.h"
#include "core/components/transformcomponent.h"
#include "input/inputcomponent.h"
#include "physics/movementcomponent.h"
#include "physics/physicscomponent.h"

using namespace Boiler;

PaddleMovementSystem::PaddleMovementSystem() : Boiler::System("Paddle System")
{
	expects<TransformComponent>();
	expects<InputComponent>();
	expects<MovementComponent>();
	expects<PhysicsComponent>();
}

void PaddleMovementSystem::update(Renderer &renderer, AssetSet &assetSet, const FrameInfo &frameInfo, EntityComponentSystem &ecs)
{
	for (Entity entity : getEntities())
	{
		MovementComponent &movement = ecs.getComponentStore().retrieve<MovementComponent>(entity);
		PhysicsComponent &physics = ecs.getComponentStore().retrieve<PhysicsComponent>(entity);

		cgfloat acceleration = physics.speed * physics.acceleration;
		vec3 moveDirection(0, 0, 0);
		bool moving = false;
		if (movement.moveLeft)
		{
			moveDirection = vec3(-1, 0, 0);
			moving = true;
		}
		if (movement.moveRight)
		{
			moveDirection += vec3(1, 0, 0);
			moving = true;
		}

		if (glm::length2(moveDirection))
		{
			physics.velocity = moveDirection * acceleration;
		}
		else
		{
			physics.velocity *= 0;
		}
		
	}
}
