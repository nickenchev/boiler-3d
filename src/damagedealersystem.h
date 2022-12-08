#ifndef DAMAGEDEALERSYSTEM_H
#define DAMAGEDEALERSYSTEM_H

#include "core/system.h"

class DamageDealerSystem : public Boiler::System
{
public:
	DamageDealerSystem();

	void update(Boiler::Renderer &renderer, Boiler::AssetSet &assetSet, const Boiler::FrameInfo &frameInfo, Boiler::EntityComponentSystem &ecs) override;
};

#endif /* DAMAGEDEALERSYSTEM_H */
