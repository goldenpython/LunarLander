#pragma once

#include "globalmath.h"

template <class RealNumber>
class CMatrix2x2;

template <class RealNumber>
class CVector2D {
	public:
		CVector2D();
		CVector2D(RealNumber x, RealNumber y);
		CVector2D(const CVector2D &v);

		CVector2D& operator = (const CVector2D &v);

		CVector2D operator + (const CVector2D &v) const;
		CVector2D& operator += (const CVector2D &v);
		CVector2D& operator + ();

		CVector2D operator - (const CVector2D &v) const;
		CVector2D& operator -= (const CVector2D &v);
		CVector2D operator - () const;

		CVector2D operator * (const RealNumber v) const;
		CVector2D& operator *= (const RealNumber v);

		CVector2D operator * (const CMatrix2x2<RealNumber> &m) const;
		CVector2D& operator *= (const CMatrix2x2<RealNumber> &m); 

		RealNumber operator * (const CVector2D<RealNumber> &m) const;

		CVector2D operator / (const RealNumber v);
		CVector2D& operator /= (const RealNumber v);

		bool operator == (const CVector2D &v) const;
		bool operator != (const CVector2D &v) const;

		RealNumber Magnitude() const;
		RealNumber Magnitude_Pow2() const;
		void Normalize();

	public:
		union {
			RealNumber M[2];
			struct {
				RealNumber x, y;
			};
		};
};

#include "vector2D_inline.h"

