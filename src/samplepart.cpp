#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include "SDL_keycode.h"
#include "input/inputevent.h"
#include "samplepart.h"
#include "core/entitycomponentsystem.h"
#include "video/vertexdata.h"
#include "core/components/rendercomponent.h"
#include "core/components/positioncomponent.h"
#include "core/components/spritecomponent.h"

#include "gltf.h"
#include "typedaccessor.h"
#include "modelaccessors.h"

using namespace Boiler;

SamplePart::SamplePart(Engine &engine) : Part("Sample", engine)
{
	turnLeft = false;
	turnRight = false;
	moveCloser = false;
	moveFurther = false;
}

void SamplePart::onStart()
{
	Logger logger("Samplepart");

	tex = engine.getImageLoader().loadImage("data/test.png");
	SpriteSheetFrame sheetFrame(tex, nullptr);

	engine.getRenderer().setClearColor({0, 0, 0});

	// load GLTF file
	//std::string gltfFile{"data/blender_box.gltf"};
	//std::string gltfFile{"data/monkey.gltf"};
	std::string gltfFile{"data/donut.gltf"};
	//std::string gltfFile{"data/Box.gltf"};
	//std::string gltfFile{"data/lantern/Lantern.gltf"};
	//std::string gltfFile{"data/littlest_tokyo/scene.gltf"};
	//std::string gltfFile{"data/glTF-Sample-Models-master/2.0/AnimatedMorphCube/glTF/AnimatedMorphCube.gltf"};
	EntityComponentSystem &ecs = engine.getEcs();
    
	std::ifstream infile(gltfFile);
	std::stringstream buffer;
	buffer << infile.rdbuf();
	const std::string jsonString = buffer.str();
	infile.close();

	auto model = Boiler::gltf::load(jsonString);

	std::vector<std::vector<std::byte>> buffers;
	for (const auto &buffer : model.buffers)
	{
		buffers.push_back(loadBuffer("data/", buffer));
	}
	gltf::ModelAccessors modelAccess(model, std::move(buffers));

	for (auto &mesh : model.meshes)
	{
		for (auto &primitive : mesh.primitives)
		{
			using namespace gltf::attributes;

			// get the primitive's position data
			std::vector<Vertex> vertices;
			auto positionAccess = modelAccess.getTypedAccessor<float, 3>(primitive, POSITION);
			float colour = 0;
			for (auto values : positionAccess)
			{
				Vertex vertex({values[0], -values[1], values[2]});
				vertex.colour = {colour, colour, colour, 1};
				colour += 0.1;
				if (colour > 1)
				{
					colour = 0;
				}
				vertices.push_back(vertex);
			}

			std::vector<uint32_t> indices;
			auto indexAccess = modelAccess.getTypedAccessor<unsigned short, 1>(primitive, primitive.indices.value());
			for (auto values : indexAccess)
			{
				indices.push_back(values[0]);
			}

			const VertexData vertData(vertices, indices);
			auto model = engine.getRenderer().loadModel(vertData);
			
			Entity primitiveEntity = ecs.newEntity();
			auto renderComp = ecs.createComponent<RenderComponent>(primitiveEntity, model, sheetFrame);
			auto renderPos = ecs.createComponent<PositionComponent>(primitiveEntity, Rect(0, 0, 0, 0));
			renderPos->rotationAxis = glm::vec3(0, 1, 0);
			//renderPos->rotationAngle = 90;
			this->object = primitiveEntity;
		}
	}

    auto keyListener = [this](const KeyInputEvent &event)
	{
		const int maxTileSize = 100;
		//EntityComponentSystem &ecs = engine.getEcs();

		if (event.state == ButtonState::DOWN)
		{
			if (event.keyCode == SDLK_a)
			{
				turnLeft = true;;
			}
			else if (event.keyCode == SDLK_d)
			{
				turnRight = true;;
			}
			else if (event.keyCode == SDLK_s)
			{
				moveCloser = true;
			}
			else if (event.keyCode == SDLK_w)
			{
				moveFurther = true;
			}
		}
		else if (event.state == ButtonState::UP)
		{
			if (event.keyCode == SDLK_a)
			{
				turnLeft = false;
			}
			else if (event.keyCode == SDLK_d)
			{
				turnRight = false;
			}
			else if (event.keyCode == SDLK_s)
			{
				moveCloser = false;
			}
			else if (event.keyCode == SDLK_w)
			{
				moveFurther = false;
			}
		}
	};
	engine.addKeyInputListener(keyListener);
}

void SamplePart::update(double deltaTime)
{
	EntityComponentSystem &ecs = engine.getEcs();
	PositionComponent &pos = ecs.getComponentStore().retrieve<PositionComponent>(object);
	if (turnLeft)
	{
		pos.rotationAngle -= 35.0f * deltaTime;
	}
	else if (turnRight)
	{
		pos.rotationAngle += 35.0f * deltaTime;
	}

	if (moveCloser)
	{
		pos.frame.position.z -= 2.0f * deltaTime;
	}
	else if (moveFurther)
	{
		pos.frame.position.z += 2.0f * deltaTime;
	}
}
