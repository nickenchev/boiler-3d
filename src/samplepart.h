#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include <memory>
#include "boiler.h"

class SamplePart : public Boiler::Part
{
	Boiler::Entity object;
	Boiler::Engine *engine;

	std::shared_ptr<const Boiler::Texture> tex;
	
public:
    SamplePart();

    void onStart(Boiler::Engine &engine) override;
    void update(double deltaTime) override;
};

#endif /* SAMPLEPART_H */
