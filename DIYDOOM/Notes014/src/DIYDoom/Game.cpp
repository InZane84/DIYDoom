#include "Game.h"

#include <iostream>
#include <string>

Game::Game() : m_iWindowWidth(320), m_iWindowHeight(200), m_pWindow(nullptr)
{
}

Game::~Game()
{
    delete m_pDoomEngine;
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
}

bool Game::Init()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pWindow = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_iWindowWidth, m_iWindowHeight, SDL_WINDOW_SHOWN);
    if (m_pWindow == nullptr)
    {
        std::cout << "SDL failed to create window! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);
    if (m_pRenderer == nullptr)
    {
        std::cout << "SDL failed to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pDoomEngine = new DoomEngine(m_pRenderer);

    // Set correct Logical size before initializing the engine
    if (SDL_RenderSetLogicalSize(m_pRenderer, m_pDoomEngine->GetRenderWidth(), m_pDoomEngine->GetRenderHeight()) != 0)
    {
        std::cout << "SDL failed to set logical size! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!m_pDoomEngine->Init())
    {
        std::cout << m_pDoomEngine->GetName() << " failed to initialize!" << std::endl;
        return false;
    }

    SDL_SetWindowTitle(m_pWindow, m_pDoomEngine->GetName().c_str());

    return true;
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            m_pDoomEngine->KeyPressed(event);
            break;

        case SDL_KEYUP:
            m_pDoomEngine->KeyReleased(event);
            break;

        case SDL_QUIT:
            m_pDoomEngine->Quit();
            break;
        }
    }

    m_pDoomEngine->UpdateKeyStatus(SDL_GetKeyboardState(NULL));
}

void Game::Render()
{
    m_pDoomEngine->Render();
}

void Game::Update()
{
    m_pDoomEngine->Update();
}

bool Game::IsOver()
{
    return m_pDoomEngine->IsOver();
}

void Game::Delay()
{
    SDL_Delay(m_pDoomEngine->GetTimePerFrame());
}
