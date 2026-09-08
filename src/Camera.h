#pragma once
#include <vector>

namespace Tenshi {

class Actor;

struct CameraEffect {
    float time;
    float dur;
    float freq;
    float mag;
    float seed;
};

class Camera {
public:
    double x = 0; 
    double y = 0;

    int viewW = 320;
    int viewH = 180;

    int worldW = 2000;
    int worldH = 1000;

    double lookAheadX = 40;
    double lookAheadY = 20;

    double lookAheadDistX = 40;
    double lookAheadDistY = 20;

    double deadzoneW = 80;
    double deadzoneH = 60;

    double lerpSpeed = 0.12;

    Actor* followedActor;

    void update(double dt);

    int getViewX() const;
    int getViewY() const;
};

} // namespace Tenshi