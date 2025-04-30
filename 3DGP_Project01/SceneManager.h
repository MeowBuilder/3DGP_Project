#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "StartScene.h"
#include "MenuScene.h"
#include "Level1.h"
#include "Level2.h"

class CSceneManager
{
private:
    CScene* m_pCurrentScene = nullptr;
public:
    CSceneManager() = default;
    ~CSceneManager();

    void SetCurrentScene(CScene* pScene);
    CScene* GetCurrentScene() const { return m_pCurrentScene; }

    void ChangeScene(int nSceneID);

    void Animate(float fElapsedTime);
    void Render(HDC hDCFrameBuffer);
    void ReleaseCurrentScene();

    void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
    void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};