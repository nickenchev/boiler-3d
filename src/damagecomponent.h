#ifndef DAMAGECOMPONENT_H
#define DAMAGECOMPONENT_H

#include "core/componenttype.h"

struct DamageComponent : public Boiler::ComponentType<DamageComponent>
{
	int amount;

	DamageComponent(int amount) : Boiler::ComponentType<DamageComponent>(this)
	{
		this->amount = amount;
	}
};

#endif /* DAMAGECOMPONENT_H */
