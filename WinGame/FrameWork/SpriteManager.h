#ifndef __SPRITEMANAGER_H__
#define __SPRITEMANAGER_H__

#include <map> // map contain a pair data with key and value
// http://www.cplusplus.com/reference/map/map
#include <fstream>

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

	RECT getSourceRect(eID id, string name);
	void loadSpriteInfo(eID id, const char* fileInfoPath);

	// release Sprite, dont release Texture, keep Texture to use in another Sprite.
	void releaseSprite(eID id);

	// release Texture, and Sprite, only use releaseTexture if this is the last sprite.
	// - cẩn thận khi dùng hàm này. nhiều sprite có thể tham chiếu đến cùng một texture
	// - nếu release texture mà vẫn còn thể hiện nào đó tham chiếu đến thì sẽ bị lỗi.
	void releaseTexture(eID id);	// dù dễ bị lỗi nhưng hàm này vẫn cần thiết để giải phóng texture khi không còn đối tượng nào nữa.

	~SpriteManager(void);
private:
	SpriteManager(void);
	static SpriteManager* _instance;
	map<eID, Sprite*> _listSprite;
	map<eID, map<string, RECT> > _sourceRectList;
};
NS_FRAMEWORK_END
#endif // !__SPRITEMANAGER_H__
