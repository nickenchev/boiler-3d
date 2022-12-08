#ifndef PADDLEMOVEMENTSYSTEM_H
#define PADDLEMOVEMENTSYSTEM_H

#include "core/system.h"

class PaddleMovementSystem : public Boiler::System
{
public:
    PaddleMovementSystem();

	void update(Boiler::Renderer &renderer, Boiler::AssetSet &assetSet, const Boiler::FrameInfo &frameInfo, Boiler::EntityComponentSystem &ecs) override;
};

#endif /* PADDLEMOVEMENTSYSTEM_H */
