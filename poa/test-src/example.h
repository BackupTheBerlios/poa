#ifndef COMPLEX_H
#define COMPLEX_H
/// Example class to test with CppUnit 

class Complex { 
  friend bool operator ==(const Complex& a, const Complex& b);
  friend Complex operator +(const Complex& a, const Complex& b);
  friend Complex operator -(const Complex& a, const Complex& b);
  friend Complex operator *(const Complex& a, const Complex& b);
  friend Complex operator /(const Complex& a, const Complex& b);
  double real, imaginary;
 public:
   Complex( double r, double i = 0 ) 
     : real(r)
         , imaginary(i) 
   {
   }
 };

 bool operator ==( const Complex &a, const Complex &b )
 { 
   return a.real == b.real  &&  a.imaginary == b.imaginary; 
 }
 Complex operator +( const Complex &a, const Complex &b )
 { 
   return Complex(a.real+b.real,a.imaginary+b.imaginary); 
 }
 Complex operator -( const Complex &a, const Complex &b )
 { 
   return Complex(a.real-b.real,a.imaginary-b.imaginary); 
 }
 Complex operator *( const Complex &a, const Complex &b )
 { 
   return Complex((a.real*b.real-a.imaginary*b.imaginary),
                  (a.real*b.imaginary+a.imaginary*b.real));
 }
 Complex operator /( const Complex &a, const Complex &b )
 { 
   return Complex((a.real*b.real+a.imaginary*b.imaginary)/
                  (b.real*b.real+b.imaginary*b.imaginary),
   				  (a.imaginary*b.real-a.real*b.imaginary)/
                  (b.real*b.real+b.imaginary*b.imaginary));
 }
#endif // COMPLEX_H 

