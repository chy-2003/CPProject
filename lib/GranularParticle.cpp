#include "../includes/GranularParticle.h"
#include "../includes/BasicSupport.h"

singleParticle::singleParticle(int Dimention) {
    Position.SetZero();
    Velocity.SetZero();
    Mass = 1.0;
    return;
}

singleParticle::singleParticle() {
    Position.SetZero();
    Velocity.SetZero();
    Mass = 1.0;
    return;
}

void singleParticle::OutputState() {
    printf("Particle %lld, Mass = %.2lf:\n", (long long)(this), Mass);
    printf("\tPosition : (%.2lf, %.2lf, %.2lf)\n", 
            Position.Elements[0], Position.Elements[1], Position.Elements[2]);
    printf("\tVelocity : (%.2lf, %.2lf, %.2lf)\n", 
            Velocity.Elements[0], Velocity.Elements[1], Velocity.Elements[2]);
    return;
}

particleGroup::particleGroup(int _Dimention, int _Number) {
    Number = _Number;
    Dimention = _Dimention;
    Particles.resize(Number);
    Particles.shrink_to_fit();
//    for (std::vector<singleParticle>::iterator it = Particles.begin(); it != Particles.end(); ++it)
//        (*it).SetDimention(Dimention);
    return;
}

particleGroup::particleGroup() {
    Number = 100;
    Dimention = 2;
    Particles.resize(Number);
    Particles.shrink_to_fit();
//    for (std::vector<singleParticle>::iterator it = Particles.begin(); it != Particles.end(); ++it)
//        (*it).SetDimention(Dimention);
    return;
}
void particleGroup::Init() {
    for (int i = 0; i < Number; ++i) {
        Particles[i].Mass = 1.0;
        Particles[i].Position.SetZero();
        Particles[i].Velocity.SetZero();
    }
    return;
}

double particleGroup::KEnergy() {
    double Ans = 0;
    for (std::vector<singleParticle>::iterator it = Particles.begin(); it != Particles.end(); ++it) 
        Ans += 0.5 * (*it).Velocity.NormSquare() * (*it).Mass;
    return Ans;
}

double particleGroup::KTemperature() {
    return KEnergy() / Number;
}

double particleGroup::Energy(double (*Potential)(const singleParticle &a, const singleParticle &b)) {
    double Ans = KEnergy();
    for (int i = 0; i < Number; ++i)
        for (int j = i + 1; j < Number; ++j)
            Ans += Potential(Particles[i], Particles[j]);
    return Ans;
}

void particleGroup::PositionRand(double Lowerbound, double Upperbound) {
    std::random_device Rd;
    std::mt19937 Gen(Rd());
    std::uniform_real_distribution<> URD(Lowerbound, Upperbound);
    for (int i = 0; i < Number; ++i) {
        for (int j = 0; j < Dimention; ++j)
            Particles[i].Position.Elements[j] = URD(Gen);
    }
    return;
}

void particleGroup::PositionRand(double Lowerbound, double Upperbound, double startAvoid) {
    std::random_device Rd;
    std::mt19937 Gen(Rd());
    std::uniform_real_distribution<> URD(Lowerbound, Upperbound);
    for (int i = 0; i < Number; ++i) {
        int Flg = 0;
        while (!Flg) {
            for (int j = 0; j < Dimention; ++j)
                Particles[i].Position.Elements[j] = URD(Gen);
            Flg = 1;
            for (int j = 0; j < i; ++j)
                if ((Particles[i].Position - Particles[j].Position).Norm() < startAvoid) {
                    Flg = 0;
                    break;
                }
        }
    }
    return;
}

void particleGroup::VelocityRand2D(double KT) {
    std::random_device Rd;
    std::mt19937 Gen(Rd());
    std::uniform_real_distribution<> URDA(-Pi, Pi);
    std::uniform_real_distribution<> URDR(0.0, std::sqrt(2.0 * KT));
    double Total = 0, Angle, Velocity;
    for (int i = 0; i < Number; ++i) {
        Angle = URDA(Gen);
        Velocity = URDR(Gen);
        Total += 0.5 * Velocity * Velocity * Particles[i].Mass;
        Particles[i].Velocity.Elements[0] = std::cos(Angle) * Velocity;
        Particles[i].Velocity.Elements[1] = std::sin(Angle) * Velocity;
    }
    double Scale = std::sqrt(Number * KT / Total);
    for (int i = 0; i < Number; ++i) {
        Particles[i].Velocity.Elements[0] *= Scale;
        Particles[i].Velocity.Elements[1] *= Scale;
    }
    return;
}

void particleGroup::VelocityRand3D(double KT) {
    std::random_device Rd;
    std::mt19937 Gen(Rd());
    std::uniform_real_distribution<> URD(0.0, 1.0);
    std::uniform_real_distribution<> URDR(0.0, std::sqrt(2.0 * KT));
    double Total = 0, AngleU, AngleV, Velocity;
    for (int i = 0; i < Number; ++i) {
        AngleU = URD(Gen);
        AngleV = URD(Gen);
        AngleU = 2.0 * Pi * AngleU;
        AngleV = std::acos(2.0 * AngleV - 1);
        Velocity = URDR(Gen);
        Total += 0.5 * Velocity * Velocity * Particles[i].Mass;
        Particles[i].Velocity.Elements[0] = std::sin(AngleU) * std::sin(AngleV) * Velocity;
        Particles[i].Velocity.Elements[1] = std::cos(AngleU) * std::sin(AngleV) * Velocity;
        Particles[i].Velocity.Elements[2] = std::cos(AngleV) * Velocity;
    }
    double Scale = std::sqrt(Number * KT / Total);
    for (int i = 0; i < Number; ++i) {
        Particles[i].Velocity.Elements[0] *= Scale;
        Particles[i].Velocity.Elements[1] *= Scale;
        Particles[i].Velocity.Elements[2] *= Scale;
    }
    return;
}

void particleGroup::RK4_2(double DeltaT,
        DVector (*Force)(const singleParticle &a, const singleParticle &b),
        singleParticle (*BoundaryModifier)(const singleParticle &a),
        int ThreadNum) {
    
    std::vector<singleParticle> T1, T2, T3, T4, Temp; //Position -> U, Velocity -> V
    T1.resize(Number); T2.resize(Number); T3.resize(Number); T4.resize(Number);
    Temp.resize(Number);

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ThreadNum)
    for (int i = 0; i < Number; ++i) {
        T1[i].Position = Particles[i].Velocity * DeltaT;
        DVector ForceSum;
        ForceSum.SetZero();
        for (int j = 0; j < Number; ++j)
            ForceSum = ForceSum + Force(Particles[i], Particles[j]);
        T1[i].Velocity = ForceSum * (DeltaT * (1.0 / Particles[i].Mass));
    }
    
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ThreadNum)
    for (int i = 0; i < Number; ++i) {
        Temp[i].Position = Particles[i].Position + T1[i].Position * 0.5;
        Temp[i].Velocity = Particles[i].Velocity + T1[i].Velocity * 0.5;
    }
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ThreadNum)
    for (int i = 0; i < Number; ++i) {
        T2[i].Position = (Particles[i].Velocity + T1[i].Velocity * 0.5) * DeltaT;
        DVector ForceSum;
        ForceSum.SetZero();
        for (int j = 0; j < Number; ++j)
            ForceSum = ForceSum + Force(Temp[i], Temp[j]);
        T2[i].Velocity = ForceSum * (DeltaT * (1.0 / Particles[i].Mass));
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ThreadNum)
    for (int i = 0; i < Number; ++i) {
        Temp[i].Position = Particles[i].Position + T2[i].Position * 0.5;
        Temp[i].Velocity = Particles[i].Velocity + T2[i].Velocity * 0.5;
    }
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ThreadNum)
    for (int i = 0; i < Number; ++i) {
        T3[i].Position = (Particles[i].Velocity + T2[i].Velocity * 0.5) * DeltaT;
        DVector ForceSum;
        ForceSum.SetZero();
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Temp[i], Temp[j]);
        T3[i].Velocity = ForceSum * (DeltaT * (1.0 / Particles[i].Mass));
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ThreadNum)
    for (int i = 0; i < Number; ++i) {
        Temp[i].Position = Particles[i].Position + T3[i].Position * 0.5;
        Temp[i].Velocity = Particles[i].Velocity + T3[i].Velocity * 0.5;
    }
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ThreadNum)
    for (int i = 0; i < Number; ++i) {
        T4[i].Position = (Particles[i].Velocity + T3[i].Velocity) * DeltaT;
        DVector ForceSum;
        ForceSum.SetZero();
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Temp[i], Temp[j]);
        T4[i].Velocity = ForceSum * (DeltaT * (1.0 / Particles[i].Mass));
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ThreadNum)
    for (int i = 0; i < Number; ++i) {
        Particles[i].Position = Particles[i].Position + 
                (T1[i].Position + T2[i].Position * 2.0 + T3[i].Position * 2.0 + T4[i].Position) * (1.0 / 6.0);
        Particles[i].Velocity = Particles[i].Velocity + 
                (T1[i].Velocity + T2[i].Velocity * 2.0 + T3[i].Velocity * 2.0 + T4[i].Velocity) * (1.0 / 6.0);
        Particles[i] = BoundaryModifier(Particles[i]);
    }
    return;
}