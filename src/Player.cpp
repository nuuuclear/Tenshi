#include "Tenshi/Player.h"

#include <SDL3_image/SDL_image.h>

#include <string>

#include "Tenshi/Actor.h"

namespace Tenshi {

Player::Player() {
	// SDL_Renderer* renderer = Angel_GetRenderer();
}

// TODO: move all actor stuff such as the sprite into the player actor.
//		 Let the game engine handle the player, this code.
//		 Make this controll the actor seperatly so that the player class is more 'vague'
//			so it can do more than just controll an actor, and is more configurable.

void Player::init() {

}

void Player::put(double _x, double _y) {
	actor->put(_x, _y);
}

// inputs
void Player::i_move(bool left, bool right) {
	actor->i_move(left, right);
}

void Player::i_jump(bool jump) {
	actor->i_jump(jump);
}

void Player::setActor(Actor* act) {
	actor = act;
}

} // namespace Tenshi