#include <memory>
#include <boiler.h>
#include <video/vulkanrenderer.h>
#include "samplepart.h"
#include <video/sdlwindow.h>

int main(int argc, char *argv[])
{
	Boiler::VulkanRenderer renderer;
	Boiler::Engine engine(renderer);

	engine.initialize(1024, 768);

	auto part = std::make_shared<SamplePart>();
	engine.start(part);

	return 0;
}
