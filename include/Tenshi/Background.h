#pragma once

#include "SpriteBatch.h"
#include "Sprite.h"
#include "Camera.h"

#include <vector>
#include <stdint.h>
#include <string>

namespace Tenshi {

class BackgroundLayer {
public:
	BackgroundLayer(Sprite* spr, std::string hndl);
	~BackgroundLayer();

	void Step();
	void Position(double x, double y);

	void SetDepth(uint16_t d);
	void SetSpeed(float speedX, float speedY);

	double GetX() const;
	double GetY() const;

	Sprite* GetSprite();
	uint16_t GetDepth();
	float GetSpeedX();
	float GetSpeedY();

	std::string handle;

	bool horizontalTile, verticalTile;
private:
	Sprite* sprite;

	uint16_t depth; // parallax depth, also used to sort draw order.

	double x, y;

	float scrollSpeedX, scrollSpeedY;
};

class Background {
public:
	Background(SDL_Renderer* r);
	~Background();

	void Step(const Camera& cam);
	void Draw(const Camera& cam);

	void AddLayer(BackgroundLayer* layer);
	void RemoveLayer(std::string handle);
	void SortLayersDepth();

	bool ParallaxEnabled;
private:
	SDL_Renderer* renderer;

	std::vector<BackgroundLayer*> layers;
};

} // namespace Tenshi