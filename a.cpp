#include "includes/ALL.h"
#include <cstring>
#include <cmath>

//g++ lib/*.cpp a.cpp -o a.exe -O2 -fopenmp -std=c++11

const int N = 64;
const double L = 20.0;
const double StartKT = 10.0;
const int ThreadNum = 12;
const int Dimension = 2;
const double gamma = 0.05;
const double epsilon = 1.0;
const double sigma = 1.0;
const double sigma12 = std::pow(sigma, 12.0);
const double sigma6 = std::pow(sigma, 6.0);
const double startAvoid = std::pow(2.0, 1.0 / 6.0) * sigma;
particleGroup AllFree(Dimension, N), Rec(Dimension, N);
DVector Wrap[100];

//#define __DO_PIC_OUTPUT__

Color White((unsigned char)255, (unsigned char)255, (unsigned char)255);
Color Red((unsigned char)0, (unsigned char)0, (unsigned char)255);
Color Blue((unsigned char)255, (unsigned char)0, (unsigned char)0);
Color Black((unsigned char)0, (unsigned char)0, (unsigned char)0);

void Init() {
    AllFree.Init();
    AllFree.PositionRand(0, L, startAvoid);
    AllFree.VelocityRand2D(StartKT);
    memset(Wrap, 0, sizeof(Wrap));
    return;
}

DVector Force(const singleParticle &a, const singleParticle &b) {
    DVector Ans;
    Ans.SetZero();
    
    DVector r = a.Position - b.Position;
    double R = r.Norm();
    if (fabs(R) < GlobalEPS) return Ans;
    DVector LennardJonesForce = 
            r.e() * (24.0 * epsilon * (2.0 * sigma12 / std::pow(R, 13.0) - sigma6 / std::pow(R, 7.0)));

    //DVector DragForce = (a.Position - b.Position) * 
    //        ((-gamma) * ((b.Velocity - a.Velocity) * (b.Position - a.Position)) / 
    //        (b.Position - a.Position).NormSquare());
    //printf("%.2lf\n", DragForce.Norm());
    //Ans = LennardJonesForce + DragForce;
    Ans = LennardJonesForce;
    return Ans;
}

DVector PeriodForce(const singleParticle &a, const singleParticle &b) {
    DVector Ans;
    Ans.SetZero();
    
    DVector r = a.Position - b.Position;
    if (r.Elements[0] < -L / 2) r.Elements[0] += L;
    if (r.Elements[0] >  L / 2) r.Elements[0] -= L;
    if (r.Elements[1] < -L / 2) r.Elements[1] += L;
    if (r.Elements[1] >  L / 2) r.Elements[1] -= L;
    if (r.Elements[2] < -L / 2) r.Elements[2] += L;
    if (r.Elements[2] >  L / 2) r.Elements[2] -= L;
    double R = r.Norm();
    if (fabs(R) < GlobalEPS) return Ans;
    DVector LennardJonesForce = 
            r.e() * (24.0 * epsilon * (2.0 * sigma12 / std::pow(R, 13.0) - sigma6 / std::pow(R, 7.0)));

    DVector DragForce = r * 
            (gamma * ((b.Velocity - a.Velocity) * r) / r.NormSquare());
    //printf("%.2lf\n", DragForce.Norm());
    Ans = LennardJonesForce + DragForce;
    //Ans = LennardJonesForce;
    return Ans;
}

double Potential(const singleParticle &a, const singleParticle &b) {
    double Ans = 0;
    DVector r = a.Position - b.Position;
    double R = r.Norm();
    if (fabs(R) < GlobalEPS) return Ans;
    double LennardJonesPotential = (4.0 * epsilon * (sigma12 / std::pow(R, 12.0) - sigma6 / std::pow(R, 6.0)));
    Ans = LennardJonesPotential;
    return Ans;
}

singleParticle BoundaryModifier(const singleParticle &a, int Index) {
    singleParticle Ans = a;
    if (Ans.Position.Elements[0] < 0) {
        Ans.Position.Elements[0] = -Ans.Position.Elements[0];
        Ans.Velocity.Elements[0] = -Ans.Velocity.Elements[0];
    }
    if (Ans.Position.Elements[0] > L) {
        Ans.Position.Elements[0] = 2.0 * L - Ans.Position.Elements[0];
        Ans.Velocity.Elements[0] = -Ans.Velocity.Elements[0];
    }
    if (Ans.Position.Elements[1] < 0) {
        Ans.Position.Elements[1] = -Ans.Position.Elements[1];
        Ans.Velocity.Elements[1] = -Ans.Velocity.Elements[1];
    }
    if (Ans.Position.Elements[1] > L) {
        Ans.Position.Elements[1] = 2.0 * L - Ans.Position.Elements[1];
        Ans.Velocity.Elements[1] = -Ans.Velocity.Elements[1];
    }
    return Ans;
}

singleParticle PeriodBoundaryModifier(const singleParticle &a, int Index) {
    singleParticle Ans = a;
    if (Ans.Position.Elements[0] < 0) {
        Ans.Position.Elements[0] += L;
        Wrap[Index].Elements[0] -= L;
    }
    if (Ans.Position.Elements[0] > L) {
        Ans.Position.Elements[0] -= L;
        Wrap[Index].Elements[0] += L;
    }
    if (Ans.Position.Elements[1] < 0) {
        Ans.Position.Elements[1] += L;
        Wrap[Index].Elements[1] -= L;
    }
    if (Ans.Position.Elements[1] > L) {
        Ans.Position.Elements[1] -= L;
        Wrap[Index].Elements[1] += L;
    }
    return Ans;
}

void WarnRE() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < i; ++j) {
            double R = (AllFree.Particles[i].Position - AllFree.Particles[j].Position).Norm();
            if (R < startAvoid / 10)
                printf("[WARNING]: particle distance < 2^(1/6) / 10\n");
        }
    return;
}

char GraphicName[100];

inline void Method_RK4_2(double DeltaT, int TotalStep) {
    Init();
    FILE *OutputTarget = fopen("CaseOne.csv", "w");
    int OutputNum = std::max(1, TotalStep / 50);
    for (int i = 0; i < TotalStep; ++i) {
        //WarnRE();
        if (i % OutputNum == 0) {
            double RecMaxV = 0.0;
//            for (int j = 1; j < N; ++j)
//                if (AllFree.Particles[j].Velocity.Norm() > RecMaxV)
//                    RecMaxV = AllFree.Particles[j].Velocity.Norm();
//            printf("MaxV = %.2lf\n", RecMaxV);
            #ifdef __DO_PIC_OUTPUT__
            memset(GraphicName, 0, sizeof(GraphicName));
            sprintf(GraphicName, "GO/%04d.bmp", i);
            BMPGraphics GraphicOutput(512, 512, GraphicName);
            GraphicOutput.SetBackground(White);
            for (int j = 0; j < N; ++j) {
                DrawParticleR(GraphicOutput, 
                        (int)(AllFree.Particles[j].Position.Elements[0] / 20.0 * 512),
                        (int)(AllFree.Particles[j].Position.Elements[1] / 20.0 * 512),
                        5, Red);
            }
            GraphicOutput.DoOutput();
            #endif
            printf("%6.2lf%\n", 100.0 * i / TotalStep);
        }
        fprintf(OutputTarget, "%.10lf, %.10lf, %.10lf\n", 
                i * DeltaT, AllFree.KTemperature(), AllFree.Energy(Potential));
        AllFree.RK4_2(DeltaT, Force, BoundaryModifier, ThreadNum);
    }
    printf("Done.\n");
    fclose(OutputTarget);
    return;
}

inline void Method_DRK4_2(double DeltaT, double Time, double Cutoff, double MaxCutoff, double MinCutoff) {
    Init();
    FILE *OutputTarget = fopen("CaseOne.csv", "w");
    for (double T = DeltaT; T < Time + DeltaT; T += DeltaT) {
        fprintf(OutputTarget, "%.10lf, %.10lf, %.10lf\n", 
                T - DeltaT, AllFree.KTemperature(), AllFree.Energy(Potential));
        //printf("%.10lf, %.10lf\n", (T - DeltaT) / Time, DeltaT);
        AllFree.DRK4_2(DeltaT, Cutoff, MaxCutoff, MinCutoff, Force, BoundaryModifier, ThreadNum);
    }
    printf("Done.\n");
    fclose(OutputTarget);
    return;
}

double InitialARS(int ThreadNum) {
    double Ans = 0;
    int ChunckSize = std::max(1, N / ThreadNum / 2);
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ChunckSize) reduction(+: Ans)
    for (int i = 0; i < N; ++i)
        Ans += ((AllFree.Particles[i].Position + Wrap[i]) - Rec.Particles[i].Position).Norm();
    return Ans / N;
}

double SelfARS(int ThreadNum) {
    double Ans = 0;
    int ChunckSize = std::max(1, N * N / ThreadNum);
    #pragma omp parallel for num_threads(ThreadNum) schedule(dynamic, ChunckSize) reduction(+: Ans)
    for (int i = 0; i < N * N; ++i)
        Ans += ((AllFree.Particles[i / N].Position + Wrap[i / N]) - 
                (AllFree.Particles[i % N].Position + Wrap[i % N])).Norm();
    return Ans / (N * N);
}

inline void Method_DRK4_2_Kick(double DeltaT, double Time, double Cutoff, double MaxCutoff, double MinCutoff,
        double KT) {
    Init();
    Rec = AllFree;

    std::random_device Rd;
    std::mt19937 Gen(Rd());
    std::uniform_real_distribution<> URDA(-Pi, Pi);
    std::uniform_real_distribution<> URDR(0.0, std::sqrt(2.0 * KT));
    std::uniform_int_distribution<> UID(0, N - 1);
    double Angle, Velocity, i;

    FILE *OutputTarget = fopen("CaseOne.csv", "w");
    for (double T = DeltaT; T < Time + DeltaT; T += DeltaT) {
        fprintf(OutputTarget, "%.10lf, %.10lf, %.10lf, %.10lf, %.10lf\n", 
                T - DeltaT, AllFree.KTemperature(), AllFree.Energy(Potential), 
                InitialARS(ThreadNum), SelfARS(ThreadNum));
        //printf("%.10lf, %.10lf\n", (T - DeltaT) / Time, DeltaT);

        AllFree.DRK4_2(DeltaT, Cutoff, MaxCutoff, MinCutoff, 
                PeriodForce, PeriodBoundaryModifier, ThreadNum);

        Angle = URDA(Gen);
        Velocity = URDR(Gen);
        i = UID(Gen);
        AllFree.Particles[i].Velocity.Elements[0] = 
                std::cos(Angle) * Velocity + AllFree.Particles[i].Velocity.Elements[0];
        AllFree.Particles[i].Velocity.Elements[1] = 
                std::sin(Angle) * Velocity + AllFree.Particles[i].Velocity.Elements[1];

    }
    printf("Done.\n");
    fclose(OutputTarget);
    return;
}

int main() {
    //Method_RK4_2(0.001, 5000);
    //Method_DRK4_2(0.001, 1.0, 0.001, 0.01, 0.0002);
    Method_DRK4_2_Kick(0.001, 5.0, 0.001, 0.01, 0.0002, 0.1);
    return 0;
}