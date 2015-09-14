#ifndef __GAMETIME_H__
#define __GAMETIME_H__

#include "define.h"
#include "TimeSpan.h"

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

class GameTime 
{
public:
	~GameTime(void);
	
	static void release();
	static GameTime* getInstance();
	void init();
	void resetLastTick();
	void updateGameTime();

	float getElapsedGameTime();					// Tổng thời gian trôi qua kể từ lần cuối gọi update
	float getTotalGameTime();					// Tổng thời gian trôi qua kể từ lúc gọi Init
private:
	static GameTime* _instance;

	TimeSpan _elapsedGameTime;
	TimeSpan _totalGameTime;
	LARGE_INTEGER	_Query;
	float			_freQuery;

	LONGLONG		startTicks;
	LONGLONG		lastTicks;
	LONGLONG		curTicks;

	GameTime(TimeSpan& elapsedGameTime);
	GameTime(void);

	void setElapsedGameTime(TimeSpan& elapsedGt);
	void setTotalGameTime(TimeSpan& totalGt);
};
typedef GameTime *pGameTime;
NS_FRAMEWORK_END

#endif // !__GAMETIME_H__
