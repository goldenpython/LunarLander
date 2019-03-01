#pragma once

#include "IGraphics.h"

#include <irrlicht.h>

class IrrGraphics : public IGraphics {
	public:
		IrrGraphics(irr::video::IVideoDriver *poDevice);

		virtual void SetColor(int nR, int nG, int nB);
		virtual void FillRect(int nX, int nY, int nWidth, int nHeight);
		virtual void DrawLine(int nXFrom, int nYFrom, int nXTo, int nYTo);
		virtual void FillTriangle(int nX1, int nY1, int nX2, int nY2, int nX3, int nY3);

		virtual int GetScreenWidth();
		virtual int GetScreenHeight();

	private:
		irr::video::IVideoDriver *m_poDriver;
		irr::video::SColor m_oCurrentColor;
};