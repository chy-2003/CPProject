#include "includes/ALL.h"
#include <cstring>
#include <cmath>

//g++ lib/*.cpp a.cpp -o a.exe -O2 -fopenmp -std=c++11

const int N = 64;
const double L = 20.0;
const double DeltaT = 0.001;
const double StartKT = 10.0;
const int ThreadNum = 4;
const int Dimension = 2;
const double gamma = 100.0;
const double epsilon = 1.0;
const double sigma = 1.0;
const double sigma12 = std::pow(sigma, 12.0);
const double sigma6 = std::pow(sigma, 6.0);
const double startAvoid = std::pow(2.0, 1.0 / 6.0) * sigma;
particleGroup AllFree(Dimension, N);

//#define __DO_PIC_OUTPUT__

Color White((unsigned char)255, (unsigned char)255, (unsigned char)255);
Color Red((unsigned char)0, (unsigned char)0, (unsigned char)255);
Color Blue((unsigned char)255, (unsigned char)0, (unsigned char)0);
Color Black((unsigned char)0, (unsigned char)0, (unsigned char)0);

void Init() {
    AllFree.Init();
    AllFree.PositionRand(0, L, startAvoid);
    AllFree.VelocityRand2D(StartKT);
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

int main() {
    Init();
    FILE *OutputTarget = fopen("CaseZero.csv", "w");
    int TotalStep = 50; int OutputNum = (TotalStep / 50);
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
    return 0;
}