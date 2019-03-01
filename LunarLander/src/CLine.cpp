#include "CLine.h"

CLine::CLine(float fX1, float fY1, float fX2, float fY2)
	: m_fX1(fX1), m_fY1(fY1), m_fX2(fX2), m_fY2(fY2) 
{
}

bool CLine::IntersectsWith(const CLine &roLine, CVector2D<float> &roIntersection) const {
	float fDet1 = Det3(this->m_fX1, this->m_fY1, this->m_fX2, this->m_fY2, roLine.m_fX1, roLine.m_fY1);
	float fDet2 = Det3(this->m_fX1, this->m_fY1, this->m_fX2, this->m_fY2, roLine.m_fX2, roLine.m_fY2);
	if (Sgn(fDet1 * fDet2) > 0)
		return false;

	fDet1 = Det3(roLine.m_fX1, roLine.m_fY1, roLine.m_fX2, roLine.m_fY2, this->m_fX1, this->m_fY1);
	fDet2 = Det3(roLine.m_fX1, roLine.m_fY1, roLine.m_fX2, roLine.m_fY2, this->m_fX2, this->m_fY2);
	if (Sgn(fDet1 * fDet2) > 0)
		return false;


	// http://en.wikipedia.org/wiki/Line%E2%80%93line_intersection

	float x1 = roLine.m_fX1, x2 = roLine.m_fX2, y1 = roLine.m_fY1, y2 = roLine.m_fY2;
	float x3 = this->m_fX1, x4 = this->m_fX2, y3 = this->m_fY1, y4 = this->m_fY2;

	float r = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	float x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / r;
	float y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / r;

	return true;
}