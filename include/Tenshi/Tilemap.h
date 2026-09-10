#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

#include <SDL3/SDL.h>

namespace Tenshi {

class Game;
class Camera;
class SpriteBatch;
class Tileset;

class Tilemap {
public:
	Tilemap(SDL_Renderer* renderer, int w, int h);
	~Tilemap();

	bool LoadFromData(const char* dataPath);

	void Step(double deltaTime);
	void Draw(const Camera& cam);

	void SetTileset(std::unique_ptr<Tileset> ts);
private:
	std::unique_ptr<SpriteBatch> batch;
	std::unique_ptr<Tileset> tileset;

	int width, height;

	std::vector<int> map; // Tile data that would usually be loaded from disc

	uint8_t chunkSize = 32; // e.g. 32x32 tiles
};

} // namespace Tenshi