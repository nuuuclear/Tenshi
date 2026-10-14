#pragma once

#include <vector>
#include <memory>

#include "Tenshi/Scene/Background.h"
#include "Tenshi/Graphics/Tileset.h"
#include "Tenshi/Graphics/Tilemap.h"
#include "Tenshi/Scene/Components.h"
#include "Tenshi/Entity/World.h"

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

	Entity createCharacter(Sprite* sprite, double x = 0.0, double y = 0.0);
	void setCameraTarget(Entity entity);
	Entity getCameraTarget() const;
	World& getWorld();
	const World& getWorld() const;

	std::unique_ptr<Background>& getBackground();
	std::unique_ptr<Tileset>& getTileset();
	std::unique_ptr<Tilemap>& getTilemap();
	std::vector<Rect<double>>& getColliders();
	std::unique_ptr<Camera>& getCamera();
private:
	std::unique_ptr<SpriteBatch> batch;

	std::unique_ptr<Tileset> tileset;
	std::unique_ptr<Tilemap> tilemap;
	std::unique_ptr<Background> background;
	std::vector<Rect<double>> worldColliders;
	World world;

	// TODO: make this a vector when scenes support multiple viewports
	std::unique_ptr<Camera> camera;
};

} // namespace Tenshi