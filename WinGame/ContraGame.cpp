#include "ContraGame.h"

ContraGame::ContraGame(HINSTANCE hInstance, LPWSTR title) : Game(hInstance, title)
{

}

ContraGame::~ContraGame()
{
}

void ContraGame::init()
{
	Game::init();
	// init game
	SceneManager::getInstance()->addScene(new PlayScene());
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
}
