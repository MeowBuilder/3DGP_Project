#pragma once

#include "Scene.h"

class CMenuScene : public CScene
{
public:
	CMenuScene();
	~CMenuScene();

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual bool IsFinished() const override { return m_bExplosionFinished; }
	virtual int GetNextSceneID() const override { return m_nNextSceneID; }

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CMenuScene::ProcessInput(UCHAR* pKeyBuffer) { /* �Է� ���� */ }
	void CMenuScene::ProcessMouseInput(float dx, float dy, bool bRightButton) { /* ȸ�� ���� */ }

	virtual void UpdateCamera(float fElapsedTime) override;

	virtual CGameObject* PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera);

private:
	CPlayer* m_pPlayer = NULL;

	//CCamera* m_pSceneCamera = nullptr;

	CTextObject* m_pHitText = NULL;

	std::vector<CTextObject*> m_pTextObjects;

	std::vector<CExplosiveObject*> m_pExplosions;

	XMFLOAT3 m_xmf3PickPosition = { 0.0f, 0.0f, 0.0f };  // ���콺 ��ġ�� ���� ���� ��ǥ

	bool m_bExplosionFinished = false;
	float m_fExplosionElapsed = 0.0f;

	int m_nNextSceneID = -1;
};

