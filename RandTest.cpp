#include "includes/ALL.h"

//g++ -o RandTest.exe RandTest.cpp lib/*.cpp

Color White((unsigned char)255, (unsigned char)255, (unsigned char)255);
Color Red((unsigned char)0, (unsigned char)0, (unsigned char)255);

int main() {
    BMPGraphics PositionPIC(1024, 1024, "PositionDistribution.bmp");
    PositionPIC.SetBackground(White);
    GranularParticle::particleGroup ParticleGroup2D(2, 8192);
    ParticleGroup2D.Init();
    ParticleGroup2D.PositionRand(0.0, 1024.0);
    for (int i = 0; i < ParticleGroup2D.Number; ++i) {
        DrawParticle(PositionPIC, 
                (int)ParticleGroup2D.Particles[i].Position.Elements[0],
                (int)ParticleGroup2D.Particles[i].Position.Elements[1],
                Red);
    }
    PositionPIC.DoOutput();

    BMPGraphics AnglePIC2D(1024, 1024, "AnglePic2D.bmp");
    BMPGraphics VelocityPIC2D(1024, 1024, "VelocityPIC2D.bmp");
    AnglePIC2D.SetBackground(White);
    VelocityPIC2D.SetBackground(White);
    ParticleGroup2D.VelocityRand2D(10);
    for (int i = 0; i < ParticleGroup2D.Number; ++i) {
        DrawParticle(VelocityPIC2D,
                (int)(ParticleGroup2D.Particles[i].Velocity.Elements[0] * 51.2 + 512),
                (int)(ParticleGroup2D.Particles[i].Velocity.Elements[1] * 51.2 + 512),
                Red);
        double V = ParticleGroup2D.Particles[i].Velocity.Norm();
        DrawParticle(AnglePIC2D,
                (int)(ParticleGroup2D.Particles[i].Velocity.Elements[0] / V * 500 + 512),
                (int)(ParticleGroup2D.Particles[i].Velocity.Elements[1] / V * 500 + 512),
                Red);
    }
    AnglePIC2D.DoOutput();
    VelocityPIC2D.DoOutput();
    return 0;
}