template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>::CVector2D() {
	x = y = 0; 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>::CVector2D(RealNumber x, RealNumber y) { 
	this->x = x , this->y = y; 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>::CVector2D(CVector2D &v) { 
	x = v.x, y = v.y; 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>& CVector2D<RealNumber>::operator = (const CVector2D &v) { 
	x = v.x, y = v.y; 
	return *this; 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber> CVector2D<RealNumber>::operator + (const CVector2D &v) const { 
	return CVector2D(x + v.x, y + v.y); 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>& CVector2D<RealNumber>::operator += (const CVector2D &v) { 
	x += v.x, y += v.y; 
	return *this; 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>& CVector2D<RealNumber>::operator + () { 
	return *this; 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber> CVector2D<RealNumber>::operator - (const CVector2D &v) const { 
	return CVector2D(x - v.x, y - v.y); 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>& CVector2D<RealNumber>::operator -= (const CVector2D &v) {
	x -= v.x, y -= v.y; 
	return *this; 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber> CVector2D<RealNumber>::operator - () const { 
	return CVector2D(-x, -y); 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber> CVector2D<RealNumber>::operator * (const RealNumber v) const {  
	return CVector2D(x * v, y * v); 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>& CVector2D<RealNumber>::operator *= (RealNumber v) { 
	x *= v, y *= v; return *this; 
}

template <class RealNumber>	
MATH_INLINE RealNumber CVector2D<RealNumber>::operator * (const CVector2D<RealNumber> &m) const {
	return x * m.x + y * m.y;
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber> CVector2D<RealNumber>::operator / (RealNumber v) { 
	return CVector2D(x / v, y / v); 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>& CVector2D<RealNumber>::operator /= (RealNumber v) { 
	x /= v, y /= v; 
	return *this; 
}

template <class RealNumber>
MATH_INLINE bool CVector2D<RealNumber>::operator == (const CVector2D &v) const { 
	return x == v.x && y == v.y; 
}

template <class RealNumber>
MATH_INLINE bool CVector2D<RealNumber>::operator != (const CVector2D &v) const { 
	return x != v.x || y != v.y; 
}

template <class RealNumber>
MATH_INLINE RealNumber CVector2D<RealNumber>::Magnitude() const { 
	return MAGNITUDE_VECTOR_2D(x, y); 
}

template <class RealNumber>
MATH_INLINE RealNumber CVector2D<RealNumber>::Magnitude_Pow2() const { 
	return x * x + y * y;
}

template <class RealNumber>
MATH_INLINE void CVector2D<RealNumber>::Normalize() { 
	RealNumber n = sqrt(Magnitude_Pow2());
	x /= n; y /= n; 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber> CVector2D<RealNumber>::operator * (const CMatrix2x2<RealNumber> &m) const { 
	return CVector2D(x * m.M11 + y * m.M21, x * m.M12 + y * m.M22); 
}

template <class RealNumber>
MATH_INLINE CVector2D<RealNumber>& CVector2D<RealNumber>::operator *= (const CMatrix2x2<RealNumber> &m) {
	RealNumber xx = x * m.M11 + y * m.M21, yy = x * m.M12 + y * m.M22; 
	x = xx; y = yy;

	return *this;
}