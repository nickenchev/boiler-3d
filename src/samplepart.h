#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include "core/part.h"
#include "core/entity.h"
#include "display/glyphmap.h"

class SamplePart : public Boiler::Part
{
	Boiler::Logger logger;
	Boiler::Entity skyBox, light1, text1;
	Boiler::GlyphMap glyphMap;
	
public:
	SamplePart(Boiler::Engine &engine);

    void onStart() override;
    void update(const Boiler::FrameInfo &frameInfo) override;
};

#endif /* SAMPLEPART_H */
