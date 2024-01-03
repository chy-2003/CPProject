#include "../includes/BasicSupport.h"
#include <cstdio>
#include <cmath>


double BasicConstants::Pi = 3.14159265358979323846264f;

BasicContainers::DVector::DVector() : Type(false) {}
BasicContainers::DVector::~DVector() {
    Elements.resize(0);
    Elements.shrink_to_fit();
    return;
}
BasicContainers::DVector::DVector(const int &_Size) : Type(false) {
    Elements.resize(_Size);
    Elements.shrink_to_fit();
    return;
}
BasicContainers::DVector::DVector(const int &_Size, const bool &_Type) : Type(_Type) {
    Elements.resize(_Size);
    Elements.shrink_to_fit();
    return;
}
BasicContainers::DVector::DVector(const int &_Size, const double &_Elements) : Type(false) {
    Elements.resize(_Size, _Elements);
    Elements.shrink_to_fit();
    return;
}
BasicContainers::DVector::DVector(const int &_Size, const bool &_Type, const double &_Elements) : Type(_Type) {
    Elements.resize(_Size, _Elements);
    Elements.shrink_to_fit();
    return;
}
int BasicContainers::DVector::Size() const {
    return Elements.size();
}
double BasicContainers::DVector::Mod() const {
    double Ans = 0; 
    int Size = Elements.size();
    for (int i = 0; i < Size; ++i)
        Ans += Elements[i];
    return Ans;
}
void BasicContainers::DVector::T() {
    Type = !Type;
    return;
}
BasicContainers::DVector BasicContainers::DVector::__Copy() const {
    DVector New; 
    New.Elements = Elements;
    New.Type = Type;
    return New;
}
BasicContainers::DVector BasicContainers::DVector::__Minus() const {
    int Size = Elements.size();
    DVector New(Size, Type);
    for (int i = 0; i < Size; ++i)
        New.Elements[i] = -Elements[i];
    return New;
}
BasicContainers::DVector BasicContainers::DVector::T() const {
    DVector New;
    New.Elements = Elements;
    New.Type = !Type;
    return New;
}
BasicContainers::DVector operator + (const BasicContainers::DVector &a, const BasicContainers::DVector &b) {
    if (a.Type != b.Type) printf("[Warning] : Adding two DVector of different type.\n");
    if (a.Elements.size() != b.Elements.size()) 
        printf("[Warning] : Adding two DVector of different length.\n");
    int N = a.Elements.size();
    BasicContainers::DVector Ans(N, a.Type);
    for (int i = 0; i < N; ++i) Ans.Elements[i] = a.Elements[i] + b.Elements[i];
    return Ans;
}
BasicContainers::DVector operator - (const BasicContainers::DVector &a, const BasicContainers::DVector &b) {
    if (a.Type != b.Type) printf("[Warning] : Reducing two DVector of different type.\n");
    if (a.Elements.size() != b.Elements.size()) 
        printf("[Warning] : Reducing two DVector of different length.\n");
    int N = a.Elements.size();
    BasicContainers::DVector Ans(N, a.Type);
    for (int i = 0; i < N; ++i) Ans.Elements[i] = a.Elements[i] - b.Elements[i];
    return Ans;
}
BasicContainers::DVector operator * (const double &a, const BasicContainers::DVector &b) {
    int N = b.Elements.size();
    BasicContainers::DVector Ans(N, b.Type);
    for (int i = 0; i < N; ++i) Ans.Elements[i] = a * b.Elements[i];
    return Ans;
}
double operator * (const BasicContainers::DVector &a, const BasicContainers::DVector &b) {
    if (a.Type != true || b.Type != false) printf("[Warning] : Multiplying two DVector of error type.\n");
    if (a.Elements.size() != b.Elements.size()) 
        printf("[Warning] : Multiplying two DVector of different length.\n");
    int N = a.Elements.size();
    double Ans = 0;
    for (int i = 0; i < N; ++i) Ans = a.Elements[i] * b.Elements[i];
    return Ans;
}
double BasicContainers::DVector::NormSquare() {
    double Ans = 0;
    for (std::vector<double>::iterator it = Elements.begin(); it != Elements.end(); ++it)
        Ans += (*it) * (*it);
    return Ans;
}
double BasicContainers::DVector::Norm() { return std::sqrt(NormSquare()); }

double BasicFunctions::sqr(double x) { return x * x; }