#include "IrrGraphics.h"
#include <irrlicht.h>

IrrGraphics::IrrGraphics(irr::video::IVideoDriver *m_poDriver) 
	: m_poDriver(m_poDriver), m_oCurrentColor(255, 0, 0, 0) {
}

void IrrGraphics::SetColor(int nR, int nG, int nB) {
	m_oCurrentColor = irr::video::SColor(255, nR, nG, nB);
}

void IrrGraphics::FillRect(int nX, int nY, int nWidth, int nHeight) {
	m_poDriver->draw2DRectangle(m_oCurrentColor, irr::core::rect<irr::s32>(nX, nY, nX + nWidth, nY + nHeight));
}

void IrrGraphics::DrawLine(int nXFrom, int nYFrom, int nXTo, int nYTo) {
	m_poDriver->draw2DLine(irr::core::vector2d<irr::s32>(nXFrom, nYFrom), irr::core::vector2d<irr::s32>(nXTo, nYTo), m_oCurrentColor);
}

void IrrGraphics::FillTriangle(int nX1, int nY1, int nX2, int nY2, int nX3, int nY3) {
	float fInvSlope1 = (float)(nX2 - nX1) / (nY2 - nY1);
	float fInvSlope2 = (float)(nX3 - nX1) / (nY3 - nY1);

	float fCurX1 = (float)nX1;
	float fCurX2 = (float)nX1;

	for (int nScanLineY = nY1; nScanLineY <= nY2; nScanLineY++) {
		DrawLine((int)fCurX1, nScanLineY, (int)fCurX2, nScanLineY);
		fCurX1 += fInvSlope1;
		fCurX2 += fInvSlope2;
	}
}

int IrrGraphics::GetScreenWidth() {
	return m_poDriver->getScreenSize().Width;
}

int IrrGraphics::GetScreenHeight() {
	return m_poDriver->getScreenSize().Height;
}