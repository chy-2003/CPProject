#include "includes/ALL.h"
#include <cstring>
#include <cmath>

//g++ lib/*.cpp a.cpp -o a.exe -O2 -fopenmp -std=c++11

const int N = 8;
const double L = 20.0;
const double DeltaT = 0.001;
const double StartKT = 0.1;
const int ThreadNum = 12;
const int Dimension = 2;
const double gamma = 100.0;
const double epsilon = 0.1;
const double sigma = 3.0;
const double sigma12 = std::pow(sigma, 12.0);
const double sigma6 = std::pow(sigma, 6.0);
particleGroup AllFree(Dimension, N);

Color White((unsigned char)255, (unsigned char)255, (unsigned char)255);
Color Red((unsigned char)0, (unsigned char)0, (unsigned char)255);
Color Blue((unsigned char)255, (unsigned char)0, (unsigned char)0);
Color Black((unsigned char)0, (unsigned char)0, (unsigned char)0);

void Init() {
    AllFree.Init();
    AllFree.PositionRand(0, L);
    AllFree.VelocityRand2D(StartKT);
    return;
}

DVector Force(const singleParticle &a, const singleParticle &b) {
    DVector Ans(Dimension);
    Ans.SetZero();
    
    DVector r = a.Position - b.Position;
    double R = r.Norm();
    if (fabs(R) < GlobalEPS) return Ans;
    DVector LennardJonesForce = 
            (4.0 * epsilon * (12.0 * sigma12 / std::pow(R, 13.0) - 6.0 * sigma6 / std::pow(R, 7.0))) * r.e();

    //DVector DragForce = ((-gamma) * 
    //        ((b.Velocity - a.Velocity) * (b.Position - a.Position)) / 
    //        (b.Position - a.Position).NormSquare()) *
    //        (b.Position - a.Position);
    
    //Ans = LennardJonesForce + DragForce;
    Ans = LennardJonesForce;
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

singleParticle BoundaryModifier(const singleParticle &a) {
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

char GraphicName[100];

int main() {
    Init();
    int MaxV = 0; double RecMaxV = 0.0;
    for (int i = 1; i < N; ++i)
        if (AllFree.Particles[i].Velocity.Norm() > RecMaxV) {
            RecMaxV = AllFree.Particles[i].Velocity.Norm();
            MaxV = i;
        }
    //AllFree.Particles[MaxV].OutputState();
    FILE *OutputTarget = fopen("CaseZero.csv", "w");
    int TotalStep = 1000;
    for (int i = 0; i < TotalStep; ++i) {
        if (i % 50 == 0) {
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
            DrawParticleR(GraphicOutput, 
                    (int)(AllFree.Particles[MaxV].Position.Elements[0] / 20.0 * 512),
                    (int)(AllFree.Particles[MaxV].Position.Elements[1] / 20.0 * 512),
                    5, Blue);
            GraphicOutput.DoOutput();
            printf("%6.2lf%\n", 100.0 * i / TotalStep);
        }
        fprintf(OutputTarget, "%.10lf, %.10lf, %.10lf\n", 
                i * DeltaT, AllFree.KTemperature(), AllFree.Energy(Potential));
        AllFree.RK4_2(DeltaT, Force, BoundaryModifier, ThreadNum);
        //printf("%d\n", i);
        //AllFree.Particles[MaxV].OutputState();
    }
    fclose(OutputTarget);
    return 0;
}