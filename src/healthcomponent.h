#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "core/componenttype.h"

struct HealthComponent : public Boiler::ComponentType<HealthComponent>
{
	unsigned int maxHealth;
	unsigned int current;

	HealthComponent(int maxHealth) : Boiler::ComponentType<HealthComponent>(this)
	{
		this->maxHealth = maxHealth;
		this->current = maxHealth;
	}
};


#endif /* HEALTHCOMPONENT_H */
