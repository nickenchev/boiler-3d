#ifndef SCORINGSYSTEM_H
#define SCORINGSYSTEM_H

#include "core/system.h"

class ScoringSystem : public Boiler::System
{
	unsigned int score;

public:
	ScoringSystem();
	void update(Boiler::Renderer &renderer, Boiler::AssetSet &assetSet, const Boiler::FrameInfo &frameInfo, Boiler::EntityComponentSystem &ecs) override;

	unsigned int getScore() const { return score; }

	void resetScore() { score = 0; }
};

#endif /* SCORINGSYSTEM_H */
