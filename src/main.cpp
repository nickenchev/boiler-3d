#include <memory>
#include <boiler.h>
#include <video/vulkanrenderer.h>
#include "samplepart.h"

int main(int argc, char *argv[])
{
	Boiler::Engine::getInstance().initialize(std::make_unique<Boiler::VulkanRenderer>(), 1024, 768);

	//auto part = std::make_shared<SamplePart>();
	//Boiler::Engine::getInstance().start(part);

	return 0;
}
