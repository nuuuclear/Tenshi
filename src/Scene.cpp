#include "Tenshi/Scene.h"

#include "Tenshi/Game.h"

namespace Tenshi {

Scene::Scene(Game* game) {
	batch = std::make_unique<SpriteBatch>(game->getRenderer());

	camera = std::make_unique<Camera>();

}

Scene::~Scene() {
	// destructors
}


void Scene::init() {
}

void Scene::Step(double dt) {
	background->Step(*camera);

	for (auto& actor : actors) {
		actor->act(dt, worldColliders);
	}

	camera->update(dt);
}

void Scene::Draw() {
	background->Draw(*camera);
	// tilemap->Draw(*camera);

	batch->begin();

	for (auto& actor : actors) {
		actor->draw(*batch, *camera);
	}

	batch->flush();
}

void Scene::setBackground(std::unique_ptr<Background> bg) {
	background = std::move(bg);
}

void Scene::setTileset(std::unique_ptr<Tileset> ts) {
	tileset = std::move(ts);
}

void Scene::setTilemap(std::unique_ptr<Tilemap> map) {
	tilemap = std::move(map);
}

Actor* Scene::pushActor(std::unique_ptr<Actor> actor) {
	Actor* ptr = actor.get();
    actors.push_back(std::move(actor));
	
    return ptr;
}

std::unique_ptr<Background>& Scene::getBackground() {
	return background;
}

std::unique_ptr<Tileset>& Scene::getTileset() {
	return tileset;
}
std::unique_ptr<Tilemap>& Scene::getTilemap() {
	return tilemap;
}

std::vector<Rect>& Scene::getColliders() {
	return worldColliders;
}
std::vector<std::unique_ptr<Actor>>& Scene::getActors() {
	return actors;
}

std::unique_ptr<Camera>& Scene::getCamera() {
	return camera;
}

} // namespace Tenshi