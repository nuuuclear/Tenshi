#pragma once

#include <vector>
#include <memory>

#include "Actor.h"
#include "Background.h"
#include "Tileset.h"
#include "Tilemap.h"

namespace Tenshi {

class Game;

class Scene {
public:
	Scene(Game* game);
	~Scene();

	void init();
	void Step(double dt);
	void Draw();

	void setBackground(std::unique_ptr<Background> bg);
	void setTileset(std::unique_ptr<Tileset> ts);
	void setTilemap(std::unique_ptr<Tilemap> map);

	Actor* pushActor(std::unique_ptr<Actor> actor);

	std::unique_ptr<Background>& getBackground();
	std::unique_ptr<Tileset>& getTileset();
	std::unique_ptr<Tilemap>& getTilemap();
	std::vector<Rect>& getColliders();
	std::vector<std::unique_ptr<Actor>>& getActors();

	std::unique_ptr<Camera>& getCamera();
private:
	std::unique_ptr<SpriteBatch> batch;

	std::unique_ptr<Tileset> tileset;
	std::unique_ptr<Tilemap> tilemap;
	std::unique_ptr<Background> background;
	std::vector<Rect> worldColliders;
	std::vector<std::unique_ptr<Actor>> actors;

	// TODO: make this a vector, the viewport can have a pointer to one of them.
	std::unique_ptr<Camera> camera;
};

} // namespace Tenshi