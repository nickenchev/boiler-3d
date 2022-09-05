#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include "core/part.h"

class SamplePart : public Boiler::Part
{
	Boiler::Logger logger;
	Boiler::Entity skyBox, light1;
	
public:
	SamplePart(Boiler::Engine &engine);

    void onStart() override;
    void update(const Boiler::FrameInfo &frameInfo) override;
};

#endif /* SAMPLEPART_H */
