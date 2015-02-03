#include <Nephilim/World/World.h>
#include <Nephilim/World/Landscape.h>

#include <Nephilim/World/Systems/PhysicsSystem.h>
#include <Nephilim/World/Systems/AudioSystem.h>
#include <Nephilim/World/Systems/NetworkSystem.h>

#include <Nephilim/World/Components/AStaticMeshComponent.h>
#include <Nephilim/World/CTransform.h>
#include <Nephilim/World/Components/ASpriteComponent.h>
#include <Nephilim/World/Components/AInputComponent.h>
#include <Nephilim/World/Components/ACameraComponent.h>
#include <Nephilim/World/Components/ABoxComponent.h>

#include <Nephilim/Foundation/Logging.h>
#include <Nephilim/Foundation/Math.h>
#include <Nephilim/Foundation/PluginLoader.h>

NEPHILIM_NS_BEGIN

World::World()
: mEnabled(true)
, mSimulationOnly(false)
{
	// Init some component managers
	createDefaultComponentManager<CTransform>();
	createDefaultComponentManager<AStaticMeshComponent>();
	createDefaultComponentManager<ASpriteComponent>();
	createDefaultComponentManager<ABoxComponent>();
	createDefaultComponentManager<ACameraComponent>();
	createDefaultComponentManager<AInputComponent>();

	Level* defaultLevel = new Level();
	mPersistentLevel = defaultLevel;
	levels.push_back(defaultLevel);

	WorldViewport mainViewport;
	_viewports.push_back(mainViewport);

	createNetworkSystem<NetworkSystem>();
}

/// Called after having the opportunity to set some initial settings
void World::initialize()
{

}

/// Step the world state forward
void World::update(const Time& deltaTime)
{
	for (std::size_t i = 0; i < mRegisteredSystems.size(); ++i)
	{
		mRegisteredSystems[i]->update(deltaTime);
	}

	for (auto a : mPersistentLevel->actors)
	{
		a->update(deltaTime);
	}
}

/// Set the player controller for this world if applicable (clients only)
/// The World _owns_ the object and destroys it when releasing
void World::setPlayerController(PlayerController* playerController)
{
	_playerController.reset(playerController);
	_playerController->_world = this;
}

/// Load a level into memory by its name
/// This requires that the level is previously indexed into this world
bool World::loadLevel(const String& name, bool async)
{
	mPersistentLevel = new Level();
	mPersistentLevel->name = name;
	return false;
}

/// Load directly the new level into this world, and index it too
bool World::loadLevel(Level* level)
{
	return false;
}


/// Unload every level currently active
/// If this world isn't multi level, it will only destroy the current level, if applicable
/// The main use of the function is to revert the world to an empty state before loading the next level
void World::unloadLevels()
{
	// TODO; support multi level
	if (mPersistentLevel)
	{
		delete mPersistentLevel;
		mPersistentLevel = nullptr;
	}
}


/// Create a PhysicsSystem from its name
/// This means the physics system will be instanced via factory (plugin or not)
PhysicsSystem* World::createPhysicsSystem(const String& name)
{
	PhysicsSystem* physicsSystem = nullptr;

	// Prepare the angel script behavior, the dirty way
	typedef PhysicsSystem* (*InstanceLoader_Fn2)();

	// FIX THE LEAK, PLUGINS ARE GAME WIDE
	PluginLoader* physicsPlugin = new PluginLoader(name);
	if (physicsPlugin->isLoaded())
	{
		InstanceLoader_Fn2 f = (InstanceLoader_Fn2)physicsPlugin->getFunctionAddress("createPhysicsSystem");
		if (f)
		{
			physicsSystem = f();
			registerSystem(physicsSystem);
			Log("GOT PHYSICS!");
		}
	}

	return physicsSystem;
}

/// Create a audio system from a plugin or factory
AudioSystem* World::createAudioSystem(const String& name)
{
	AudioSystem* audioSystem = nullptr;

	// Prepare the angel script behavior, the dirty way
	typedef AudioSystem* (*InstanceLoader_Fn2)();

	// FIX THE LEAK, PLUGINS ARE GAME WIDE
	PluginLoader* audioPlugin = new PluginLoader(name);
	if (audioPlugin->isLoaded())
	{
		InstanceLoader_Fn2 f = (InstanceLoader_Fn2)audioPlugin->getFunctionAddress("createAudioSystem");
		if (f)
		{
			audioSystem = f();
			registerSystem(audioSystem);
			Log("GOT AUDIO!");
		}
	}

	return audioSystem;
}

/// Get one of the currently loaded levels
Level* World::getLevel(std::size_t index)
{
	return levels[index];
}

/// Spawns an actor
Actor* World::spawnActor()
{
	Actor* actor = new Actor();
	actor->_world = this;
	mPersistentLevel->actors.push_back(actor);
	return actor;
}

/// Returns the first actor it finds with that name or nullptr
Actor* World::getActorByName(const String& _name)
{
	for (auto actor : mPersistentLevel->actors)
	{
		if (actor->mName == _name)
		{
			//Log("Compare %s with %s", actor->mName.c_str(), _na)
			return actor;
		}
	}
	return nullptr;
}

/// Returns the Actor with the given ID
Actor* World::getActorByUUID(uint32_t _uuid)
{
	for (auto actor : mPersistentLevel->actors)
	{
		if (actor->uuid == _uuid)
		{
			//Log("Compare %s with %s", actor->mName.c_str(), _na)
			return actor;
		}
	}
	return nullptr;
}

/// Get the total numbers of spawned actors
int World::getActorCount()
{
	return mPersistentLevel->actors.size();
}

/// Spawn a vanilla Landscape (not subclassed)
Landscape* World::spawnLandscape()
{
	// Landscapes need to belong to their own levels, they are heavier map objects and need to be culled more efficiently
	if (mPersistentLevel)
	{
		Landscape* land = new Landscape();
		mPersistentLevel->landscapes.push_back(land);
		return land;
	}

	return nullptr;
}

/// Cumulate the transform hierarchy so all model matrices are computed and can be used for rendering
void World::updateTransformHierarchy()
{
	for (std::size_t i = 0; i < mPersistentLevel->actors.size(); ++i)
	{
		if (mPersistentLevel->actors[i]->root != nullptr)
		{

		}
	}
}

/// Registers a system to this scene
void World::registerSystem(System* system)
{
	system->_World = this;
	mRegisteredSystems.push_back(system);
}

/// Create and return a new entity
Entity World::createEntity()
{
	// Create new entity
	Entity entity;
	entity.id = 0;
	entity.id = entityManager.nextAssignment.id++;
	entityManager.entities.push_back(entity);
	return entity;
}

/// Removes the entity if it is in the scene
void World::removeEntity(Entity entity)
{
	entityManager.destroy(entity);

	for (auto it = componentManagers.begin(); it != componentManagers.end(); ++it)
	{
		it->second->removeComponentsFromEntity(entity);
	}
}

Entity World::getEntityByIndex(std::size_t index)
{
	return entityManager.entities[index];
}

void World::destroyActor(Actor* actor)
{
	delete actor;
}

NEPHILIM_NS_END