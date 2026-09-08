#pragma once

#include <stdint.h>
#include <vector>
#include <set>

namespace Tenshi {

// Entity type (a preset) for the componatant composer
enum EntityType {
	ENT_NIL,
	ENT_CHARACTER,
	ENT_PROJECTILE
};

struct EntityHandle {
	uint32_t index, id;
};

struct Entity {
	EntityHandle handle;
	EntityType type;
};

// TODO: determine this later, it will need to higher.
constexpr uint32_t MAX_ENTITIES = 1028;

// entity system, usually in a stage
class EntitySystem {
public:
	void Update();
	void Draw();

	Entity GetEntity();

	void Attach();
private:
	Entity entities[MAX_ENTITIES];
};

Entity MakeEntity(EntityHandle handle);
Entity SpawnEntity(EntitySystem system, EntityType type);
Entity PutEntity(EntityHandle handle);

// will probably be identical to system->GetEntity();
Entity GetEntity(EntitySystem system, EntityHandle handle);

} // namespace Tenshi