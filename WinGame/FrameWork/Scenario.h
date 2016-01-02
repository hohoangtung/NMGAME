

#ifndef __SCENARIO__
#define __SCENARIO__

#include "StopWatch.h"
#include <queue>
using namespace std;

[event_source(native)]
class Scenario
{
public:
	Scenario(string name);
	~Scenario();

	__event void update(float milisecond, bool& isFinish);

private:

	string _name;

};

class ScenarioManager
{
public:
	ScenarioManager();
	~ScenarioManager();
	void update(float milisecond);
	void insertScenario(Scenario* scenario);
	bool isFinish();
private:

	queue<Scenario*> _drama;
};

#endif // !__SCENARIO__
