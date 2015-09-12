#ifndef __SPRITEMANAGER_H__
#define __SPRITEMANAGER_H__

#include <map> // map contain a pair data with key and value
// http://www.cplusplus.com/reference/map/map
#include "define.h"
#include "Sprite.h"
NS_FRAMEWORK
NS_FRAMEWORK_BEGIN
class SpriteManager
{
public:
	static SpriteManager* getInstance();
	static void release();
	
	void loadResource(LPD3DXSPRITE spritehandle);
	Sprite* getSprite(eID id);
	void releaseSprite(eID id);
	~SpriteManager(void);
private:
	LPD3DXSPRITE _spriteHandle;

	SpriteManager(void);
	static SpriteManager* _instance;
	map<eID, Sprite> _listSprite;
};
NS_FRAMEWORK_END
#endif // !__SPRITEMANAGER_H__
