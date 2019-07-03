#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include <memory>
#include "boiler.h"

class SamplePart : public Boiler::Part
{
	
public:
    SamplePart();

    void onStart() override;
    void update(double deltaTime) override;
};

#endif /* SAMPLEPART_H */
