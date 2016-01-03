#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <vector>
#include "../define.h"
#include "../../objects/Scenes/Scene.h"

class SceneManager
{
public:
	~SceneManager();
	static SceneManager* getInstance();

	bool init();
	void updateInput(float dt);
	void update(float dt);
	void draw(LPD3DXSPRITE spriteHandle);
	void release();

	void addScene(Scene* scene);
	void removeScene();
	void replaceScene(Scene* scene);
	void clearScenes();
	Scene* getCurrentScene();

private:
	SceneManager();
	static SceneManager* _instance;

	vector<Scene*> _scenes;
};

#endif // !__SCENE_MANAGER_H__
