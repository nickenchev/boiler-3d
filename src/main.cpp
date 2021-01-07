#include <memory>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_vulkan.h>

#include <boiler.h>
#include <video/vulkanrenderer.h>

#include "samplepart.h"

int main(int argc, char *argv[])
{
	bool enableDebug = false;
	if (argc > 1)
	{
		if (std::strcmp(argv[1], "-debug") == 0)
		{
			enableDebug = true;
		}
	}
	Boiler::Size initialSize(1280, 720);

	// create an SDL window
	SDL_Window *win = nullptr;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0)
	{
		SDL_WindowFlags winFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI |
													 SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);

        win = SDL_CreateWindow("Boiler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               initialSize.width, initialSize.height, winFlags);
		if (!win)
		{
			throw std::runtime_error("Unable to create main window");
		}
	}
	else
    {
		throw std::runtime_error("Error Initializing SDL: " + std::string(SDL_GetError()));
    }
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Vulkan extensions required by SDL
	unsigned int extCount = 0;
	SDL_Vulkan_GetInstanceExtensions(win, &extCount, nullptr);
	std::vector<const char *> extensions;
	extensions.resize(extCount);
	SDL_Vulkan_GetInstanceExtensions(win, &extCount, extensions.data());

	// create the renderer and surface
	auto renderer = std::make_unique<Boiler::Vulkan::VulkanRenderer>(extensions, enableDebug);
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	SDL_Vulkan_CreateSurface(win, renderer->getVulkanInstance(), &surface);
	renderer->setSurface(surface);

	// setup the engine itself
	static Boiler::Engine engine(renderer.get());
	engine.initialize(initialSize);

	// create the part and start it
	auto part = std::make_shared<SamplePart>(engine);
	engine.start(part);

	SDL_DestroyWindow(win);

	return 0;
}
