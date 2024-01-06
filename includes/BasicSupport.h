#ifndef __BBG_BasicSupport__
#define __BBG_BasicSupport__

/*
 * Programed by PaimonZAYCHIK(chy)
 * 
 * version 3 (2024.01.03)
 *  - Gramma Fix
 *  - BasicContainers -> Resize, SetZero
 * 
 * version 2 (2023.12.28)
 *  - Add BasicContainers -> DVector
 *     NormSquare
 *     Norm
 * 
 * version 1 (2023.11.27)
 *  - Add BasicContainers -> DVector
 *     Mod
 *     Size
 *     __Copy    :  Don't use. return a vector equals to this vector
 *     __Minus   :  Don't use. return -DVector
 *     T         :  Change the vector itself
 *     FuncT     :  return a vector equals to the transformation of this vector
 *     operator = 
 *     operator +
 *     operator - 
 *     operator - (left)
 *     operator * (num)
 *     operator * (dvector) ,inner product
 * 
 * version 0 (2023.11.26)
 *  - BasicFunctions -> sqr
 *     omit
 */

#include <cmath>
#include <cstdio>
#include <cstring>

const double GlobalEPS = 1e-8;

extern double Pi;

double sqr(double x);

class DVector {
    public :
        double Elements[3];
        void SetZero();
        DVector __Minus() const;
        DVector& operator = (const DVector &Other) {
            memcpy(this->Elements, Other.Elements, sizeof(this->Elements));
            return *this;
        }
        DVector operator + (const DVector &other) const; 
        DVector operator -() const { return __Minus(); }
        DVector operator - (const DVector &other) const;
        DVector operator * (const double &other) const;
        friend DVector operator * (const double &r, const DVector &v);
        double operator * (const DVector &other) const;
        double NormSquare();
        double Norm();
        DVector e();
};

#endif