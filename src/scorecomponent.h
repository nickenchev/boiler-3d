#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "core/componenttype.h"

struct ScoreComponent : public Boiler::ComponentType<ScoreComponent>
{
	unsigned int score;

	ScoreComponent(unsigned int score) : Boiler::ComponentType<ScoreComponent>(this)
	{
		this->score = score;
	}
};

#endif /* SCORECOMPONENT_H */
