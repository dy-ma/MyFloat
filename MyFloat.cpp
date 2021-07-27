#include "MyFloat.h"

MyFloat::MyFloat(){
  sign = 0;
  exponent = 0;
  mantissa = 0;
}

MyFloat::MyFloat(float f){
  unpackFloat(f);
}

MyFloat::MyFloat(const MyFloat & rhs){
	sign = rhs.sign;
	exponent = rhs.exponent;
	mantissa = rhs.mantissa;
}

ostream& operator<<(std::ostream &strm, const MyFloat &f){
	//this function is complete. No need to modify it.
	strm << f.packFloat();
	return strm;
}


MyFloat MyFloat::operator+(const MyFloat& rhs) const{
	// cout << "operator+ called" << endl; // remove
	cout << *this << " + " << rhs << endl; // remove
	if (rhs.sign == 1) { 	  // a + -b == a - b
		MyFloat rhscpy(rhs);  // -a + -b == -a - b == -(a + b)
		rhscpy.sign = 0;
		return (*this - rhscpy);
	}
	else if (sign == 1) { // -a + b == b - a
		MyFloat cpy(*this);
		cpy.sign = 0;
		return (rhs - cpy);
	}
	// now both are positive
	MyFloat ans;
	// ans = packFloat() + rhs.packFloat(); // bad solution,  uses hardware add
	return ans; //you don't have to return *this. it's just here right now so it will compile
}

MyFloat MyFloat::operator-(const MyFloat& rhs) const{
	// cout << "operator- called" << endl; // remove
	cout << *this << " - " << rhs << endl; // remove
	if (rhs.sign == 1) { 	// a - -b == a + b
		MyFloat rhscpy(rhs); // -a - -b == -a + b == b - a
		rhscpy.sign = 0;
		return (*this + rhscpy);
	}
	else if (sign == 1 && rhs.sign == 0) { // -a - b == -(a + b)
		MyFloat cpy(*this); // flip sign, add together, replace sign
		MyFloat rhscpy(rhs);
		MyFloat ans;
		cpy.sign = 0;
		rhscpy.sign = 0;
		ans = cpy + rhscpy;
		ans.sign = 1;
		return ans;
	}
	// now both are positive
	return *this; //you don't have to return *this. it's just here right now so it will compile
}

bool MyFloat::operator==(const float rhs) const{
	MyFloat myrhs(rhs);
	if (sign == myrhs.sign) {
		if (exponent == myrhs.exponent) {
			if (mantissa == myrhs.mantissa)
				return true;
		}
	}
	return false; //this is just a stub so your code will compile
}


void MyFloat::unpackFloat(float f) {
	//this function must be written in inline assembly
	//extracts the fields of f into sign, exponent, and mantissa

	long longf = *(long*)&f; // cast address of f to the address of a long
							// keep the underlying bits the same, but allow bitwise ops

	__asm__(
		// code
		"xorl %%ecx, %%ecx;" // zero ecx
		// get sign bit
		"movb $31, %%cl;"
		"movb $1, %%ch;"
		"bextr %%ecx, %%eax, %[sig];"

		// get exp
		"movb $23, %%cl;"
		"movb $8 , %%ch;"
		"bextr %%ecx, %%eax, %[exp];"

		// get mant
		"movb $0, %%cl;"
		"movb $23, %%ch;"
		"bextr %%ecx, %%eax, %[man]"
		:
		// outputs
		[sig] "=&r" (sign), [exp] "=&r" (exponent), [man] "=&r" (mantissa)
		:
		// inputs
		"a" (longf) // store f into a  register
		:
		// clobbered
		"cc", "%ecx"
	);
}//unpackFloat

float MyFloat::packFloat() const{
	//this function must be written in inline assembly
  //returns the floating point number represented by this
	float f = 0;
	__asm__(
		// code
		"xorl %[f], %[f];" // zero f
		"shl $31, %[sig];" // shift sig to 32nd bit
		"or %[sig], %[f];" // or sign with f
		"shl $23, %[exp];"
		"or %[exp], %[f];"
		"or %[man], %[f]"
		:
		// outputs
		[f] "=&r" (f)
		:
		// inputs
		[sig] "r" (sign), [exp] "r" (exponent), [man] "r" (mantissa)
		:
		// clobbered
	);
	return f;
}//packFloat
//



