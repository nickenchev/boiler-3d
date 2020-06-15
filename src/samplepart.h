#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include <memory>
#include "boiler.h"

class SamplePart : public Boiler::Part
{
	Boiler::Entity object;

	bool turnLeft, turnRight, moveCloser, moveFurther;
	std::shared_ptr<const Boiler::Texture> tex;
	
public:
    SamplePart(Boiler::Engine &engine);

    void onStart() override;
    void update(double deltaTime) override;
};

#endif /* SAMPLEPART_H */
