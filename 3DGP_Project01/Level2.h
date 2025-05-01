#pragma once

#include "Scene.h"
#include "TankEnemy.h"

class CLevel2 : public CScene
{
public:
    CLevel2();
    virtual ~CLevel2();

    virtual bool IsFinished() const override { return m_bSceneChange; }
    virtual int GetNextSceneID() const override { return m_nNextSceneID; }

    virtual void BuildObjects() override;
    virtual void ReleaseObjects() override;
    virtual void Animate(float fElapsedTime) override;
    virtual void Render(HDC hDCFrameBuffer) override;
    virtual void UpdateCamera(float fElapsedTime) override;
    virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
    virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

    virtual void ProcessInput(UCHAR* pKeyBuffer) override;
    virtual void ProcessMouseInput(float dx, float dy, bool bRightButton) override;

    CGameObject* PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera);
private:
    CTankPlayer* m_pPlayer = nullptr;
    std::vector<CTankEnemy*> m_pEnemies;
    std::vector<CGameObject*> m_pObstacles;
    std::vector<CExplosiveObject*> m_pExplosions;

    CTextObject* m_pWinText = nullptr;

    CGameObject* m_pLockedObject = nullptr;

    bool m_bShield = false;
    bool m_bShowWinMessage = false;

    bool m_bSceneChange = false;
    int m_nNextSceneID = -1;

    bool m_bAutoFire = false;
    float m_fAutoFireElapsed = 0.0f;   // 자동 공격 시간 누적
    float m_fAutoFireInterval = 1.0f;  // 1초에 한 발

    void CheckPlayerEnemyCollision();
    void CheckBulletEnemyCollision();
    void CheckObstacleCollision();
    void CheckWinCondition();
    void TriggerExplosion(XMFLOAT3 position, COLORREF color);
};