#include <memory>
#include <boiler.h>
#include <video/vulkanrenderer.h>
#include "samplepart.h"
#include <video/sdlwindow.h>

int main(int argc, char *argv[])
{
	std::unique_ptr<Boiler::Renderer> renderer = std::make_unique<Boiler::VulkanRenderer>();
    
	Boiler::Engine engine(*renderer);
	engine.initialize(1024, 768);
	auto part = std::make_shared<SamplePart>();
	engine.start(part);

	return 0;
}
