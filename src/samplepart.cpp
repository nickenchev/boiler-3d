#include <fstream>
#include <sstream>
#include <unordered_map>

#include <glm/glm.hpp>
#include "SDL_keycode.h"
#include "input/inputevent.h"
#include "samplepart.h"
#include "core/entitycomponentsystem.h"
#include "video/vertexdata.h"
#include "core/components/rendercomponent.h"
#include "core/components/positioncomponent.h"
#include "core/components/spritecomponent.h"

using namespace Boiler;

auto SamplePart::loadPrimitive(const gltf::ModelAccessors &modelAccess, const gltf::Primitive &primitive)
{
	EntityComponentSystem &ecs = engine.getEcs();
	using namespace gltf::attributes;

	// get the primitive's position data
	std::vector<Vertex> vertices;
	auto positionAccess = modelAccess.getTypedAccessor<float, 3>(primitive, POSITION);
	for (auto values : positionAccess)
	{
		Vertex vertex({values[0], -values[1], values[2]});
		vertex.colour = {1, 1, 1, 1};
		vertices.push_back(vertex);
	}

	auto normalAccess = modelAccess.getTypedAccessor<float, 3>(primitive, NORMAL);
	unsigned int vertexIndex = 0;
	for (auto normal : normalAccess)
	{
		vertices[vertexIndex++].normal = {normal[0], normal[1], normal[2]};
	}

	std::vector<uint32_t> indices;
	auto indexAccess = modelAccess.getTypedAccessor<unsigned short, 1>(primitive, primitive.indices.value());
	for (auto values : indexAccess)
	{
		indices.push_back(values[0]);
	}

	const VertexData vertData(vertices, indices);
	return engine.getRenderer().loadModel(vertData);
}

Entity SamplePart::loadNode(const gltf::Model &model, const gltf::ModelAccessors &modelAccess, std::unordered_map<int, Entity> &nodeEntities, int nodeIndex)
{
	SpriteSheetFrame sheetFrame(tex, nullptr);

	EntityComponentSystem &ecs = engine.getEcs();
	const auto &node = model.nodes[nodeIndex];

	Entity nodeEntity = ecs.newEntity();
	objects.push_back(nodeEntity);
	nodeEntities[nodeIndex] = nodeEntity;

	logger.log("Loading node: {} ({}), as entity #{}", node.name, nodeIndex, nodeEntity.getId());

	// treat node's children
	for (int childNodeIdx : node.children)
	{
		// check if this entity hasn't been processed
		Entity childEntity;
		if (nodeEntities.find(childNodeIdx) == nodeEntities.end())
		{
			childEntity = loadNode(model, modelAccess, nodeEntities, childNodeIdx);
		}
		else
		{
			childEntity = nodeEntities[childNodeIdx];
		}
		ecs.createComponent<ParentComponent>(childEntity, nodeEntity);
	}

	// if node has a mesh, ensure it is loaded
	if (node.mesh)
	{
		const auto &mesh = model.meshes[node.mesh.value()];

		logger.log("Loading mesh with name: {}", mesh.name);
		for (auto &primitive : mesh.primitives)
		{
			auto renderComp = ecs.createComponent<RenderComponent>(nodeEntity, loadPrimitive(modelAccess, primitive), sheetFrame);
		}
	}

	// ensure each node has a transformation component
	auto renderPos = ecs.createComponent<PositionComponent>(nodeEntity, Rect(0, 0, 0, 0));
	if (node.scale.has_value())
	{
		renderPos->scale = {
			node.scale.value()[0],
			node.scale.value()[1],
			node.scale.value()[2]
		};
	}
	if (node.translation.has_value())
	{
		renderPos->frame.position = {
			node.translation.value()[0],
			-node.translation.value()[1],
			node.translation.value()[2]
		};
	}
	renderPos->rotationAxis = glm::vec3(0, 1, 0);
	return nodeEntity;
}

SamplePart::SamplePart(Engine &engine) : Part("Sample", engine), logger("Sample Part")
{
	moveLeft = false;
	moveRight = false;
	moveCloser = false;
	moveFurther = false;
	moveUp = false;
	moveDown = false;

	camPos = {0, 0, 10.0f};
	direction ={0, 0, -1.0f};
}

void SamplePart::onStart()
{
	tex = engine.getImageLoader().loadImage("data/test.png");
	SpriteSheetFrame sheetFrame(tex, nullptr);

	engine.getRenderer().setClearColor({0, 0, 0});

	// load GLTF file
	//std::string gltfFile{"data/blender_box.gltf"};
	//std::string gltfFile{"data/monkey.gltf"};
	//std::string gltfFile{"data/multi_test.gltf"};
	//std::string gltfFile{"data/parent.gltf"};
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

	const Boiler::gltf::Scene &scene = model.scenes[model.scene];
	std::unordered_map<int, Entity> entityMap;
	for (auto &nodeIndex : scene.nodes)
	{
		loadNode(model, modelAccess, entityMap, nodeIndex);
	}

    auto keyListener = [this](const KeyInputEvent &event)
	{
		const int maxTileSize = 100;
		//EntityComponentSystem &ecs = engine.getEcs();

		if (event.keyCode == SDLK_a)
		{
			moveLeft = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_d)
		{
			moveRight = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_s)
		{
			moveCloser = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_w)
		{
			moveFurther = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_PAGEUP)
		{
			moveUp = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_PAGEDOWN)
		{
			moveDown = event.state == ButtonState::DOWN;
		}
	};
	engine.addKeyInputListener(keyListener);
}

void SamplePart::update(double deltaTime)
{
	glm::mat4 view = glm::lookAt(camPos, camPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
	engine.getRenderer().setViewMatrix(view);

	EntityComponentSystem &ecs = engine.getEcs();
	if (moveLeft)
	{
		camPos.x -= 2.0f * deltaTime;
	}
	else if (moveRight)
	{
		camPos.x += 2.0f * deltaTime;
	}

	if (moveCloser)
	{
		camPos.z += 2.0f * deltaTime;
	}
	else if (moveFurther)
	{
		camPos.z -= 2.0f * deltaTime;
	}

	if (moveUp)
	{
		camPos.y -= 2.0f * deltaTime;
	}
	else if (moveDown)
	{
		camPos.y += 2.0f * deltaTime;
	}
}
