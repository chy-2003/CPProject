#ifndef __BBG_BasicSupport__
#define __BBG_BasicSupport__

/*
 * Programed by PaimonZAYCHIK(chy)
 * 
 * version 3 (2024.01.03)
 *  - Bug Fix
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

namespace BasicConstants {
    extern double Pi;
}

namespace BasicContainers {
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
            DVector __Copy() const;
            DVector __Minus() const;
            void T();
            DVector T() const;
            DVector& operator = (const DVector &Other) {
                this->Type = Other.Type;
                this->Elements = Other.Elements;
                return *this;
            }
            friend DVector operator + (const DVector &, const DVector &);
            DVector operator -() const { return __Minus(); }
            friend DVector operator - (const DVector &, const DVector &);
            friend DVector operator * (const double &, const DVector &);
            friend double operator * (const DVector &, const DVector &);
            double NormSquare();
            double Norm();
    };
}

namespace BasicFunctions {
    double sqr(double x);
}

#endif