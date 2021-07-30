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
	if (exponent == 0 && mantissa == 0) return rhs; // adding 0 cases
	else if (rhs.exponent == 0 && rhs.mantissa == 0) return *this;
	else if (abs(*this) == abs(rhs)) {
		if (sign != rhs.sign) return 0;
	}
	if (exponent < rhs.exponent) return (rhs + *this); // TODO: just compare exp
	
	// actual addition
	MyFloat ans;
	MyFloat x1(*this); // larger exponent
	MyFloat x2(rhs); // smaller exponent
	ans.exponent = x1.exponent;
	unsigned exp_diff = x1.exponent - x2.exponent;
	// append 1, shift
	x1.mantissa |= (1 << 23); // append implied 1
	x2.mantissa |= (1 << 23); // append implied 1
	int borrow = x2.mantissa >> (exp_diff - 1); // if 1, borrow 
	borrow &= 1;
	x2.mantissa >>= exp_diff; // exponents should be equal
	if (x1.sign == x2.sign) {
		// a + b, -a + -b
		ans.mantissa = x1.mantissa + x2.mantissa;
		ans.sign = x1.sign;
		if (ans.mantissa > 0x7fffff) { // overflow detected, carry
			ans.exponent++;
		}
		ans.exponent--;
	}
	else { // subtract
		// -a + b, a + -b
		ans.mantissa = x1.mantissa - x2.mantissa;
		ans.sign = x1.sign;
		if (borrow) {
			if (ans.mantissa == 0) {
				ans.exponent--;
			}
			ans.mantissa--;
		}
	}
	// normalize
	// make fit
	while (ans.mantissa > 0xFFFFFF) {
		ans.mantissa >>= 1;
		ans.exponent++;
	}
	// leading 1
	while (ans.mantissa < (1 << 23)) {
		ans.mantissa <<= 1;
		ans.exponent--;
	}
	// remove leading 1
	ans.mantissa &= ((1 << 23) - 1);

	// cout << ans.sign << " " << ans.exponent << " " << ans.mantissa << endl;

	return ans; //you don't have to return *this. it's just here right now so it will compile
}

MyFloat MyFloat::operator-(const MyFloat& rhs) const{
	// cout << "operator- called" << endl; // remove
	// cout << *this << " - " << rhs << endl; // remove
	if (*this == rhs) {
		return 0;
	}
	else if (rhs.sign == 1) { 	// a - -b == a + b
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
	MyFloat rhscpy(rhs);
	rhscpy.sign = 1;
	return (*this + rhscpy); //you don't have to return *this. it's just here right now so it will compile
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

bool MyFloat::operator==(const MyFloat rhs) const{
	if (sign == rhs.sign) {
		if (exponent == rhs.exponent) {
			if (mantissa == rhs.mantissa)
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

 MyFloat MyFloat::abs(const MyFloat& f) {
	MyFloat x(f);
	x.sign = 0;
	return x;
}