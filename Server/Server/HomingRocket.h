#pragma once
#include "Object.h"


namespace rocket {

    // Rocket targeting parameters
    const int binSearchPrecision = 20;
    const double targetingRadius = 6.0;
    const double triggerRadiusConst = 0.6;
    // Explosion param
    namespace explosion {
        const bool isMove = 0;
        const double radius = 2.0;
        const double power = 5.0;
        const double time = 0.1;
    }

    //Also I had to include a sign func which i couldn't find;

    // Tells the rocket to go sideways
    // Use if the rocket moves too fast to turn quickly enough, for example, if sniping from large distances at enemies close to walls
    double naiveAccelDir(Object rocket, Object target, double accel);

    // Tells Rocket the optimal Acceleration Direction, takes objects and the rocket acceleration(i couldn't find it)
    double accelDir(Object rocket, Object target, double accel);

    // Inner wrapping of the previous func, now the rocket's coordinate system is used
    double calcDir(Vec2 targetPos, Vec2 targetVel, double accel);


    // Updates rocket dir if the rocket dir is close to the optimum
    double updateDir(Object rocket, Object target, double accel, double rocketDir);

    // Analogous inner wrapping for the updater
    double calcUpdateDir(Vec2 targetPos, Vec2 targetVel, double accel, double rocketDir);


    // Function with zeroes that correspond to hitting the target;
    // Its parameters: target.pos, target.vel, time to reach the target
    double homingFunc(Vec2 pos, Vec2 vel, double time, double accel);
    double homingFuncDeriv(Vec2 pos, Vec2 vel, double time, double accel);

    // Tells if the target lies in range of the rocket
    bool isInRange(Object rocket, Object target);
};