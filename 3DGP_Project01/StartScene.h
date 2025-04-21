#pragma once

#include "Scene.h"

class CStartScene : public CScene
{
public:
	CStartScene(CPlayer* pPlayer);
	virtual ~CStartScene();

private:
	int m_nObjects = 0;
	CGameObject** m_ppObjects = NULL;

	CCamera* m_pSceneCamera = nullptr;

	CGameObject* m_pLockedObject = NULL;

	
	std::vector<CTextObject3D*> m_pTextObjects;

#ifdef _WITH_DRAW_AXIS
	CGameObject* m_pWorldAxis = NULL;
#endif

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CStartScene::ProcessInput(UCHAR* pKeyBuffer) { /* 입력 없음 */ }
	void CStartScene::ProcessMouseInput(float dx, float dy, bool bRightButton) { /* 회전 없음 */ }

	virtual void UpdateCamera(float fElapsedTime) override;

	virtual CGameObject* PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera);
};

