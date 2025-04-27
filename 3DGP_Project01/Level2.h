#pragma once

#include "Scene.h"
#include "TankEnemy.h"

class CLevel2 : public CScene
{
public:
    CLevel2();
    virtual ~CLevel2();

    bool IsFinished() const override { return m_bSceneChange; }
    int GetNextSceneID() const override { return m_nNextSceneID; }

    virtual void BuildObjects() override;
    virtual void ReleaseObjects() override;
    virtual void Animate(float fElapsedTime) override;
    virtual void Render(HDC hDCFrameBuffer) override;
    virtual void UpdateCamera(float fElapsedTime) override;
    virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
    virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

    virtual void ProcessInput(UCHAR* pKeyBuffer) override;
    virtual void ProcessMouseInput(float dx, float dy, bool bRightButton) override;

private:
    CTankPlayer* m_pPlayer = nullptr;
    std::vector<CTankEnemy*> m_pEnemies;
    std::vector<CGameObject*> m_pObstacles;

    bool m_bAutoAttack = false;
    bool m_bShield = false;
    bool m_bShowWinMessage = false;

    bool m_bSceneChange = false;
    int m_nNextSceneID = -1;

    void FirePlayerBullet();
    void CheckBulletCollisions();
    void CheckWinCondition();
};