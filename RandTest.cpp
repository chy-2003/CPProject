#include "includes/ALL.h"

//g++ -o RandTest.exe RandTest.cpp lib/*.cpp

int main() {
    BMPGraphics PositionPIC(1024, 1024, "PositionDistribution.bmp");
    PositionPIC.SetBackground(Color((unsigned char)255, (unsigned char)255, (unsigned char)255));
    GranularParticle::particleGroup ParticleGroup2D(2, 8192);
    ParticleGroup2D.Init();
    ParticleGroup2D.PositionRand(0.0, 1024.0);
    for (int i = 0; i < ParticleGroup2D.Number; ++i) {
        //PositionPIC.SetPixel((int)ParticleGroup2D.Particles[i].Position.Elements[0], 
        //        (int)ParticleGroup2D.Particles[i].Position.Elements[1],
        //        Color((unsigned char)0, (unsigned char)0, (unsigned char)255));
        DrawParticle(PositionPIC, (int)ParticleGroup2D.Particles[i].Position.Elements[0],
                (int)ParticleGroup2D.Particles[i].Position.Elements[1],
                Color((unsigned char)0, (unsigned char)0, (unsigned char)255));
    }
    PositionPIC.DoOutput();
    return 0;
}