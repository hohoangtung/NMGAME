#include "Scene.h"

Scene::Scene()
{
	//600 là chi?u cao màn hình, ?? t?m
	_viewport = new Viewport(0, 600);
}

Scene::~Scene()
{
	delete _viewport;
	_viewport = nullptr;
}

void Scene::setViewport(Viewport * viewport)
{
	if (_viewport != viewport)
		_viewport = viewport;
}

Viewport * Scene::getViewport()
{
	return _viewport;
}
