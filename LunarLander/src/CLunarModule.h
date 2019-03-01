#pragma once

#include "IGraphics.h"
#include "CLine.h"
#include "vector2D.h"
#include <vector>

using namespace std;

class CTerrain;
class CLunarModule {
	public:
		CLunarModule();

		void Draw(IGraphics *poGraphics, bool bLeftEngineActive, bool bRightEngineActive, CVector2D<float> &roCameraPos);
		void Update(float fFrameDeltaTimeInSeconds, bool bLeftEngineActive, bool bRightEngineActive);

		float GetPosX() const;
		float GetPosY() const;
		float GetLifePercent() const;

		void SetTerrain(CTerrain *poTerrain);

		enum State { STATE_LANDED, STATE_INAIR, STATE_DEAD};

		State GetState() const;
	private:
		void ApplyLunarModuleTranformToLine(CLine &roLineOut);
		void DrawLine(IGraphics *poGraphics, const CLine &roLine, CVector2D<float> &roCameraPos);
		void DrawLineVector(IGraphics *poGraphics, const vector<CLine> &roLines, CVector2D<float> &roCameraPos);

		void CheckCollisions(float fFrameDeltaTimeInSeconds);
		bool CheckCollisionWithTerrainSegment(const vector<CLine> &roLunarModuleElements, const CLine &roTerrainLine, CVector2D<float> &roIntersectionPoint);

		void SetState(State eState);

		vector<CLine> m_oBody;
		vector<CLine> m_oEngineLeft, m_oEngineRight;
		vector<CLine> m_oLandingGear;

		enum State m_eModulestate;

		CVector2D<float> m_oCenter, m_oCenterPrev;
		CVector2D<float> m_oSpeed, m_oSpeedPrev;

		float m_fAngle, m_fAnglePrev;
		float m_fAngularSpeed, m_fAngularSpeedPrev;

		float m_fLifePercent;

		float m_fRadius;

		CTerrain *m_poTerrain;
};