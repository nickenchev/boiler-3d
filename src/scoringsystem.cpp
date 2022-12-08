#include "scoringsystem.h"
#include "scorecomponent.h"
#include "core/entitycomponentsystem.h"
#include "healthcomponent.h"

using namespace Boiler;

ScoringSystem::ScoringSystem() : System("Scoring System")
{
	score = 0;
	expects<ScoreComponent>();
	expects<HealthComponent>();
}

void ScoringSystem::update(Renderer &renderer, AssetSet &assetSet, const FrameInfo &frameInfo, EntityComponentSystem &ecs)
{
	for (Entity entity : getEntities())
	{
		HealthComponent &health = ecs.retrieve<HealthComponent>(entity);
		if (health.current <= 0)
		{
			ScoreComponent &scoreComp = ecs.retrieve<ScoreComponent>(entity);
			score += scoreComp.score;

			ecs.removeComponent<HealthComponent>(entity);
		}
	}
}
