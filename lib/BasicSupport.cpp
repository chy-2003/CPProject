#include "../includes/BasicSupport.h"
#include <cstdio>
#include <cmath>


double Pi = 3.14159265358979323846264f;

double sqr(double x) { return x * x; }

DVector::DVector() : Type(false) {}
DVector::~DVector() {
    Elements.resize(0);
    Elements.shrink_to_fit();
    return;
}
DVector::DVector(const int &_Size) : Type(false) {
    Elements.resize(_Size);
    Elements.shrink_to_fit();
    return;
}
DVector::DVector(const int &_Size, const bool &_Type) : Type(_Type) {
    Elements.resize(_Size);
    Elements.shrink_to_fit();
    return;
}
DVector::DVector(const int &_Size, const double &_Elements) : Type(false) {
    Elements.resize(_Size, _Elements);
    Elements.shrink_to_fit();
    return;
}
DVector::DVector(const int &_Size, const bool &_Type, const double &_Elements) : Type(_Type) {
    Elements.resize(_Size, _Elements);
    Elements.shrink_to_fit();
    return;
}
int DVector::Size() const {
    return Elements.size();
}
void DVector::Resize(int _size) {
    Elements.resize(_size);
    Elements.shrink_to_fit();
    return;
}
void DVector::SetZero() {
    for (std::vector<double>::iterator it = Elements.begin(); it != Elements.end(); ++it)
        (*it) = 0;
    return;
}
double DVector::Mod() const {
    double Ans = 0; 
    int Size = Elements.size();
    for (int i = 0; i < Size; ++i)
        Ans += Elements[i];
    return Ans;
}
void DVector::T() {
    Type = !Type;
    return;
}
DVector DVector::__Copy() const {
    DVector New; 
    New.Elements = Elements;
    New.Type = Type;
    return New;
}
DVector DVector::__Minus() const {
    int Size = Elements.size();
    DVector New(Size, Type);
    for (int i = 0; i < Size; ++i)
        New.Elements[i] = -Elements[i];
    return New;
}
DVector DVector::T() const {
    DVector New;
    New.Elements = Elements;
    New.Type = !Type;
    return New;
}
DVector operator + (const DVector &a, const DVector &b) {
    int N = a.Elements.size();
    DVector Ans(N, a.Type);
    for (int i = 0; i < N; ++i) Ans.Elements[i] = a.Elements[i] + b.Elements[i];
    return Ans;
}
DVector operator - (const DVector &a, const DVector &b) {
    int N = a.Elements.size();
    DVector Ans(N, a.Type);
    for (int i = 0; i < N; ++i) Ans.Elements[i] = a.Elements[i] - b.Elements[i];
    return Ans;
}
DVector operator * (const double &a, const DVector &b) {
    int N = b.Elements.size();
    DVector Ans(N, b.Type);
    for (int i = 0; i < N; ++i) Ans.Elements[i] = a * b.Elements[i];
    return Ans;
}
double operator * (const DVector &a, const DVector &b) {
    int N = a.Elements.size();
    double Ans = 0;
    for (int i = 0; i < N; ++i) Ans = a.Elements[i] * b.Elements[i];
    return Ans;
}
double DVector::NormSquare() {
    double Ans = 0;
    for (std::vector<double>::iterator it = Elements.begin(); it != Elements.end(); ++it)
        Ans += (*it) * (*it);
    return Ans;
}
double DVector::Norm() { return std::sqrt(NormSquare()); }

DVector DVector::e() {
    double Len = Norm();
    if (std::fabs(Len) < GlobalEPS) 
        printf("[Warning]: You may have called e() of a zero vector.\n");
    DVector Ans = (*this);
    for (std::vector<double>::iterator it = Ans.Elements.begin(); it != Ans.Elements.end(); ++it)
        (*it) /= Len;
    return Ans;
}