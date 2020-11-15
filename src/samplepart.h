#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include "boiler.h"
#include "core/part.h"

class SamplePart : public Boiler::Part
{
	Boiler::Logger logger;

	glm::vec3 camPosition;
	glm::vec3 camDirection;
	glm::vec3 camUp;

	bool moveLeft, moveRight, moveCloser, moveFurther, moveUp, moveDown, turnLeft, turnRight, lookUp, lookDown;
	
public:
    SamplePart(Boiler::Engine &engine);

    void onStart() override;
    void update(double deltaTime) override;
};

#endif /* SAMPLEPART_H */
