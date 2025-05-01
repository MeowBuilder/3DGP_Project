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

protected:
	CPlayer* m_pPlayer = NULL;

	int m_nNextSceneID = -1;

	bool m_bSceneFinished = false;
public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void ProcessInput(UCHAR* pKeyBuffer); // 키보드 이동
	virtual void ProcessMouseInput(float dx, float dy, bool bRightButton); // 마우스 회전

	virtual void UpdateCamera(float fElapsedTime);

	virtual bool IsFinished() const { return m_bSceneFinished; }
	virtual int GetNextSceneID() const { return m_nNextSceneID; }
};