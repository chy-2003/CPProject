#include "../includes/BasicSupport.h"
#include <cstdio>
#include <cmath>


double Pi = 3.14159265358979323846264f;

double sqr(double x) { return x * x; }

void DVector::SetZero() {
    Elements[0] = Elements[1] = Elements[2] = 0;
    return;
}
DVector DVector::__Minus() const {
    DVector New;
    New.Elements[0] = -Elements[0];
    New.Elements[1] = -Elements[1];
    New.Elements[2] = -Elements[2];
    return New;
}
DVector DVector::operator+ (const DVector &other) const {
    DVector Ans;
    Ans.Elements[0] = Elements[0] + other.Elements[0];
    Ans.Elements[1] = Elements[1] + other.Elements[1];
    Ans.Elements[2] = Elements[2] + other.Elements[2];
    return Ans;
}
DVector DVector::operator- (const DVector &other) const{
    DVector Ans; 
    Ans.Elements[0] = Elements[0] - other.Elements[0];
    Ans.Elements[1] = Elements[1] - other.Elements[1];
    Ans.Elements[2] = Elements[2] - other.Elements[2];
    return Ans;
}
DVector DVector::operator* (const double &other) const{
    DVector Ans;
    Ans.Elements[0] = Elements[0] * other;
    Ans.Elements[1] = Elements[1] * other;
    Ans.Elements[2] = Elements[2] * other;
    return Ans;
}
DVector operator * (const double &other, const DVector r) {
    DVector Ans;
    Ans.Elements[0] = r.Elements[0] * other;
    Ans.Elements[1] = r.Elements[1] * other;
    Ans.Elements[2] = r.Elements[2] * other;
    return Ans;
}
double DVector::operator* (const DVector &other) const{
    return Elements[0] * other.Elements[0] +
            Elements[1] * other.Elements[1] +
            Elements[2] * other.Elements[2];
}
double DVector::NormSquare() {
    return Elements[0] * Elements[0] +
            Elements[1] * Elements[1] +
            Elements[2] * Elements[2];
}
double DVector::Norm() { return std::sqrt(NormSquare()); }

DVector DVector::e() {
    double Len = Norm();
//    if (std::fabs(Len) < GlobalEPS) 
//        printf("[Warning]: You may have called e() of a zero vector.\n");
    DVector Ans = (*this);
    Ans.Elements[0] /= Len;
    Ans.Elements[1] /= Len;
    Ans.Elements[2] /= Len;
    return Ans;
}