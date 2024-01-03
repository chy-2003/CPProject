#ifndef __BBG_GRANULARPARTICLE__
#define __BBG_GRANULARPARTICLE__

#include "BasicSupport.h"
#include <vector>
#include <random>
#include <cmath>

/*
 *
 *
 * Programed by PaimonZAYCHIK(chy)
 * 
 * version 0 (2024.01.03)
 *  - singleParticle
 *    - SetDimention
 * 
 *  - particleGroup
 *    - Init
 *    - KEnergy
 *    - KTemperature
 *    - PositionRand
 *    - VelocityRand2D
 *    - VelocityRand3D
 *
 */

namespace GranularParticle {
    class singleParticle {
    public :
        BasicContainers::DVector Position, Velocity;
        double Mass;
        singleParticle(int);
        singleParticle();
        void SetDimention(int);
    };

    class particleGroup {
    public :
        std::vector<singleParticle> Particles;
        int Number, Dimention;
        particleGroup(int, int);
        particleGroup();
        void Init();
        double KEnergy();
        double KTemperature();
        void PositionRand(double, double);
        void VelocityRand2D(double);              //for 2D system, uniform distribution(circle)
        void VelocityRand3D(double);              //for 3D system, uniform distribution(ball)
    };
}

#endif