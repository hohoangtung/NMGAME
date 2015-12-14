#include "Scenario.h"

Scenario::Scenario(string name)
{
	this->_name = name;
}

Scenario::~Scenario()
{
}


ScenarioManager::ScenarioManager()
{
}
void ScenarioManager::insertScenario(Scenario* scenario)
{
	_drama.push(scenario);
}
void ScenarioManager::update(float milisecond)
{
	bool isFinish;

	isFinish = false;
	auto it = _drama.front();

	it->update(milisecond, isFinish);
	if (isFinish == true)
	{
		delete it;
		_drama.pop();
	}
}
bool ScenarioManager::isFinish()
{
	return (_drama.empty() == true);
}

ScenarioManager::~ScenarioManager()
{
}