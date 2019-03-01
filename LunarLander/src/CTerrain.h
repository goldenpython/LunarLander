#pragma once

#include "IGraphics.h"
#include <vector>
#include "CLine.h"

using namespace std;

class CTerrain {
	public:
		CTerrain(float fMapWidth, float fMapHeight, float fSegmentSize);
		void Generate();
		void Draw(IGraphics *poGraphics, const CVector2D<float> &roCameraPos);
		void Update(float fFrameDeltaTimeInSeconds);
		float GetWindSpeed() const;

		class CTerrainLineIterator {
			public:
				bool HasMoreElements() const;
				CLine GetNextElement();
			private:
				CTerrainLineIterator(int nIdexMin, int nIndexMax, CTerrain &roTerrain);
				CTerrain &m_roTerrain;
				int m_nIndexMin, m_nIndexMax, m_nCurrentIndex;

				friend class CTerrain;
		};

		CTerrainLineIterator GetCollisionLines(float fMinX, float fMaxX);
	private:
		float m_fMapWidth, m_fMapHeight, m_fSegmentSize;
		vector<float> m_oHeightMap;
		float m_fWindSpeed, m_fTime;
};