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
	// cout << *this << " + " << rhs << endl; // remove
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
	if (exponent == 0 && mantissa == 0) return rhs; // adding 0 cases
	else if (rhs.exponent == 0 && rhs.mantissa == 0) return *this;
	else if (*this < rhs) return (rhs + *this);
	
	// actual addition
	MyFloat ans;
	MyFloat x1(*this);
	MyFloat x2(rhs);
	ans.exponent = x1.exponent;
	unsigned exp_diff = x1.exponent - x2.exponent;
	x2.mantissa >>= exp_diff; // exponents should be equal
	x2.mantissa |= (1 << (23 - exp_diff) ); // append 1
	ans.mantissa = x1.mantissa + x2.mantissa;
	// if a carry happens, increment exponent
	if (ans.mantissa < x1.mantissa || ans.mantissa < x2.mantissa) { // overflow deteced
		ans.exponent++;
	}
	// while (ans.mantissa >> 22 != 1) {
		// ans.mantissa >>= 1;
	// }
	// ans.mantissa >>= 1;
	// normalize answer
	// ans.mantissa <<= exp_diff; // how tf
	// ans.exponent += exp_diff; // how tf

	cout << ans.sign << " " << ans.exponent << " " << ans.mantissa << endl;

	return ans; //you don't have to return *this. it's just here right now so it will compile
}

MyFloat MyFloat::operator-(const MyFloat& rhs) const{
	// cout << "operator- called" << endl; // remove
	// cout << *this << " - " << rhs << endl; // remove
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
	if (rhs.exponent == 0 && rhs.mantissa == 0) return *this;
	else if (exponent == 0 && mantissa == 0) {
		MyFloat ans (rhs);
		ans.sign = 1;
		return ans;
	}
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

bool operator<(const MyFloat& lhs, const MyFloat& rhs) {
	float f1 = lhs.packFloat(), f2 = rhs.packFloat();
	if (f1 < f2) return true;
	else return false;
}

void normalize(void) {
	
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



