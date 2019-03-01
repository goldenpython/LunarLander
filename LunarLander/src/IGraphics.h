#pragma once

class IGraphics {
	public:
		virtual void SetColor(int nR, int nG, int nB) = 0;
		virtual void FillRect(int nX, int nY, int nWidth, int nHeight) = 0;
		void FillCenteredRect(int nX, int nY, int nWidth, int nHeight) { FillRect(nX - nWidth / 2, nY - nHeight / 2, nWidth, nHeight); }
		virtual void DrawLine(int nXFrom, int nYFrom, int nXTo, int nYTo) = 0;
		virtual void FillTriangle(int nX1, int nY1, int nX2, int nY2, int nX3, int nY3) = 0;

		virtual int GetScreenWidth() = 0;
		virtual int GetScreenHeight() = 0;
};