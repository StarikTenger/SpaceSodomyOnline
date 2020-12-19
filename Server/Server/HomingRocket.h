#pragma once
#include "System.h"
#include "Object.h"


namespace rocket {


    //Also I had to include a sign func which i couldn't find;

    // Tells the rocket to go sideways
    // Use if the rocket moves too fast to turn quickly enough, for example, if sniping from large distances at enemies close to walls
    double naiveAccelDir(Object rocket, Object target, double accel, Parameters& parameters);

    // Tells Rocket the optimal Acceleration Direction, takes objects and the rocket acceleration(i couldn't find it)
    double accelDir(Object rocket, Object target, double accel, Parameters& parameters);

    // Inner wrapping of the previous func, now the rocket's coordinate system is used
    double calcDir(Vec2 targetPos, Vec2 targetVel, double accel, Parameters& parameters);


    // Updates rocket dir if the rocket dir is close to the optimum
    double updateDir(Object rocket, Object target, double accel, double rocketDir, Parameters& parameters);

    // Analogous inner wrapping for the updater
    double calcUpdateDir(Vec2 targetPos, Vec2 targetVel, double accel, double rocketDir, Parameters& parameters);


    // Function with zeroes that correspond to hitting the target;
    // Its parameters: target.pos, target.vel, time to reach the target
    double homingFunc(Vec2 pos, Vec2 vel, double time, double accel);
    double homingFuncDeriv(Vec2 pos, Vec2 vel, double time, double accel);

    // Tells if the target lies in range of the rocket
    bool isInRange(Object rocket, Object target, Parameters& parameters);
};