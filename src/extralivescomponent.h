#ifndef EXTRALIVESCOMPONENT_H
#define EXTRALIVESCOMPONENT_H

#include "core/componenttype.h"

struct ExtraLivesComponent : public Boiler::ComponentType<ExtraLivesComponent>
{
	unsigned int count;

	ExtraLivesComponent(unsigned int count) : Boiler::ComponentType<ExtraLivesComponent>(this)
	{
		this->count = count;
	}
};

#endif /* EXTRALIVESCOMPONENT_H */
