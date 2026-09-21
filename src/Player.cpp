#include "Tenshi/Player.h"

#include "Tenshi/Actor.h"

namespace Tenshi {

Player::Player() {

}

void Player::init() {

}

void Player::put(double _x, double _y) {
	if (actor == nullptr) return;

	actor->put(_x, _y);
}

// inputs
void Player::i_move(bool left, bool right) {
	if (actor == nullptr) return;

	actor->i_move(left, right);
}

void Player::i_jump(bool jump) {
	if (actor == nullptr) return;

	actor->i_jump(jump);
}

void Player::setActor(Actor* actor) {
	this->actor = actor;
}

Actor* Player::getActor() const {
	return actor;
}

} // namespace Tenshi