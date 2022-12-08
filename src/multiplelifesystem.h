#ifndef MULTIPLELIFESYSTEM_H
#define MULTIPLELIFESYSTEM_H

#include "core/system.h"

class MultipleLifeSystem : public Boiler::System
{
public:
    MultipleLifeSystem();

	void update(Boiler::Renderer &renderer, Boiler::AssetSet &assetSet, const Boiler::FrameInfo &frameInfo, Boiler::EntityComponentSystem &ecs) override;
};


#endif /* MULTIPLELIFESYSTEM_H */
