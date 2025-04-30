#include "SceneManager.h"

CSceneManager::~CSceneManager()
{
    ReleaseCurrentScene();
}

void CSceneManager::SetCurrentScene(CScene* pScene)
{
    ReleaseCurrentScene();
    m_pCurrentScene = pScene;
    if (m_pCurrentScene) m_pCurrentScene->BuildObjects();
}

void CSceneManager::ChangeScene(int nSceneID)
{
    if (nSceneID == -1) return;

    switch (nSceneID)
    {
    case 0:
        SetCurrentScene(new CStartScene());
        break;
    case 1:
        SetCurrentScene(new CMenuScene());
        break;
    case 2:
        SetCurrentScene(new CLevel1());
        break;
    case 3:
        SetCurrentScene(new CLevel2());
        break;
    case 4:
        SetCurrentScene(new CScene());
        break;
    default:
        break;
    }
}

void CSceneManager::Animate(float fElapsedTime)
{
    if (m_pCurrentScene) m_pCurrentScene->UpdateCamera(fElapsedTime);
    if (m_pCurrentScene) m_pCurrentScene->Animate(fElapsedTime);

    if (m_pCurrentScene && m_pCurrentScene->IsFinished()) {

        ChangeScene(m_pCurrentScene->GetNextSceneID());
    }
}

void CSceneManager::Render(HDC hDCFrameBuffer)
{
    if (m_pCurrentScene) m_pCurrentScene->Render(hDCFrameBuffer);
}

void CSceneManager::ReleaseCurrentScene()
{
    if (m_pCurrentScene)
    {
        m_pCurrentScene->ReleaseObjects();
        delete m_pCurrentScene;
        m_pCurrentScene = nullptr;
    }
}

void CSceneManager::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    if (m_pCurrentScene)
        m_pCurrentScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
}

void CSceneManager::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    if (m_pCurrentScene)
        m_pCurrentScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
}