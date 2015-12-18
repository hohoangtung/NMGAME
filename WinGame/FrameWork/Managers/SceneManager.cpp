#include "SceneManager.h"

SceneManager* SceneManager::_instance = nullptr;

SceneManager::~SceneManager()
{
}

SceneManager::SceneManager()
{
}

SceneManager * SceneManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SceneManager();
	}
	
	return _instance;
}

bool SceneManager::init()
{
	if (!_scenes.empty())
		_scenes.back()->init();

	return true;
}

void SceneManager::updateInput(float dt)
{
	if (!_scenes.empty())
		_scenes.back()->updateInput(dt);
}

void SceneManager::update(float dt)
{
	if (!_scenes.empty())
		_scenes.back()->update(dt);
}

void SceneManager::draw(LPD3DXSPRITE spriteHandle)
{
	if (!_scenes.empty())
		_scenes.back()->draw(spriteHandle);
}

void SceneManager::release()
{
	if (!_scenes.empty())
	{
		_scenes.back()->release();
		SAFE_DELETE(_scenes.back());
	}
}

void SceneManager::addScene(Scene * scene)
{
	_scenes.push_back(scene);
	_scenes.back()->init();
}

void SceneManager::removeScene()
{
	if (!_scenes.empty())
	{
		_scenes.back()->release();
		delete _scenes.back();
		_scenes.pop_back();
	}
}

void SceneManager::replaceScene(Scene * scene)
{
	this->removeScene();
	this->addScene(scene);
}

void SceneManager::clearScenes()
{
	while (!_scenes.empty())
	{
		_scenes.back()->release();
		delete _scenes.back();
		_scenes.pop_back();
	}
}

Scene * SceneManager::getCurrentScene()
{
	if (!_scenes.empty())
		return _scenes.back();

	return nullptr;
}
