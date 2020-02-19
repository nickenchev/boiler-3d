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

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

namespace Boiler { namespace gltf {
	constexpr auto noMesh = -1;
}}

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
	this->engine = &engine;
	engine.getRenderer().setClearColor({0, 0, 0});

	using namespace tinyobj;
	attrib_t attrib;
	std::vector<shape_t> shapes;
	std::vector<material_t> materials;
	std::string warning, error;
	Logger logger("Samplepart");

	tex = engine.getImageLoader().loadImage("data/chalet.png");
	SpriteSheetFrame sheetFrame(tex, nullptr);

	// load GLTF file
	using namespace tinygltf;
	//std::string gltfFile{"data/Box.gltf"};
	std::string gltfFile{"data/lantern/Lantern.gltf"};
	//std::string gltfFile{"data/littlest_tokyo/scene.gltf"};
	//std::string gltfFile{"data/glTF-Sample-Models-master/2.0/AnimatedMorphCube/glTF/AnimatedMorphCube.gltf"};
	tinygltf::Model gltfModel;
	TinyGLTF gltfLoader;
	std::string gltfWarn, gltfErr;

	logger.log("Loading GLTF 2 model: {}", gltfFile);
	bool loadRet = gltfLoader.LoadASCIIFromFile(&gltfModel, &gltfErr, &gltfWarn, gltfFile);

	if (!gltfWarn.empty())
	{
		logger.log("GLTF Warning: {}", gltfWarn);
	}
	if (!gltfErr.empty())
	{
		logger.log("GLTF Error: {}", gltfErr);
	}
	if (!loadRet)
	{
		throw std::runtime_error("Error parsing GLTF model");
	}
	else
	{
		logger.log("Loading scene data: {} buffers, {} meshes", gltfModel.buffers.size(), gltfModel.meshes.size());
	}

	EntityComponentSystem &ecs = engine.getEcs();

	// load scene data
	/*
	for (const auto &nodeIndex : gltfModel.scenes[gltfModel.defaultScene].nodes)
	{
		const Node &node = gltfModel.nodes[nodeIndex];
		logger.log("Node mesh: {}", node.mesh);
		if (node.mesh > gltf::noMesh)
		{
			Mesh &mesh = gltfModel.meshes[node.mesh];
			logger.log("Loading mesh: {}", mesh.name);

			for (auto &primitive : mesh.primitives)
			{
				Entity primitiveEntity = ecs.newEntity();
				object = primitiveEntity;

				if (primitive.mode != 4)
				{
					throw std::runtime_error("Only triangle list supported");
				}

				// vertex buffer
				const Accessor &positionAccess = gltfModel.accessors[primitive.attributes["POSITION"]];
				const BufferView &posBufferView = gltfModel.bufferViews[positionAccess.bufferView];
				const Buffer &positionBuffer = gltfModel.buffers[posBufferView.buffer];
				const float *positionData = reinterpret_cast<const float *>(&positionBuffer.data[posBufferView.byteOffset + positionAccess.byteOffset]);

				std::vector<Vertex> vertices;

				vertices.reserve(positionAccess.count);
				for (size_t i = 0; i < positionAccess.count; ++i)
				{
					Vertex vertex;
					vertex.position = {positionData[i * 3], positionData[i * 3 + 1], positionData[i * 3 + 2]};
					vertex.colour = {1, 1, 1, 1};
					vertices.push_back(vertex);
				}

				// index buffer
				const Accessor &indexAccess = gltfModel.accessors[primitive.indices];
				const BufferView &idxBufferView = gltfModel.bufferViews[indexAccess.bufferView];
				const Buffer &indexBuffer = gltfModel.buffers[idxBufferView.buffer];
				const unsigned short *indexData = reinterpret_cast<const unsigned short *>(&indexBuffer.data[idxBufferView.byteOffset + indexAccess.byteOffset]);

				std::vector<uint32_t> indices;
				indices.reserve(indexAccess.count);
				for (size_t i = 0; i < indexAccess.count; ++i)
				{
					indices.push_back(indexData[i]);
				}

				const VertexData vertData(vertices, indices);
				auto renderComp = ecs.createComponent<RenderComponent>(primitiveEntity, engine.getRenderer().loadModel(vertData), sheetFrame);
				auto renderPos = ecs.createComponent<PositionComponent>(primitiveEntity, Rect(0, 0, 0, 0));
				renderPos->rotationAxis = glm::vec3(0, 1, 0);
				renderPos->rotationAngle = 90;
			}
		}
	}
	*/

	/*
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

	VertexData vertData(vertices, indices);
	object = ecs.newEntity();
	auto renderComp = ecs.createComponent<RenderComponent>(object, engine.getRenderer().loadModel(vertData), sheetFrame);
	renderComp->colour = Colour::fromRGBA(252, 171, 20, 255);
	auto renderPos = ecs.createComponent<PositionComponent>(object, Rect(0, 0, 0, 0));
	renderPos->scale *= 1;
	renderPos->rotationAxis = glm::vec3(1, 1, 0);
	renderPos->rotationAngle = 90;
	*/

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
	/*
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
	*/
}
