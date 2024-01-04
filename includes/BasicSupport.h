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
#include <vector>
#include <cstdio>

const double GlobalEPS = 1e-8;

extern double Pi;

double sqr(double x);

class DVector {
    public :
        std::vector<double> Elements;
        bool Type;   //False for n*1, True for 1*n
        DVector();
        ~DVector();
        DVector(const int &);
        DVector(const int &, const bool &);
        DVector(const int &, const double &);
        DVector(const int &, const bool &, const double &);
        int Size() const;
        double Mod() const;
        void Resize(int _size);
        void SetZero();
        DVector __Copy() const;
        DVector __Minus() const;
        void T();
        DVector T() const;
        DVector& operator = (const DVector &Other) {
            this->Type = Other.Type;
            this->Elements = Other.Elements;
            return *this;
        }
        friend DVector operator + (const DVector &a, const DVector &b); 
        DVector operator -() const { return __Minus(); }
        friend DVector operator - (const DVector &a, const DVector &b);
        friend DVector operator * (const double &a, const DVector &b);
        friend double operator * (const DVector &a, const DVector &b);
        double NormSquare();
        double Norm();
        DVector e();
};

#endif