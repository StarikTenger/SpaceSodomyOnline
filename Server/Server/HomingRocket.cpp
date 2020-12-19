#include "HomingRocket.h"
#include <iostream>


double rocket::naiveAccelDir(Object rocket, Object target, double accel, Parameters& parameters) {
    return (geom::dir(target.vel - rocket.vel) + M_PI / 2) * std::signbit((geom::rotate(target.vel - rocket.vel, M_PI / 2) * (target.pos - rocket.pos)));
}

double rocket::accelDir(Object rocket, Object target, double accel, Parameters& parameters) {
    return rocket::calcDir(target.pos - rocket.pos, target.vel - rocket.vel, accel, parameters);
}

double rocket::calcDir(Vec2 targetPos, Vec2 targetVel, double accel, Parameters& parameters) {

    double timeLow = 0, timeHigh = 1. + 2 * sqrt(4 * (targetVel * targetVel) / (accel * accel));

    while ( homingFunc(targetPos, targetVel, timeHigh, accel) > 0) {
        timeHigh *= 2;
    }

    
    for (int i = 0; i < parameters.rocket_binSearchPrecision ; i++) {
        if (homingFunc(targetPos, targetVel, (timeLow + timeHigh) / 2, accel) >= 0)
            timeLow = (timeLow + timeHigh) / 2;
        else 
            timeHigh = (timeLow + timeHigh) / 2;
    }
    
    return geom::dir(targetPos + targetVel * timeLow);
}


double rocket::updateDir(Object rocket, Object target,double accel, double rocketDir, Parameters& parameters) {
    return rocket::calcUpdateDir(target.pos - rocket.pos, target.vel - rocket.vel, accel, rocketDir, parameters);
}

double rocket::calcUpdateDir(Vec2 targetPos, Vec2 targetVel, double accel, double rocketDir, Parameters& parameters) {
    int timeEst = 0;
    if (!(geom::direction(rocketDir + M_PI / 2) * targetVel)) {
        timeEst = -(geom::direction(rocketDir + M_PI / 2) * targetPos) / (geom::direction(rocketDir + M_PI / 2) * targetVel);
    } 
    timeEst -= rocket::homingFuncDeriv(targetPos, targetVel, timeEst, accel) / rocket::homingFunc(targetPos, targetVel, timeEst, accel);
    return geom::dir(targetPos + targetVel * timeEst);
}


double rocket::homingFunc(Vec2 pos, Vec2 vel, double time, double accel) {
    return (pos * pos) + 2. * (pos * vel) * time + (vel * vel) * time * time - (1. / 4. * accel * accel * time * time * time * time);
}

double rocket::homingFuncDeriv(Vec2 pos, Vec2 vel, double time, double accel) {
    return (2. * (pos * vel) + 2. * (vel * vel) * time -  (accel * accel) * pow(time, 3));
}


bool rocket::isInRange(Object rocket, Object target, Parameters& parameters) {
    return (geom::distance(rocket.pos, target.pos) < parameters.rocket_targetingRadius);
}