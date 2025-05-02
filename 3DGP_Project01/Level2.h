#pragma once

#include "Scene.h"
#include "TankEnemy.h"

class CLevel2 : public CScene
{
public:
    CLevel2();
    virtual ~CLevel2();

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

    bool m_bShowWinMessage = false;

    void CheckPlayerEnemyCollision();
    void CheckBulletEnemyCollision();
    void CheckObstacleCollision();
    void CheckWinCondition();
    void TriggerExplosion(XMFLOAT3 position, COLORREF color);
};