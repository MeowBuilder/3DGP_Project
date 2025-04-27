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

    if (m_pCurrentScene) {
        m_pCurrentScene->ReleaseObjects();
        delete m_pCurrentScene;
    }

    switch (nSceneID)
    {
    case 0:
        m_pCurrentScene = new CStartScene();
        break;
    case 1:
        m_pCurrentScene = new CMenuScene();
        break;
    case 2:
        m_pCurrentScene = new CLevel1();
        break;
    case 3:
        m_pCurrentScene = new CLevel2();
        break;
    case 4:
        m_pCurrentScene = new CScene();
        break;
    default:
        break;
    }

    m_pCurrentScene->BuildObjects();
}

void CSceneManager::BuildCurrentScene()
{
    if (m_pCurrentScene) m_pCurrentScene->BuildObjects();
}

void CSceneManager::UpdateCamera(float fElapsedTime)
{
    if (m_pCurrentScene) m_pCurrentScene->UpdateCamera(fElapsedTime);
}

void CSceneManager::Animate(float fElapsedTime)
{
    if (m_pCurrentScene) m_pCurrentScene->Animate(fElapsedTime);

    // StartScene이 끝났으면 GameScene으로 전환
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