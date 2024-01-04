#include "includes/ALL.h"

//g++ -o RandTest.exe RandTest.cpp lib/*.cpp

Color White((unsigned char)255, (unsigned char)255, (unsigned char)255);
Color Red((unsigned char)0, (unsigned char)0, (unsigned char)255);
Color Black((unsigned char)0, (unsigned char)0, (unsigned char)0);

int main() {
    BMPGraphics PositionPIC(1024, 1024, "TestOutput/PositionDistribution.bmp");
    PositionPIC.SetBackground(White);
    particleGroup ParticleGroup2D(2, 8192);
    ParticleGroup2D.Init();
    ParticleGroup2D.PositionRand(0.0, 1024.0);
    for (int i = 0; i < ParticleGroup2D.Number; ++i) {
        DrawParticle(PositionPIC, 
                (int)ParticleGroup2D.Particles[i].Position.Elements[0],
                (int)ParticleGroup2D.Particles[i].Position.Elements[1],
                Red);
    }
    WriteSentence(PositionPIC, 10, 10, (unsigned char *)"PositionUniformDistributionTest", 31, Black, 1);
    PositionPIC.DoOutput();

    BMPGraphics AnglePIC2D(1024, 1024, "TestOutput/AnglePic2D.bmp");
    BMPGraphics VelocityPIC2D(1024, 1024, "TestOutput/VelocityPIC2D.bmp");
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
    WriteSentence(AnglePIC2D, 10, 10, (unsigned char *)"Angle2DUniformDistributionTest", 30, Black, 1);
    WriteSentence(VelocityPIC2D, 10, 10, (unsigned char *)"Velocity2DUniformDistributionTest", 33, Black, 1);
    AnglePIC2D.DoOutput();
    VelocityPIC2D.DoOutput();

    BMPGraphics Angle3DXY(1024, 1024, "TestOutput/Angle3DXY.bmp");
    BMPGraphics Angle3DYZ(1024, 1024, "TestOutput/Angle3DYZ.bmp");
    BMPGraphics Angle3DZX(1024, 1024, "TestOutput/Angle3DZX.bmp");
    Angle3DXY.SetBackground(White);
    Angle3DYZ.SetBackground(White);
    Angle3DZX.SetBackground(White);
    particleGroup ParticleGroup3D(3, 8192);
    ParticleGroup3D.Init();
    ParticleGroup3D.PositionRand(0.0, 1024.0);
    ParticleGroup3D.VelocityRand3D(10);
    for (int i = 0; i < ParticleGroup3D.Number; ++i) {
        double V = ParticleGroup3D.Particles[i].Velocity.Norm();
        DrawParticle(Angle3DXY,
                (int)(ParticleGroup3D.Particles[i].Velocity.Elements[0] / V * 500 + 512),
                (int)(ParticleGroup3D.Particles[i].Velocity.Elements[1] / V * 500 + 512),
                Red);
        DrawParticle(Angle3DYZ,
                (int)(ParticleGroup3D.Particles[i].Velocity.Elements[1] / V * 500 + 512),
                (int)(ParticleGroup3D.Particles[i].Velocity.Elements[2] / V * 500 + 512),
                Red);
        DrawParticle(Angle3DZX,
                (int)(ParticleGroup3D.Particles[i].Velocity.Elements[2] / V * 500 + 512),
                (int)(ParticleGroup3D.Particles[i].Velocity.Elements[0] / V * 500 + 512),
                Red);
    }
    WriteSentence(Angle3DXY, 10, 10, (unsigned char *)"Angle3DUniformDistributionTest-XY", 33, Black, 1);
    WriteSentence(Angle3DYZ, 10, 10, (unsigned char *)"Angle3DUniformDistributionTest-YZ", 33, Black, 1);
    WriteSentence(Angle3DZX, 10, 10, (unsigned char *)"Angle3DUniformDistributionTest-ZX", 33, Black, 1);
    Angle3DXY.DoOutput();
    Angle3DYZ.DoOutput();
    Angle3DZX.DoOutput();
    return 0;
}