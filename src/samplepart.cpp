#include <fstream>
#include <sstream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
	const auto &indexAccessor = modelAccess.getModel().accessors[primitive.indices.value()];
	if (indexAccessor.componentType == 5123)
	{
		auto indexAccess = modelAccess.getTypedAccessor<unsigned short, 1>(primitive, primitive.indices.value());
		for (auto values : indexAccess)
		{
			indices.push_back(values[0]);
		}
	}
	else if (indexAccessor.componentType == 5125)
	{
		auto indexAccess = modelAccess.getTypedAccessor<unsigned int, 1>(primitive, primitive.indices.value());
		for (auto values : indexAccess)
		{
			indices.push_back(values[0]);
		}
	}
	

	const VertexData vertData(vertices, indices);
	return engine.getRenderer().loadModel(vertData);
}

Entity SamplePart::loadNode(const gltf::Model &model, const gltf::ModelAccessors &modelAccess, std::unordered_map<int, Entity> &nodeEntities, int nodeIndex)
{
	SpriteSheetFrame sheetFrame(tex, nullptr);
	EntityComponentSystem &ecs = engine.getEcs();
	Entity nodeEntity = ecs.newEntity();

	const gltf::Node &node = model.nodes[nodeIndex];
	logger.log("Loading node: {}", node.name);

	if (node.mesh.has_value())
	{
		const auto &mesh = model.meshes[node.mesh.value()];
		logger.log("Loading mesh: {}", mesh.name);

		auto renderComp = ecs.createComponent<RenderComponent>(nodeEntity);
		for (auto &primitive : mesh.primitives)
		{
			renderComp->meshes.push_back(Mesh(loadPrimitive(modelAccess, primitive), sheetFrame));
		}
	}
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
	if (node.rotation.has_value())
	{
		renderPos->rotation.x = node.rotation.value()[0];
		renderPos->rotation.y = node.rotation.value()[1];
		renderPos->rotation.z = node.rotation.value()[2];
		renderPos->rotation.w = node.rotation.value()[3];
	}

	//renderPos->rotationAxis = glm::vec3(0, 1, 0);

	// if this node has children, create them and assign the current node as parent
	if (node.children.size() > 0)
	{
		for (int childIndex : node.children)
		{
			Entity childEntity = loadNode(model, modelAccess, nodeEntities, childIndex);
			ecs.createComponent<ParentComponent>(childEntity, nodeEntity);
		}
	}

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
	turnLeft = false;
	turnRight = false;
	lookUp = false;
	lookDown = false;

	camPosition = {0, 0, 10.0f};
	camDirection = {0, 0, -1.0f};
	camUp = {0, 1.0f, 0};
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
	//std::string gltfFile{"data/rotations.gltf"};
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

	// load all of the buffers
	std::vector<std::vector<std::byte>> buffers;
	for (const auto &buffer : model.buffers)
	{
		buffers.push_back(loadBuffer("data/", buffer));
	}

	// Model accessors which are used for typed access into buffers
	gltf::ModelAccessors modelAccess(model, std::move(buffers));

	// grab the default scene and load the node heirarchy
	const gltf::Scene &scene = model.scenes[model.scene];
	std::unordered_map<int, Entity> nodeEntities;
	for (auto &nodeIndex : scene.nodes)
	{
		loadNode(model, modelAccess, nodeEntities, nodeIndex);
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
		else if (event.keyCode == SDLK_LEFT)
		{
			turnLeft = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_RIGHT)
		{
			turnRight = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_UP)
		{
			lookUp = event.state == ButtonState::DOWN;
		}
		else if (event.keyCode == SDLK_DOWN)
		{
			lookDown = event.state == ButtonState::DOWN;
		}
	};
	engine.addKeyInputListener(keyListener);
}

void SamplePart::update(double deltaTime)
{
	EntityComponentSystem &ecs = engine.getEcs();
	if (moveLeft)
	{
		glm::vec3 moveAmount = glm::cross(camDirection, camUp);
		moveAmount *= deltaTime;
		camPosition -= moveAmount;
	}
	else if (moveRight)
	{
		glm::vec3 moveAmount = glm::cross(camDirection, camUp);
		moveAmount *= deltaTime;
		camPosition += moveAmount;
	}

	if (moveCloser)
	{
		glm::vec3 moveAmount = camDirection;
		moveAmount *= 3.0f * deltaTime;
		camPosition -= moveAmount;
	}
	else if (moveFurther)
	{
		glm::vec3 moveAmount = camDirection;
		moveAmount *= 3.0f * deltaTime;
		camPosition += moveAmount;
	}

	if (moveUp)
	{
		camPosition.y -= 1.0f * deltaTime;
	}
	else if (moveDown)
	{
		camPosition.y += 1.0f * deltaTime;
	}

	if (lookUp)
	{
		const glm::vec3 axis = glm::cross(camUp, camDirection);
		camDirection = glm::rotate(camDirection, static_cast<float>(2.0f * deltaTime), axis);
		camUp = glm::cross(camDirection, axis);
	}
	else if (lookDown)
	{
		const glm::vec3 axis = glm::cross(camUp, camDirection);
		camDirection = glm::rotate(camDirection, static_cast<float>(-2.0f * deltaTime), axis);
		camUp = glm::cross(camDirection, axis);
	}

	if (turnLeft)
	{
		camDirection = glm::rotate(camDirection, static_cast<float>(1.0f * deltaTime), camUp);
	}
	else if (turnRight)
	{
		camDirection = glm::rotate(camDirection, static_cast<float>(-1.0f * deltaTime), camUp);
	}


	glm::mat4 view = glm::lookAt(camPosition, camPosition + camDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	engine.getRenderer().setViewMatrix(view);

}
