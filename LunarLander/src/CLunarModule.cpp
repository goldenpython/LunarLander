#include "CLunarModule.h"
#include "CTerrain.h"
#include "CLine.h"
#include "vector2D.h"
#include "IGraphics.h"

CLunarModule::CLunarModule() 
	:	m_poTerrain(NULL),
		m_oCenter(100, 100),
		m_oSpeed(0, 0),
		m_eModulestate(STATE_INAIR)
{
	const float fSize = 20;

	m_oBody.push_back(CLine(0,		-fSize,		-fSize,		+fSize));
	m_oBody.push_back(CLine(-fSize, +fSize,		+fSize,		+fSize));
	m_oBody.push_back(CLine(+fSize,	+fSize,		0,			-fSize));

	const float fFactor = 0.20f;
	const float fEngineSize = fSize * fFactor;
	m_oBody.push_back(CLine(-fSize + fEngineSize, +fSize, -fSize + fEngineSize, fSize + fEngineSize));
	m_oBody.push_back(CLine(-fSize + fEngineSize, fSize + fEngineSize, -fSize + 2 * fEngineSize, fSize + fEngineSize));
	m_oBody.push_back(CLine(-fSize + 2 * fEngineSize, fSize + fEngineSize, -fSize + 2 * fEngineSize, +fSize));

	m_oBody.push_back(CLine(+fSize - fEngineSize, +fSize, +fSize - fEngineSize, fSize + fEngineSize));
	m_oBody.push_back(CLine(+fSize - fEngineSize, fSize + fEngineSize, +fSize - 2 * fEngineSize, fSize + fEngineSize));
	m_oBody.push_back(CLine(+fSize - 2 * fEngineSize, fSize + fEngineSize, +fSize - 2 * fEngineSize, +fSize));

	m_oEngineLeft.push_back(CLine(-fSize + fEngineSize, fSize + fEngineSize * 2, -fSize + 1.5f * fEngineSize, fSize + fEngineSize * 4));
	m_oEngineLeft.push_back(CLine(-fSize + 2 * fEngineSize, fSize + fEngineSize * 2, -fSize + 1.5f * fEngineSize, fSize + fEngineSize * 4));

	m_oEngineRight.push_back(CLine(+fSize - 1 * fEngineSize, fSize + fEngineSize * 2, +fSize - 1.5f * fEngineSize, fSize + fEngineSize * 4));
	m_oEngineRight.push_back(CLine(+fSize - 2 * fEngineSize, fSize + fEngineSize * 2, +fSize - 1.5f * fEngineSize, fSize + fEngineSize * 4));

	const float fLandingGearSize = fSize * 0.75f;
	m_oLandingGear.push_back(CLine(-fSize + 4 * fEngineSize, +fSize, -fSize + 3 * fEngineSize, +fSize + fLandingGearSize));
	m_oLandingGear.push_back(CLine(+fSize - 4 * fEngineSize, +fSize, +fSize - 3 * fEngineSize, +fSize + fLandingGearSize));

	float fDx = m_oLandingGear[0].m_fX2;
	float fDy = m_oLandingGear[0].m_fY2;
	m_fRadius = sqrt(fDx * fDx + fDy * fDy);

	m_fAngle = m_fAngularSpeed = 0;
	m_fAnglePrev = m_fAngularSpeedPrev = 0;
	m_fLifePercent = 1;
}

void CLunarModule::Draw(IGraphics *poGraphics, bool bLeftEngineActive, bool bRightEngineActive, CVector2D<float> &roCameraPos) {
	poGraphics->SetColor(255, 255, 255);
	DrawLineVector(poGraphics, m_oBody, roCameraPos);
	DrawLineVector(poGraphics, m_oLandingGear, roCameraPos);

	poGraphics->SetColor(255, 0, 0);

	if (bLeftEngineActive) {
		DrawLineVector(poGraphics, m_oEngineLeft, roCameraPos);
	}

	if (bRightEngineActive) {
		DrawLineVector(poGraphics, m_oEngineRight, roCameraPos);
	}
}

void CLunarModule::DrawLineVector(IGraphics *poGraphics, const vector<CLine> &roLines, CVector2D<float> &roCameraPos) {
	for (int i = 0; i < static_cast<int>(roLines.size()); i++) {
		CLine oLineAfterTranform(roLines[i]);
		ApplyLunarModuleTranformToLine(oLineAfterTranform);
		DrawLine(poGraphics, oLineAfterTranform, roCameraPos);
	}
}

void CLunarModule::Update(float fFrameDeltaTimeInSeconds, bool bLeftEngineActive, bool bRightEngineActive) {
	if (GetState() == STATE_DEAD)
		return;

	if ((bLeftEngineActive || bRightEngineActive) && GetState() == STATE_LANDED) {
		SetState(STATE_INAIR);
	}

	m_oCenterPrev = m_oCenter;
	m_oSpeedPrev = m_oSpeed;
	m_fAnglePrev = m_fAngle;
	m_fAngularSpeedPrev = m_fAngularSpeed;

	m_oCenter += m_oSpeed;
	if (m_poTerrain && GetState() != STATE_LANDED) {
		m_oCenter += CVector2D<float>(m_poTerrain->GetWindSpeed(), 0.0f) * fFrameDeltaTimeInSeconds;
	}

	m_oSpeed.x *= pow(ANGULAR_SPEED_DECAY, fFrameDeltaTimeInSeconds);
	m_oSpeed.y += G * fFrameDeltaTimeInSeconds;

	if (bLeftEngineActive) {
		m_oSpeed.y -= POWER * fFrameDeltaTimeInSeconds * cos(m_fAngle);
		m_oSpeed.x += POWER * fFrameDeltaTimeInSeconds * sin(m_fAngle);
		m_fAngularSpeed += ANGULAR_POWER * fFrameDeltaTimeInSeconds;
	}

	if (bRightEngineActive) {
		m_oSpeed.y -= POWER * fFrameDeltaTimeInSeconds * cos(m_fAngle);
		m_oSpeed.x += POWER * fFrameDeltaTimeInSeconds * sin(m_fAngle);
		m_fAngularSpeed -= ANGULAR_POWER * fFrameDeltaTimeInSeconds;
	}

	m_fAngle += m_fAngularSpeed * fFrameDeltaTimeInSeconds;
	m_fAngularSpeed *= pow(ANGULAR_SPEED_DECAY, fFrameDeltaTimeInSeconds);

	// printf("%f\n", m_fAngle);

	if (GetState() != STATE_LANDED) {
		CheckCollisions(fFrameDeltaTimeInSeconds);
	} else {
		m_oSpeed *= 0;
	}
}

void CLunarModule::CheckCollisions(float fFrameDeltaTimeInSeconds) {
	if (!m_poTerrain)
		return;

	CTerrain::CTerrainLineIterator oIt = m_poTerrain->GetCollisionLines(m_oCenter.x - m_fRadius, m_oCenter.x + m_fRadius);

	CVector2D<float> oNewSpeed, oPosOffset;
	int nNumCollisions = 0;
	float fAngularSpeed = 0;

	while (oIt.HasMoreElements()) {
		const CLine &oLine = oIt.GetNextElement();
		CVector2D<float> oIntersectionPoint;

		bool bLandingGearCollision = CheckCollisionWithTerrainSegment(m_oLandingGear, oLine, oIntersectionPoint);
		bool bTerrainIsFlat = abs(oLine.m_fY1 - oLine.m_fY2) < MIN_VALUE;
		if (bTerrainIsFlat && bLandingGearCollision && m_oSpeed.Magnitude_Pow2() < MAX_LANDING_SPEED_POW2 && m_oSpeed.y > 0) {
			float fCosAngle = cos(m_fAngle);
			if (abs(1 - fCosAngle) < MAX_COS_ANGLE_LANDING_DIFF) {
				m_fLifePercent -= abs(m_oSpeed.y) / (DAMAGE_BY_SPEED);
				if (m_fLifePercent <= 0) {
					m_fLifePercent = 0;
					SetState(STATE_DEAD);
				} else {
					SetState(STATE_LANDED);
				}
				m_oSpeed *= 0;
				m_fAngularSpeed = m_fAngle = 0;
				return;
			}
		}

		if (bLandingGearCollision || CheckCollisionWithTerrainSegment(m_oBody, oLine, oIntersectionPoint)) {
			CVector2D<float> oNormal(oLine.m_fY2 - oLine.m_fY1, -(oLine.m_fX2 - oLine.m_fX1));
			oNormal.Normalize();

			// http://www.cosinekitty.com/raytrace/chapter10_reflection.html
			CVector2D<float> oSpeed(m_oSpeed);
			float fNormalSpeed = oNormal * oSpeed;
			m_fLifePercent -= abs(fNormalSpeed) / (DAMAGE_BY_SPEED);
			if (m_fLifePercent <= 0) {
				m_fLifePercent = 0;
				SetState(STATE_DEAD);
			}
			CVector2D<float> oReflection;
			oReflection = oSpeed - oNormal * fNormalSpeed * 2;

			oNewSpeed += oReflection;
			nNumCollisions++;

			oIntersectionPoint -= m_oCenter;
			oIntersectionPoint /= PI * m_fRadius * m_fRadius;
			fAngularSpeed += (oIntersectionPoint.x * oSpeed.y - oIntersectionPoint.y * oSpeed.x) * ANGULAR_POWER * 5;
			// fAngularSpeed += oNormal * (oIntersectionPoint - oSpeed) * ANGULAR_POWER * 2;
			//CVector2D<float> oV = oIntersectionPoint - m_oCenter;
			//oV.Normalize();
			//fAngularSpeed += oNormal * oV;

			oPosOffset += oNormal;

			// break;
			// m_fSpeedX *= -1;
			// m_fSpeedY *= -1;
		}
	}

	if (nNumCollisions > 0) {
  		m_oCenter = m_oCenterPrev;
		m_fAngularSpeed = m_fAngularSpeedPrev;
		m_fAngle = m_fAnglePrev;

		oNewSpeed /= nNumCollisions;
		m_oCenter += oPosOffset;
		oNewSpeed *= 0.4f;
		m_oSpeedPrev = m_oSpeed;
		m_oSpeed = oNewSpeed;


		m_fAngularSpeedPrev = m_fAngularSpeed;
		m_fAngularSpeed += fAngularSpeed;//
	}
}

bool CLunarModule::CheckCollisionWithTerrainSegment(const vector<CLine> &roLunarModuleElements, const CLine &roTerrainLine, CVector2D<float> &roIntersectionPoint) {
	for (int i = 0; i < static_cast<int>(roLunarModuleElements.size()); i++) {
		CLine oLineAfterTranform(roLunarModuleElements[i]);
		ApplyLunarModuleTranformToLine(oLineAfterTranform);
		CVector2D<float> oIntersection;
		if (roTerrainLine.IntersectsWith(oLineAfterTranform, oIntersection)) {
			printf("Coliziune");
			return true;
		}
	}

	return false;
}

float CLunarModule::GetPosX() const {
	return m_oCenter.x;
}

float CLunarModule::GetPosY() const {
	return m_oCenter.y;
}

float CLunarModule::GetLifePercent() const {
	return m_fLifePercent;
}

void CLunarModule::DrawLine(IGraphics *poGraphics, const CLine &roLine, CVector2D<float> &roCameraPos) {
	poGraphics->DrawLine(static_cast<int>(roLine.m_fX1 - roCameraPos.x), static_cast<int>(roLine.m_fY1 - roCameraPos.y), static_cast<int>(roLine.m_fX2 - roCameraPos.x), static_cast<int>(roLine.m_fY2 - roCameraPos.y));
}

void CLunarModule::ApplyLunarModuleTranformToLine(CLine &roLine) {
	float fX1 = cos(m_fAngle) * roLine.m_fX1 - sin(m_fAngle) * roLine.m_fY1;
	float fY1 = sin(m_fAngle) * roLine.m_fX1 + cos(m_fAngle) * roLine.m_fY1;
	roLine.m_fX1 = fX1 + m_oCenter.x;
	roLine.m_fY1 = fY1 + m_oCenter.y;

	float fX2 = cos(m_fAngle) * roLine.m_fX2 - sin(m_fAngle) * roLine.m_fY2;
	float fY2 = sin(m_fAngle) * roLine.m_fX2 + cos(m_fAngle) * roLine.m_fY2;
	roLine.m_fX2 = fX2 + m_oCenter.x;
	roLine.m_fY2 = fY2 + m_oCenter.y;
}

void CLunarModule::SetTerrain(CTerrain *poTerrain) {
	m_poTerrain = poTerrain;
}

CLunarModule::State CLunarModule::GetState() const {
	return m_eModulestate;
}

void CLunarModule::SetState(State eState) {
	m_eModulestate = eState;
}