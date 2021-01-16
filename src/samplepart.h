#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include "boiler.h"
#include "core/common.h"
#include "core/part.h"
#include "input/mousemotionevent.h"

class SamplePart : public Boiler::Part
{
	Boiler::Logger logger;

	Boiler::MouseMotionEvent mouseMotion;
	float prevXFactor, prevYFactor;

	glm::vec3 camPosition;
	glm::vec3 camDirection;
	glm::vec3 camUp;

	bool moveLeft, moveRight, moveCloser, moveFurther, moveUp, moveDown;
	Boiler::Entity skyBox, light1;
	
public:
	SamplePart(Boiler::Engine &engine);

    void onStart() override;
    void update(Boiler::Time deltaTime) override;
};

#endif /* SAMPLEPART_H */
