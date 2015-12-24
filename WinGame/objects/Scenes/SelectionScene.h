#ifndef __SELECTION_SCENE_H__
#define __SELECTION_SCENE_H__

#include "../Scene.h"

class SelectionScene : public Scene
{
public:
	SelectionScene();
	~SelectionScene();

	bool virtual init();
	void virtual updateInput(float dt);
	void virtual update(float dt);
	void virtual draw(LPD3DXSPRITE spriteHandle);
	void virtual release();

private:

};

#endif // !__SELECTION_SCENE_H__
