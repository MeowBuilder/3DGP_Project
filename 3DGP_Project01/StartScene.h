#pragma once

#include "Scene.h"

class CStartScene : public CScene
{
public:
	CStartScene();
	~CStartScene();

private:
	CPlayer* m_pPlayer = NULL;

	//CCamera* m_pSceneCamera = nullptr;

	CTextObject3D* m_pHitText = NULL;

	std::vector<CTextObject3D*> m_pTextObjects;

	std::vector<CExplosiveObject*> m_pExplosions;

	 XMFLOAT3 m_xmf3PickPosition = { 0.0f, 0.0f, 0.0f };  // 마우스 위치에 따른 점의 좌표

	 bool m_bExplosionFinished = false;
	 float m_fExplosionElapsed = 0.0f;

	 int m_nNextSceneID = -1;

#ifdef _WITH_DRAW_AXIS
	CGameObject* m_pWorldAxis = NULL;
#endif

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	bool IsFinished() const override { return m_bExplosionFinished; }

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CStartScene::ProcessInput(UCHAR* pKeyBuffer) { /* 입력 없음 */ }
	void CStartScene::ProcessMouseInput(float dx, float dy, bool bRightButton) { /* 회전 없음 */ }

	virtual void UpdateCamera(float fElapsedTime) override;

	virtual CGameObject* PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera);

	int GetNextSceneID() const override
	{
		return m_nNextSceneID;
	}
};

