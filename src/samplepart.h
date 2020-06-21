#ifndef SAMPLEPART_H
#define SAMPLEPART_H

#include <memory>
#include "boiler.h"
#include "gltf.h"
#include "typedaccessor.h"
#include "modelaccessors.h"

class SamplePart : public Boiler::Part
{
	Boiler::Logger logger;
	std::vector<Boiler::Entity> objects;

	glm::vec3 camPos{0, 0, 10.0f};
	glm::vec3 direction{0, 0, -1.0f};

	bool moveLeft, moveRight, moveCloser, moveFurther, moveUp, moveDown;
	std::shared_ptr<const Boiler::Texture> tex;

	auto loadPrimitive(const Boiler::gltf::ModelAccessors &modelAccess, const Boiler::gltf::Primitive &primitive);
	Boiler::Entity loadNode(const Boiler::gltf::Model &model, const Boiler::gltf::ModelAccessors &modelAccess, std::unordered_map<int, Boiler::Entity> &nodeEntities, int nodeIndex);
	
public:
    SamplePart(Boiler::Engine &engine);

    void onStart() override;
    void update(double deltaTime) override;
};

#endif /* SAMPLEPART_H */
