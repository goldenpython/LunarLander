#pragma once

#include <stdio.h>
#include "vector2D.h"

class CLine {
	public:
		CLine(float fX1, float fY1, float fX2, float fY2);
		// ~CLine() { static int jjj = 0; printf("CLine destructor %d\n", jjj++); };
		bool IntersectsWith(const CLine &roLine, CVector2D<float> &roIntersection) const;

		float m_fX1, m_fY1, m_fX2, m_fY2;
};