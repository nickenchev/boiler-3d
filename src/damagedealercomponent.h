#ifndef DAMAGEDEALERCOMPONENT_H
#define DAMAGEDEALERCOMPONENT_H

#include "core/componenttype.h"

struct DamageDealerComponent : public Boiler::ComponentType<DamageDealerComponent>
{
	int amount;

	DamageDealerComponent(int amount) : Boiler::ComponentType<DamageDealerComponent>(this)
	{
		this->amount = amount;
	}
};

#endif /* DAMAGEDEALERCOMPONENT_H */
