#include <memory>
#include <boiler.h>
#include <video/vulkanrenderer.h>
#include <video/openglrenderer.h>

#include "samplepart.h"

int main(int argc, char *argv[])
{
	Boiler::Engine engine(std::make_unique<Boiler::VulkanRenderer>());
	engine.initialize(1280, 720);

	// create the part and start it
	auto part = std::make_shared<SamplePart>(engine);
	engine.start(part);

	return 0;
}
