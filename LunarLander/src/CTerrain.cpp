#include "CTerrain.h"

CTerrain::CTerrain(float fMapWidth, float fMapHeight, float fSegmentSize) 
	: m_fMapWidth(fMapWidth), m_fMapHeight(fMapHeight), m_fSegmentSize(fSegmentSize), m_fWindSpeed(0), m_fTime(0)
{
	Generate();
}

void CTerrain::Generate() {
	m_oHeightMap.clear();

	const float fMinHeight = 0.5f * m_fMapHeight;
	const float fMaxHeight = 0.9f * m_fMapHeight;

	m_oHeightMap.push_back(0);
	float fLastHeight = (fMinHeight + fMaxHeight) / 2;
	m_oHeightMap.push_back(fLastHeight);

	while (m_oHeightMap.size() * m_fSegmentSize < m_fMapWidth - m_fSegmentSize) {
		int nRand = rand() % 100;
		if (nRand > 30) {
			float fAngle = PI / 180 * m_oHeightMap.size() * 9;
			//fLastHeight += m_fSegmentSize * sin(fAngle)/* * cos(fAngle * 2)*/;
			fLastHeight += (sin(fAngle) > 0 ? 1 : -1) * m_fSegmentSize * ((rand() % 100) / 50.0f);

			if (fLastHeight < fMinHeight)
				fLastHeight += m_fSegmentSize;

			if (fLastHeight > fMaxHeight)
				fLastHeight -= m_fSegmentSize;
		}

		m_oHeightMap.push_back(fLastHeight);
	}

	m_oHeightMap.push_back(0);
}

void CTerrain::Draw(IGraphics *poGraphics, const CVector2D<float> &roCameraPos) {
	int nCameraIndex = (int)(roCameraPos.x / m_fSegmentSize);
	int nMinIndex = Clamp((int)nCameraIndex - 1, 1, (int)m_oHeightMap.size());
	int nMaxIndex = Clamp((int)(nCameraIndex + 2 + poGraphics->GetScreenWidth() / m_fSegmentSize), 1, (int)m_oHeightMap.size());

	poGraphics->SetColor(195, 195, 195);
	for (int i = nMinIndex; i < nMaxIndex; i++) {
		int nX1 = (int)((i - 1) * m_fSegmentSize - roCameraPos.x);
		int nY1 = (int)(m_oHeightMap[i - 1] - roCameraPos.y);
		int nX2 = (int)(i * m_fSegmentSize - roCameraPos.x);
		int nY2 = (int)(m_oHeightMap[i] - roCameraPos.y);
		// poGraphics->DrawLine((int)fX1, (int)fY1, (int)fX2, (int)fY2);

		if (nY1 > nY2) {
			poGraphics->FillRect(nX1, nY1, nX2 - nX1, (int)(m_fMapHeight - nY1));
			poGraphics->FillTriangle(nX2, nY2, nX1, nY1, nX2, nY1);
		} else if (nY1 < nY2){
			poGraphics->FillRect(nX1, nY2, nX2 - nX1, (int)m_fMapHeight - nY2);
			poGraphics->FillTriangle(nX1, nY1, nX1, nY2, nX2, nY2);
		} else {
			poGraphics->FillRect(nX1, nY1, nX2 - nX1, (int)m_fMapHeight - nY2);
		}
	}
}

CTerrain::CTerrainLineIterator CTerrain::GetCollisionLines(float fMinX, float fMaxX) {
	int nMinIndex = Clamp((int)(fMinX / m_fSegmentSize - 1), 0, (int)m_oHeightMap.size() - 2);
	int nMaxIndex = Clamp((int)(fMaxX / m_fSegmentSize + 1), 0, (int)m_oHeightMap.size() - 2);
	return CTerrain::CTerrainLineIterator(nMinIndex, nMaxIndex, *this);
	// return CTerrain::CTerrainLineIterator(0, m_oHeightMap.size() - 2, *this);
}

CTerrain::CTerrainLineIterator::CTerrainLineIterator(int nIndexMin, int nIndexMax, CTerrain &roTerrain) 
	: m_nIndexMin(nIndexMin), m_nIndexMax(nIndexMax), m_roTerrain(roTerrain), m_nCurrentIndex(nIndexMin)
{
}

bool CTerrain::CTerrainLineIterator::HasMoreElements() const {
	return m_nIndexMin < m_nIndexMax;
}

CLine CTerrain::CTerrainLineIterator::GetNextElement() {
	int nIndex = m_nIndexMin++;
	return CLine(nIndex * m_roTerrain.m_fSegmentSize, m_roTerrain.m_oHeightMap[nIndex], (nIndex + 1) * m_roTerrain.m_fSegmentSize, m_roTerrain.m_oHeightMap[nIndex + 1]);
}

void CTerrain::Update(float fFrameDeltaTimeInSeconds) {
	m_fTime += fFrameDeltaTimeInSeconds;
	m_fWindSpeed = sin(m_fTime / 10) * cos(m_fTime / 30) * sin(m_fTime / 10 * 5) * MAX_WIND_SPEED;
}

float CTerrain::GetWindSpeed() const {
	return m_fWindSpeed;
}