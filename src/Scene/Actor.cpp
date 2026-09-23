#include "Tenshi/Scene/Actor.h"

#include "Tenshi/Graphics/Sprite.h"
#include "Tenshi/Core/Log.h"

namespace Tenshi {

Actor::Actor()
	: sprite(nullptr)
{
}

void Actor::init() {
	if (!sprite) {
		LogError(
            LogCategory::Resource,
            "Actor initialized with out a sprite"
        );
        return;
    }

	x = 0;
	y = 0;
	xm = 0;
	ym = 0;
	velocityX = 0;
	velocityY = 0;

	gravity = 260;
	termvel = 44;

	resist = 2;
	
	speed_walk = 120;
	speed_jump = 30;

	visible = false;

	life = 100;
	life_max = 100;

	SpriteBlock& idle = sprite->createBlock("idle");

	SpriteFrame f1;
	f1.images = { 2, 4 };
	f1.chance = { 0.9f, 0.1f };

	SpriteFrame f2;
	f2.images = { 3, 5 };
	f2.chance = { 0.9f, 0.1f };

	idle.addFrame(f1);
	idle.addFrame(f2);
	idle.setSpeed(3.0f);

	sprite->setBlock("idle");

	SpriteBlock& walk = sprite->createBlock("walk");

	SpriteFrame f11;
	f11.images = { 0, };
	f11.chance = { 1.0f };

	SpriteFrame f22;
	f22.images = { 2 };
	f22.chance = { 1.0f };

	SpriteFrame f3;
	f3.images = { 1 };
	f3.chance = { 1.0f };

	SpriteFrame f4;
	f4.images = { 2, };
	f4.chance = { 1.0f };

	walk.addFrame(f11);
	walk.addFrame(f22);
	walk.addFrame(f3);
	walk.addFrame(f4);
	walk.setSpeed(5.0f);

	feet.w = 6;
	feet.h = 8;

	feet.offsetX = (sprite->getWidth() / 2) - 3;
	feet.offsetY = sprite->getHeight() - 8;
}

void Actor::put(double _x, double _y) {
	visible = true;

	xm = 0;
	ym = 0;
	velocityX = 0;
	velocityY = 0;

	setPosition(_x, _y);
}

void Actor::act(double dt, const std::vector<Rect_d>& world) {
	velocityX = speed_walk * xm;

	velocityY += gravity * dt;
	if (velocityY > termvel)
		velocityY = termvel;

	double moveX = velocityX * dt;
	double moveY = velocityY * dt;

	// horizontal
	x += moveX;

	Rect_d box = feet.getBounds(x, y);

	for (const Rect_d& col : world) {
		if (Collider::intersects(box, col)) {
			if (moveX > 0)
				x = col.x - feet.offsetX - feet.w;
			else
				x = col.x + col.w - feet.offsetX;

			velocityX = 0;
		}
	}

	// vertical
	y += moveY;

	box = feet.getBounds(x, y);

	for (const Rect_d& col : world) {
		if (Collider::intersects(box, col)) {
			if (moveY > 0)
				y = col.y - feet.offsetY - feet.h;
			else
				y = col.y + col.h - feet.offsetY;

			velocityY = 0;
		}
	}

	if (sprite != nullptr) {
		if (xm == 1 || xm == -1) {
			sprite->setBlock("walk");
		}
		else {
			sprite->setBlock("idle");
		}

		sprite->step(dt);
	}
}

void Actor::draw(SpriteBatch& batch, const Camera& camera) {
	if (sprite == nullptr) return; 

	if (facingDirection == -1) {
		sprite->setFlip(SDL_FLIP_HORIZONTAL);
	}
	else if (facingDirection == 1) {
		sprite->setFlip(SDL_FLIP_NONE);
	}

	sprite->draw(batch, camera, x, y);
}

// inputs
void Actor::i_move(bool left, bool right) {
	xm = right - left;

	facingDirection = xm;
}

void Actor::i_jump(bool jump) {
	velocityY -= speed_jump * jump;
}

// setters
void Actor::setPosition(double _x, double _y) {
	x = _x;
	y = _y;
}

void Actor::setVelocityX(double velx) {
	velocityX = velx;
}

void Actor::setVelocityY(double vely) {
	velocityY = vely;
}

void Actor::setSprite(Sprite* spr) {
	sprite = spr;
}

// getters
Sprite* Actor::getSprite() {
	return sprite;
}

double Actor::getVelocityX() {
	return velocityX;
}

double Actor::getVelocityY() {
	return velocityY;
}

int Actor::getCentreX() {
	return x + sprite->getWidth() / (double)2;
}

int Actor::getCentreY() {
	return y + sprite->getHeight() / (double)2;
}

} // namespace Tenshi
