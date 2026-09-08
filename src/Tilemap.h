#pragma once

#include <stdint.h>
#include <vector>

namespace Tenshi {

class Game;
class Camera;
class SpriteBatch;
class Tileset;

class Tilemap {
public:
	Tilemap(Game* game, int w, int h);
	~Tilemap();

	bool LoadFromData(const char* dataPath);

	void Step(double deltaTime);
	void Draw(const Camera& cam);

	void SetTileset(Tileset* ts);
private:
	SpriteBatch* batch;
	Tileset* tileset;

	int width, height;

	std::vector<int> map; // Tile data that would usually be loaded from disc

	uint8_t chunkSize = 32; // e.g. 32x32 tiles
};

} // namespace Tenshi