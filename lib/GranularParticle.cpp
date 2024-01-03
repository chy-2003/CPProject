#include "../includes/GranularParticle.h"

GranularParticle::singleParticle::singleParticle(int Dimention) {
    Position.Resize(Dimention);
    Velocity.Resize(Dimention);
    for (int i = 0; i < Dimention; ++i)
        Position.Elements[i] = Velocity.Elements[i] = 0;
    Mass = 1;
    return;
}

GranularParticle::singleParticle::singleParticle() {
    Position.Resize(2);
    Velocity.Resize(2);
    for (int i = 0; i < 2; ++i)
        Position.Elements[i] = Velocity.Elements[i] = 0;
    Mass = 1;
    return;
}

void GranularParticle::singleParticle::SetDimention(int Dimention) {
    Position.Resize(Dimention);
    Velocity.Resize(Dimention);
    return;
}

GranularParticle::singleParticle operator * (const double &a, const GranularParticle::singleParticle &b) {
    GranularParticle::singleParticle Ans = b;
    Ans.Position = 0.5 * b.Position;
    Ans.Velocity = 0.5 * b.Velocity;
    return Ans;
}

GranularParticle::singleParticle operator + (const GranularParticle::singleParticle &a, 
        const GranularParticle::singleParticle &b) {
    GranularParticle::singleParticle Ans = a;
    Ans.Position = a.Position + b.Position;
    Ans.Velocity = a.Velocity + b.Velocity;
    return Ans;
}

GranularParticle::particleGroup::particleGroup(int _Dimention, int _Number) {
    Number = _Number;
    Dimention = _Dimention;
    Particles.resize(Number);
    for (std::vector<singleParticle>::iterator it = Particles.begin(); it != Particles.end(); ++it)
        (*it).SetDimention(Dimention);
    return;
}

GranularParticle::particleGroup::particleGroup() {
    Number = 100;
    Dimention = 2;
    Particles.resize(Number);
    for (std::vector<singleParticle>::iterator it = Particles.begin(); it != Particles.end(); ++it)
        (*it).SetDimention(Dimention);
    return;
}
void GranularParticle::particleGroup::Init() {
    for (int i = 0; i < Number; ++i) {
        Particles[i].Mass = 1;
        for (int i = 0; i < Dimention; ++i)
            Particles[i].Position.Elements[i] = Particles[i].Velocity.Elements[i] = 0;
    }
    return;
}

double GranularParticle::particleGroup::KEnergy() {
    double Ans = 0;
    for (std::vector<singleParticle>::iterator it = Particles.begin(); it != Particles.end(); ++it) 
        Ans += 0.5 * (*it).Velocity.NormSquare() * (*it).Mass;
    return Ans;
}

double GranularParticle::particleGroup::KTemperature() {
    return KEnergy() / Number;
}

void GranularParticle::particleGroup::PositionRand(double Lowerbound, double Upperbound) {
    std::random_device Rd;
    std::mt19937 Gen(Rd());
    std::uniform_real_distribution<> URD(Lowerbound, Upperbound);
    for (int i = 0; i < Number; ++i) {
        for (int j = 0; j < Dimention; ++j)
            Particles[i].Position.Elements[j] = URD(Gen);
    }
    return;
}

void GranularParticle::particleGroup::VelocityRand2D(double KT) {
    std::random_device Rd;
    std::mt19937 Gen(Rd());
    std::uniform_real_distribution<> URDA(-BasicConstants::Pi, BasicConstants::Pi);
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

void GranularParticle::particleGroup::VelocityRand3D(double KT) {
    std::random_device Rd;
    std::mt19937 Gen(Rd());
    std::uniform_real_distribution<> URD(0.0, 1.0);
    std::uniform_real_distribution<> URDR(0.0, std::sqrt(2.0 * KT));
    double Total = 0, AngleU, AngleV, Velocity;
    for (int i = 0; i < Number; ++i) {
        AngleU = URD(Gen);
        AngleV = URD(Gen);
        AngleU = 2.0 * BasicConstants::Pi * AngleU;
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

void GranularParticle::particleGroup::RK4_2(double DeltaT,
        BasicContainers::DVector (*Force)(const singleParticle &a, const singleParticle &b),
        singleParticle (*BoundaryModifier)(const singleParticle &a),
        int ThreadNum) {
    std::vector<singleParticle> T1, T2, T3, T4; //Position -> U, Velocity -> V
    T1.resize(Number); T2.resize(Number); T3.resize(Number); T4.resize(Number);
    int Chunksize = std::max(1, Number / ThreadNum / 2);

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize)
    for (int i = 0; i < Number; ++i) {
        T1[i].Position = DeltaT * Particles[i].Velocity;
        BasicContainers::DVector ForceSum;
        ForceSum.Resize(Number); ForceSum.SetZero(); 
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Particles[i], Particles[j]);
        T1[i].Velocity = DeltaT * (1.0 / Particles[i].Mass) * ForceSum;
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        T2[i].Position = DeltaT * (Particles[i].Velocity + 0.5 * T1[i].Velocity);
        BasicContainers::DVector ForceSum;
        ForceSum.Resize(Number); ForceSum.SetZero();
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Particles[i] + 0.5 * T1[i], Particles[j] + 0.5 * T1[j]);
        T2[i].Velocity = DeltaT * (1.0 / Particles[i].Mass) * ForceSum;
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        T3[i].Position = DeltaT * (Particles[i].Velocity + 0.5 * T2[i].Velocity);
        BasicContainers::DVector ForceSum;
        ForceSum.Resize(Number); ForceSum.SetZero();
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Particles[i] + 0.5 * T2[i], Particles[j] + 0.5 * T2[j]);
        T3[i].Velocity = DeltaT * (1.0 / Particles[i].Mass) * ForceSum;
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        T4[i].Position = DeltaT * (Particles[i].Velocity + T3[i].Velocity);
        BasicContainers::DVector ForceSum;
        ForceSum.Resize(Number); ForceSum.SetZero();
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Particles[i] + T3[i], Particles[j] + T3[j]);
        T4[i].Velocity = DeltaT * (1.0 / Particles[i].Mass) * ForceSum;
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        Particles[i].Position = Particles[i].Position + 
                (1.0 / 6.0) * (T1[i].Position + 2.0 * T2[i].Position + 2.0 * T3[i].Position + T4[i].Position);
        Particles[i].Velocity = Particles[i].Velocity + 
                (1.0 / 6.0) * (T1[i].Velocity + 2.0 * T2[i].Velocity + 2.0 * T3[i].Velocity + T4[i].Velocity);
        Particles[i] = BoundaryModifier(Particles[i]);
    }
    return;
}