#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "core/system.h"

class DamageSystem : public Boiler::System
{
public:
	DamageSystem();

	void update(Boiler::Renderer &renderer, Boiler::AssetSet &assetSet, const Boiler::FrameInfo &frameInfo, Boiler::EntityComponentSystem &ecs) override;
};

#endif /* DAMAGESYSTEM_H */
