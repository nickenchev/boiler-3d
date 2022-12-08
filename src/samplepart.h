#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include "core/part.h"
#include "core/entity.h"
#include "display/glyphmap.h"

namespace Boiler
{
	class EntityComponentSystem;
}

class SamplePart : public Boiler::Part
{
	Boiler::Logger logger;
	Boiler::Entity skyBox, light1, text1, ball;
	Boiler::GlyphMap glyphMap;
	unsigned int paddles;
	
public:
	SamplePart(Boiler::Engine &engine);

    void onStart() override;
    void update(const Boiler::FrameInfo &frameInfo) override;
};

#endif /* SAMPLEPART_H */
