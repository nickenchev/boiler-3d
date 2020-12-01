#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include "boiler.h"
#include "core/part.h"
#include "input/mousemotionevent.h"

class SamplePart : public Boiler::Part
{
	Boiler::Logger logger;
	std::string modelPath;

	Boiler::MouseMotionEvent mouseMotion;
	float prevXFactor, prevYFactor;

	glm::vec3 camPosition;
	glm::vec3 camDirection;
	glm::vec3 camUp;

	bool moveLeft, moveRight, moveCloser, moveFurther, moveUp, moveDown;
	
public:
	SamplePart(Boiler::Engine &engine, const std::string &modelPath);

    void onStart() override;
    void update(double deltaTime) override;
};

#endif /* SAMPLEPART_H */
