#include <memory>
#include <boiler.h>
#include <video/vulkanrenderer.h>
#include <video/openglrenderer.h>
#include "samplepart.h"
#include <video/sdlwindow.h>

int main(int argc, char *argv[])
{
	std::unique_ptr<Boiler::Renderer> renderer = std::make_unique<Boiler::VulkanRenderer>();
	//std::unique_ptr<Boiler::Renderer> renderer = std::make_unique<Boiler::OpenGLRenderer>(false);
    
	Boiler::Engine engine(*renderer);
	engine.initialize(1024, 768);

	// create the part and start it
	auto part = std::make_shared<SamplePart>();
	engine.start(part);

	return 0;
}
