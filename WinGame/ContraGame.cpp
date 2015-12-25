#include "ContraGame.h"
#include "objects\GameOverScene.h"
#include "objects\BeginState3Scene.h"
#include "objects\Stage3.h"
ContraGame::ContraGame(HINSTANCE hInstance, LPWSTR title) : Game(hInstance, title, WINDOW_WIDTH, WINDOW_HEIGHT)
{

}

ContraGame::~ContraGame()
{
}

void ContraGame::init()
{
	Game::init();
	// init game
#if _DEBUG
	//SceneManager::getInstance()->addScene(new Stage3());
	SceneManager::getInstance()->addScene(new PlayScene());
#else
	SceneManager::getInstance()->addScene(new IntroScene());
#endif
}

void ContraGame::release()
{
	Game::release();

	// release game
	SceneManager::getInstance()->clearScenes();
}

void ContraGame::updateInput(float deltatime)
{
	SceneManager::getInstance()->updateInput(deltatime);
}

void ContraGame::update(float deltatime)
{
	SceneManager::getInstance()->update(deltatime);
}

void ContraGame::draw()
{
	this->_spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);

	SceneManager::getInstance()->draw(_spriteHandle);

	this->_spriteHandle->End();
}

void ContraGame::loadResource()
{
	// Game::init đã gọi hàm này rồi nên không cần gọi lại ContraGame::loadResource
	// load resource
	SpriteManager::getInstance()->loadResource(_spriteHandle);
	SoundManager::getInstance()->loadSound(Game::hWindow->getWnd());

}
