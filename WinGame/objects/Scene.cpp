#include "Scene.h"

Scene::Scene()
{

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
