
#include "Score.h"

int Score::_score = 0;

void Score::plusScore(int value)
{
	_score += value;
}
int Score::getScore()
{
	return _score;
}
Score::Score()
{
}

Score::~Score()
{
}
