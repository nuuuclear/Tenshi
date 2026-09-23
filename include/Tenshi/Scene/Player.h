#pragma once

namespace Tenshi {

class Sprite;
class Actor;

enum FACING_DIRECTION {
	FD_LEFT,
	FD_RIGHT
};

class Player {
public:
	Player();

	void init();

	void put(double _x, double _y);

	// inputs
	void i_move(bool left, bool right);
	void i_jump(bool jump);
	void i_dash(bool dash);

	// getters
	Actor* getActor() const;

	// setters
	void setActor(Actor* actor);

private:
	Actor* actor = nullptr;
};

} // namespace Tenshi