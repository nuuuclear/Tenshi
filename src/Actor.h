#pragma once

#include <stdint.h>
#include <vector>

#include "Collider.h"

namespace Tenshi {

class Sprite;
class SpriteBatch;
class Camera;

class Actor {
public:
    Actor();

    Collider collidesWith;

    void init();

    void put(double _x, double _y);
    void animate();
    void draw(SpriteBatch& batch, const Camera& camera);
    void act(double dt, const std::vector<Rect>& world);

    void show();
    void halt();

    void i_move(bool left, bool right);
    void i_jump(bool jump);
    void i_dash(bool dash);

    Sprite* getSprite();
    Collider getCollider();

    void getPosition();

    int getCentreX();
    int getCentreY();

    double getVelocityX();
    double getVelocityY();

    void setPosition(double _x, double _y);
    void setDirection(uint32_t direction);

    void setVelocityX(double velx);
    void setVelocityY(double vely);

    void setSprite(Sprite* spr);

private:
    int state;

    bool alive;

    int life;
    int life_max;

    int air;

    int equip;
    int unit;
    uint32_t direction;

    Sprite* sprite;
    bool visible;

    int facingDirection;

    Collider feet;

    double x;
    double y;

    int draw_x;
    int draw_y;

    int xm;
    int ym;

    double velocityX;
    double velocityY;

    double speed_walk;
    double speed_run;
    double speed_boost;

    double speed_jump;

    double dash_linear;
    double dash_diagonal;

    double gravity;
    double termvel;

    double resist;
};

} // namespace Tenshi