#ifndef __BBG_GRANULARPARTICLE__
#define __BBG_GRANULARPARTICLE__

#include "BasicSupport.h"
#include <vector>
#include <random>
#include <cmath>
#include <omp.h>

/*
 *
 *
 * Programed by PaimonZAYCHIK(chy)
 * 
 * version 2 (2024.01.12)
 *  - particleGroup
 *    - DRK4_2
 * 
 * version 1 (2024.01.03)
 *  - particleGroup
 *    - RK4_2
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

class singleParticle {
public :
    double Mass;
    DVector Position, Velocity;
    singleParticle(int);
    singleParticle();
    void OutputState();
};

class particleGroup {
public :
    std::vector<singleParticle> Particles;
    int Number, Dimension;
    particleGroup(int, int);
    particleGroup();
    particleGroup& operator= (const particleGroup &Other) {
        Number = Other.Number;
        Dimension = Other.Dimension;
        Particles = Other.Particles;
        return (*this);
    }
    void Init();
    double KEnergy();
    double KTemperature();
    double Energy(double (*Potential)(const singleParticle &, const singleParticle &));
    void PositionRand(double, double);
    void PositionRand(double, double, double);
    void VelocityRand2D(double);              //for 2D system, uniform distribution(circle)
    void VelocityRand3D(double);              //for 3D system, uniform distribution(ball)

    void RK4_2(double DeltaT, 
            DVector (*Force)(const singleParticle &a, const singleParticle &b), 
            singleParticle (*BoundaryModifier)(const singleParticle &a, int Index),
            int ThreadNum);

    void DRK4_2(double &DeltaT, double Cutoff, double MaxDeltaT, double MinDeltaT,
            DVector (*Force)(const singleParticle &a, const singleParticle &b), 
            singleParticle (*BoundaryModifier)(const singleParticle &a, int Index),
            int ThreadNum);
};

#endif