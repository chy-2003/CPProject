#include "../includes/GranularParticle.h"
#include "../includes/BasicSupport.h"

singleParticle::singleParticle(int Dimention) {
    Position.Resize(Dimention);
    Velocity.Resize(Dimention);
    Position.SetZero();
    Velocity.SetZero();
    Mass = 1.0;
    return;
}

singleParticle::singleParticle() {
    Position.Resize(2);
    Velocity.Resize(2);
    Position.SetZero();
    Velocity.SetZero();
    Mass = 1.0;
    return;
}

void singleParticle::SetDimention(int Dimention) {
    Position.Resize(Dimention);
    Velocity.Resize(Dimention);
    return;
}

void singleParticle::OutputState() {
    printf("Particle %d, Mass = %.2lf:\n", (int)(this), Mass);
    printf("\tPosition : ");
    for (std::vector<double>::iterator it = Position.Elements.begin(); 
            it != Position.Elements.end(); ++it)
        printf("%.2lf,", (*it));
    printf("\n");
    printf("\tVelocity : ");
    for (std::vector<double>::iterator it = Velocity.Elements.begin(); 
            it != Velocity.Elements.end(); ++it)
        printf("%.2lf,", (*it));
    printf("\n");
    return;
}

particleGroup::particleGroup(int _Dimention, int _Number) {
    Number = _Number;
    Dimention = _Dimention;
    Particles.resize(Number);
    Particles.shrink_to_fit();
    for (std::vector<singleParticle>::iterator it = Particles.begin(); it != Particles.end(); ++it)
        (*it).SetDimention(Dimention);
    return;
}

particleGroup::particleGroup() {
    Number = 100;
    Dimention = 2;
    Particles.resize(Number);
    for (std::vector<singleParticle>::iterator it = Particles.begin(); it != Particles.end(); ++it)
        (*it).SetDimention(Dimention);
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
    int Chunksize = std::max(1, Number / ThreadNum / 2);

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize)
    for (int i = 0; i < Number; ++i) {
        T1[i].Position = DeltaT * Particles[i].Velocity;
        DVector ForceSum;
        ForceSum.Resize(Number); ForceSum.SetZero(); 
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Particles[i], Particles[j]);
        T1[i].Velocity = DeltaT * (1.0 / Particles[i].Mass) * ForceSum;
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        Temp[i].Position = Particles[i].Position + 0.5 * T1[i].Position;
        Temp[i].Velocity = Particles[i].Velocity + 0.5 * T1[i].Velocity;
    }
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        T2[i].Position = DeltaT * (Particles[i].Velocity + 0.5 * T1[i].Velocity);
        DVector ForceSum;
        ForceSum.Resize(Number); ForceSum.SetZero();
        for (int j = 0; j < Number; ++j)
            ForceSum = ForceSum + Force(Temp[i], Temp[j]);
        T2[i].Velocity = DeltaT * (1.0 / Particles[i].Mass) * ForceSum;
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        Temp[i].Position = Particles[i].Position + 0.5 * T2[i].Position;
        Temp[i].Velocity = Particles[i].Velocity + 0.5 * T2[i].Velocity;
    }
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        T3[i].Position = DeltaT * (Particles[i].Velocity + 0.5 * T2[i].Velocity);
        DVector ForceSum;
        ForceSum.Resize(Number); ForceSum.SetZero();
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Temp[i], Temp[j]);
        T3[i].Velocity = DeltaT * (1.0 / Particles[i].Mass) * ForceSum;
    }

    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        Temp[i].Position = Particles[i].Position + 0.5 * T3[i].Position;
        Temp[i].Velocity = Particles[i].Velocity + 0.5 * T3[i].Velocity;
    }
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, Chunksize) 
    for (int i = 0; i < Number; ++i) {
        T4[i].Position = DeltaT * (Particles[i].Velocity + T3[i].Velocity);
        DVector ForceSum;
        ForceSum.Resize(Number); ForceSum.SetZero();
        for (int j = 0; j < Number; ++j) 
            ForceSum = ForceSum + Force(Temp[i], Temp[j]);
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