#include "scoringsystem.h"
#include "scorecomponent.h"
#include "physics/collisioncomponent.h"
#include "core/entitycomponentsystem.h"

using namespace Boiler;

ScoringSystem::ScoringSystem() : System("Scoring System")
{
	score = 0;

	expects<ScoreComponent>();
	expects<CollisionComponent>();
}

void ScoringSystem::update(Renderer &renderer, AssetSet &assetSet, const FrameInfo &frameInfo, EntityComponentSystem &ecs)
{
	for (Entity entity : getEntities())
	{
		ScoreComponent &scoreComp = ecs.retrieve<ScoreComponent>(entity);
		score += scoreComp.score;
	}
}
