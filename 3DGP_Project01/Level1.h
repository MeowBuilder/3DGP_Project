#pragma once

#include "Scene.h"

struct RailSegment
{
	XMFLOAT3 position;
	XMFLOAT3 tangent;
	float rotation;
};

class CLevel1: public CScene
{
public:
    CLevel1();
    virtual ~CLevel1();

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
    std::vector<RailSegment> m_RailSegments;
    std::vector<CRailObject*> m_pRailObjects;

    int m_nCurrentRailIndex = 0;
    float m_fRailProgress = 0.0f;
    float speed = 5.0f;
    float fRotateLerpSpeed = 5.0f;
};

