#include <SDL.h>
#include <SDL_vulkan.h>

#include <memory>
#include <boiler.h>
#include <vector>
#include <video/vulkanrenderer.h>

#include "samplepart.h"

int main(int, char *[])
{
	std::vector<std::string> models {
		"data/parent/parents.gltf",
		"data/glTF-Sample-Models-master/2.0/VC/glTF/VC.gltf",
		"data/glTF-Sample-Models-master/2.0/CesiumMan/glTF/CesiumMan.gltf",
		"data/glTF-Sample-Models-master/2.0/Buggy/glTF/Buggy.gltf",
		"data/glTF-Sample-Models-master/2.0/WaterBottle/glTF/WaterBottle.gltf",
		"data/glTF-Sample-Models-master/2.0/BrainStem/glTF/BrainStem.gltf",
		"data/glTF-Sample-Models-master/2.0/Sponza/glTF/Sponza.gltf",
		"data/littlest_tokyo/glTF/littlest_tokyo.gltf",
		"data/sea_house/scene.gltf",
		"data/glTF-Sample-Models-master/2.0/AnimatedCube/glTF/AnimatedCube.gltf",
		"data/glTF-Sample-Models-master/2.0/AnimatedTriangle/glTF/AnimatedTriangle.gltf",
		"data/glTF-Sample-Models-master/2.0/BoxAnimated/glTF/BoxAnimated.gltf",
		"data/environment_for_firefox_reality/scene.gltf",
		"data/Поляны 13/scene.gltf"
	};

    for (size_t i = 0; i < models.size(); ++i)
	{
		std::cout << i << ") " << models[i] << std::endl;
	}

	int choice = 0;
	std::cin >> choice;
	std::string modelPath = models[choice];

	Boiler::Size initialSize(1280, 720);

	// create an SDL window
	SDL_Window *win = nullptr;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0)
	{
		SDL_WindowFlags winFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI |
													 SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);

        win = SDL_CreateWindow("Boiler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

	// Vulkan extensions required by SDL
	unsigned int extCount = 0;
	SDL_Vulkan_GetInstanceExtensions(win, &extCount, nullptr);
	std::vector<const char *> extensions;
	extensions.resize(extCount);
	SDL_Vulkan_GetInstanceExtensions(win, &extCount, extensions.data());

	// create the renderer and surface
	auto renderer = std::make_unique<Boiler::Vulkan::VulkanRenderer>(extensions);
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	SDL_Vulkan_CreateSurface(win, renderer->getVulkanInstance(), &surface);
	renderer->setSurface(surface);

	// setup the engine itself
	Boiler::Engine engine(renderer.get());
	engine.initialize(initialSize);

	// create the part and start it
	auto part = std::make_shared<SamplePart>(engine, modelPath);
	engine.start(part);

	SDL_DestroyWindow(win);

	return 0;
}
