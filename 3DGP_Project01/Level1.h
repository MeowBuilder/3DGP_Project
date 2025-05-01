#pragma once

#include "Scene.h"

struct RailSegment
{
	XMFLOAT3 position;  // 위치
	XMFLOAT3 tangent;   // 진행 방향
	float rotation;     // 꼬임 정도 (0도, 90도, 180도 등)
};

class CLevel1: public CScene
{
public:
    CLevel1();
    virtual ~CLevel1();

    virtual bool IsFinished() const override { return m_bRailFinished; }
    virtual int GetNextSceneID() const override { return m_nNextSceneID; }

    virtual void BuildObjects() override;
    virtual void ReleaseObjects() override;
    virtual void Animate(float fElapsedTime) override;
    virtual void Render(HDC hDCFrameBuffer) override;
    virtual void UpdateCamera(float fElapsedTime) override;
    virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
    virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

private:
    void BuildRailSegments();
    void BuildRailObjects();
    void UpdatePlayerOnRail(float fElapsedTime);

private:
    std::vector<XMFLOAT3> m_ControlPoints;
    std::vector<RailSegment> m_RailSegments;
    std::vector<CRailObject*> m_pRailObjects;

    CPlayer* m_pPlayer = nullptr;
    int m_nCurrentRailIndex = 0;
    float m_fRailProgress = 0.0f;

    int m_nNextSceneID = -1;
    bool m_bRailFinished = false;
};

