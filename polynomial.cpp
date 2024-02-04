#include "polynomial.h"
#include <algorithm>
#include <list>
#include <iterator>
#include "term.h"
#include <cmath>


using namespace std;


void Polynomial::normalize ()
{
    //std::cout << "Before normalization: " << this << std::endl;
    if (degree < 0)
    {
        terms.clear();
        //std::cout << "After clearing terms: " <<this << std::endl;
        return;
    }
    //std::cout << "Before Sort: " << this << std::endl;
    // Sort terms based on power in descending order
    terms.sort([](const Term &b, const Term &a) { return a.power > b.power; });
    //std::cout << "After Sort: " <<this << std::endl;
    auto it = terms.begin();
    while (it != terms.end())
    {
        auto current = it;
        ++it;

        // Combine like terms
        while (it != terms.end() && current->power == it->power)
        {
            //std::cout << "Before Combine: " << this << std::endl;
            current->coefficient += it->coefficient;
            it = terms.erase(it);
            //std::cout << "After Combine: " <<this << std::endl;
        }
    }

    // Remove terms with zero coefficients
    //std::cout << "Before Remove: " << this << std::endl;
    terms.remove_if([](const Term &term) { return term.coefficient == 0; });
    //std::cout << "After Remove: " <<this << std::endl;

    if (terms.begin() != terms.end()){
        //std::cout << "Before Front: " << this << std::endl;
        degree = terms.back().power;
        //std::cout << "After Front: " <<this << std::endl;
    }
    else
        degree = 0;

    //std::cout << "After normalization: " << *this << std::endl;
}

//Constructor with an initializer list of terms.
Polynomial::Polynomial(std::initializer_list<Term> terms) : degree(0), terms() {
    for (const auto& term : terms) {
        cout << "initializer-list-constructor" << endl;
        this->terms.push_back(term);
    }
    normalize();
}

Polynomial::Polynomial() : degree(0), terms(){
    cout << "default-constructor" << endl;
}

Polynomial::Polynomial(int nC, int coeff[]) 
    : degree(nC - 1), terms() {
    cout << "int-array-constructor" << endl;
    for (int i = 0; i <= degree; i++) {
        terms.push_back(Term(coeff[i], i));
    }
    normalize();
}


Polynomial::Polynomial(int b, int a)
    : degree(0), terms() {
    cout << "int constructor" << endl;
    cout << "b: " << b << " a: " << a << endl;
    terms.push_back(Term(b, 0));
    terms.push_back(Term(a, 1));
    cout << "terms: " << terms.size() << endl;
    normalize();
}




int Polynomial::getDegree() const
{
    cout << "get degree" << endl;
    return degree;
}





void Polynomial::setDegree(int deg) {
    degree = deg; // Now it's a member function and can access degree
}
Polynomial Polynomial::operator*(int scale) const {
  if (this->getDegree() == -1) {
    return Polynomial();
  }

  Polynomial result(0);

  for (std::list<Term>::const_iterator it = terms.begin(); it != terms.end(); ++it) {
    int temp = scale * it->coefficient;
    int temp2 = it->power;
    result.terms.push_front(Term(temp, temp2));
  }

  result.degree = this->getDegree();
  result.normalize();
  return result;
}

Polynomial Polynomial::operator*(const Term term) const {
    if (degree == -1)
        return Polynomial();

    std::vector<int> results(degree + 1 + term.power, 0);

    for (const auto &t : terms) {
        results[t.power + term.power] += t.coefficient * term.coefficient;
    }

    // Convert results to a list of terms and construct the resulting Polynomial.
    Polynomial result;
    for (size_t i = 0; i < results.size(); ++i) {
        if (results[i] != 0) {
            result.terms.push_back(Term(results[i], i));
        }
    }
    result.normalize();
    return result;
}


Polynomial Polynomial::operator-(const Polynomial& p) const {
    Polynomial result;
    auto it1 = terms.begin();
    auto it2 = p.terms.begin();

    while (it1 != terms.end() && it2 != p.terms.end()) {
        if (it1->power < it2->power) {
            result.terms.push_back(*it1++);
        } else if (it1->power > it2->power) {
            result.terms.push_back(Term(-it2->coefficient, it2->power));
            ++it2;
        } else {
            int diffCoeff = it1->coefficient - it2->coefficient;
            if (diffCoeff != 0) {
                result.terms.push_back(Term(diffCoeff, it1->power));
            }
            ++it1;
            ++it2;
        }
    }

    while (it1 != this->terms.end()) {
        result.terms.push_back(*it1++);
    }
    while (it2 != p.terms.end()) {
        result.terms.push_back(Term(-it2->coefficient, it2->power));
        ++it2;
    }

    result.normalize();
    return result;
}





int Polynomial::evaluate(int x) const {
    int result = 0;
    for (const auto& term : terms) {
        result += term.coefficient * std::pow(x, term.power); // Ensure <cmath> is included.
    }
    return result;
}







// Polynomial Polynomial::operator*(int scale) const 
// {
//     if (degree == -1)
//         return Polynomial();

//     Polynomial result(*this);
//     for (auto &term : result.terms) // Iterate over result.terms, not terms
//     {
//         term.coefficient *= scale; // Scale the coefficients directly
//     }
//     result.normalize();
//     return result;
// }




int Polynomial::getCoeff(int powers) const
{
  std::list<Term>::const_iterator it = terms.begin();

  while (it != terms.end())
  {
    if (it->power == powers)
      return it->coefficient;
    else
      ++it;
  }

  return 0;
}









// Addition operator for polynomials
Polynomial Polynomial::operator+(const Polynomial &p) const {
    if (degree == -1 || p.degree == -1) {
        return Polynomial();
    }

    int resultSize = std::max(degree, p.degree) + 1;
    std::vector<int> resultCoefficients(resultSize, 0); // Use a vector for ease of use

    // Add coefficients from *this polynomial
    for (const auto& term : terms) {
        resultCoefficients[term.power] += term.coefficient;
    }

    // Add coefficients from p polynomial
    for (const auto& term : p.terms) {
        resultCoefficients[term.power] += term.coefficient;
    }

    // Convert the vector to a list of Terms to create the result Polynomial
    Polynomial result;
    for (int i = 0; i < resultSize; ++i) {
        if (resultCoefficients[i] != 0) {
            result.terms.push_back(Term(resultCoefficients[i], i));
        }
    }
    result.normalize();
    return result;
}



Polynomial& Polynomial::operator*=(int scale) {
    if (scale == 0) {
        terms.clear();
        degree = -1;
    } else {
        for (auto &term : terms) {
            term.coefficient *= scale;
        }
    }
    normalize();
    return *this; // Return a reference to the current object
}








bool Polynomial::operator==(const Polynomial &p) const
{
    cout << "compare" << endl;
    if (getDegree() != p.getDegree())
        return false;

    auto thisIt = terms.begin();

    for (auto it = p.terms.begin(); it != p.terms.end(); ++it)
    {
        if (thisIt == terms.end() || thisIt->coefficient != it->coefficient)
            return false;
        ++thisIt;
    }
    return true;
}




Polynomial Polynomial::operator/(const Polynomial &denominator) const
{
    if (degree == -1 || denominator.degree == -1)
        return Polynomial();
    if (*this == Polynomial(0))
        return *this;
    if (denominator.getDegree() > getDegree())
        return Polynomial();
    
    int resultSize = degree - denominator.degree + 1;
    int* results = new int[resultSize];
    for (int i = 0; i < resultSize; ++i)
        results[i] = 0;
    
    Polynomial remainder = *this;
    for (int i = resultSize-1; i >= 0; --i)
    {
        // Try to divide remainder by denominator*x^(i-1)
        int remainder1stCoeff = remainder.getCoeff(i+denominator.getDegree());
        int denominator1stCoeff = denominator.getCoeff(denominator.getDegree());
        if (remainder1stCoeff % denominator1stCoeff == 0)
        {
            results[i] = remainder1stCoeff / denominator1stCoeff;
            Polynomial subtractor = denominator * Term(-results[i], i);
            remainder = remainder + subtractor;
        }
        else
        {
            // Can't divide this
            break;
        }
    }
    if (remainder == Polynomial(0))
    {
        Polynomial result (resultSize, results);
        delete [] results;
        return result;
    }
    else
    {
        // A non-zero remainder could not be removed - division fails
        delete [] results;
        return Polynomial(0);
    }
}

