#pragma once

#include "stdafx.h"
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "GraphicsPipeline.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

private:
	CPlayer* m_pPlayer = NULL;

	int m_nNextSceneID = -1;

#ifdef _WITH_DRAW_AXIS
	CGameObject* m_pWorldAxis = NULL;
#endif

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void ProcessInput(UCHAR* pKeyBuffer); // Ű���� �̵�
	virtual void ProcessMouseInput(float dx, float dy, bool bRightButton); // ���콺 ȸ��

	virtual void UpdateCamera(float fElapsedTime);

	virtual bool IsFinished() const { return false; } // �⺻�� false

	virtual int GetNextSceneID() const
	{
		return m_nNextSceneID;
	}
};