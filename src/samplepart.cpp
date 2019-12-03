#include <array>
#include <glm/glm.hpp>
#include "SDL_keycode.h"
#include "input/inputevent.h"
#include "samplepart.h"
#include "core/entitycomponentsystem.h"
#include "video/vertexdata.h"
#include "core/components/rendercomponent.h"
#include "core/components/positioncomponent.h"
#include "core/components/spritecomponent.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "external/tiny_obj_loader.h"

using namespace Boiler;

SamplePart::SamplePart() : Part("Sample")
{
	turnLeft = false;
	turnRight = false;
	moveCloser = false;
	moveFurther = false;
}

void SamplePart::onStart(Engine &engine)
{
	using namespace tinyobj;
	attrib_t attrib;
	std::vector<shape_t> shapes;
	std::vector<material_t> materials;
	std::string warning, error;
	Logger logger("Samplepart");
	
	if (!LoadObj(&attrib, &shapes, &materials, &warning, &error, "data/chalet.obj"))
	{
		throw std::runtime_error(warning + error);
	}

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	logger.log("attrib: {}", attrib.GetVertices().size());

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};
			vertex.position = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.textureCoordinate = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.colour = {1.0f, 1.0f, 1.0f, 1.0f};
			vertices.push_back(vertex);
			indices.push_back(indices.size());
		}
	}
	logger.log("Verts: {}, Indices: {}", vertices.size(), indices.size());

	tex = engine.getImageLoader().loadImage("data/chalet.png");
	SpriteSheetFrame sheetFrame(tex, nullptr);

	this->engine = &engine;
	engine.getRenderer().setClearColor({0, 0, 0});

	VertexData vertData(vertices, indices);
	EntityComponentSystem &ecs = engine.getEcs();
	object = ecs.newEntity();
	auto renderComp = ecs.createComponent<RenderComponent>(object, engine.getRenderer().loadModel(vertData), sheetFrame);
	renderComp->colour = Colour::fromRGBA(252, 171, 20, 255);
	auto renderPos = ecs.createComponent<PositionComponent>(object, Rect(0, 0, 0, 0));
	renderPos->scale *= 1;
	renderPos->rotationAxis = glm::vec3(1, 1, 0);
	renderPos->rotationAngle = 90;

    auto keyListener = [this, &engine](const KeyInputEvent &event)
	{
		const int maxTileSize = 100;
		EntityComponentSystem &ecs = engine.getEcs();

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
	EntityComponentSystem &ecs = engine->getEcs();
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
		pos.frame.position.z += 2.0f * deltaTime;
	}
	else if (moveFurther)
	{
		pos.frame.position.z -= 2.0f * deltaTime;
	}
}
