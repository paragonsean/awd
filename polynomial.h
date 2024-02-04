#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <list>
#include <initializer_list>
#include "term.h"
#include <algorithm>
#include <iterator>
#include <cmath>

class Polynomial {
public:
    // Constructors
    Polynomial();
 
    Polynomial(int nCoeff, int coeff[]);
    Polynomial(std::initializer_list<Term> init);
    Polynomial(int a, int b = 0);
    // Iterators
    using iterator = std::list<Term>::iterator;
    using const_iterator = std::list<Term>::const_iterator;
    iterator begin() { return terms.begin(); }
    iterator end() { return terms.end(); }
    const_iterator begin() const { return terms.begin(); }
    const_iterator end() const { return terms.end(); }

    // Accessors
    int getCoeff(int power) const;
    int getDegree() const;
    int evaluate(int x) const;
 
    // Arithmetic operators
    Polynomial operator+(const Polynomial& p) const;
    Polynomial operator*(int scale) const;
    Polynomial operator*(const Term term) const;
    Polynomial operator-(const Polynomial& p) const;
    Polynomial operator/(const Polynomial& p) const;
    Polynomial& operator*=(int scale);
    bool operator==(const Polynomial& p) const;
    // Other member functions
  
    void setDegree(int deg);

private:
    int degree;  // Degree of the polynomial
    std::list<Term> terms;  // Terms of the polynomial, in ascending order of power

    // Utility functions
    void normalize();  // Removes terms with zero coefficients

    // Friend functions
    friend std::ostream& operator<<(std::ostream&, const Polynomial&);

public:
    // For use by instructor only
    bool sanityCheck() const;
};

// Function to print a polynomial to an ostream
std::ostream& operator<<(std::ostream&, const Polynomial&);

// Inline functions
inline Polynomial operator*(int by, const Polynomial& p) { return p * by; }
inline bool operator!=(const Polynomial& p, const Polynomial& q) { return !(p == q); }

#endif