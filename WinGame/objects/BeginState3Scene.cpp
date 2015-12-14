
#include "BeginState3Scene.h"


bool BeginStage3Scene::init()
{
	_waitscreen = SpriteManager::getInstance()->getSprite(eID::BEGIN_STAGE3);
	_waitscreen->setPosition(VECTOR2ZERO);
	_waitscreen->setScale(SCALE_FACTOR);
	_waitscreen->setOrigin(GVector2(0.0f, 1.0f));
	_highscore = this->loadHighScore(filehighscore);

	_access = new StopWatch();

	if (_score > _highscore)
	{
		_highscore = _score;
		this->saveHighScore(filehighscore, _highscore);
	}
	return true;
}

void BeginStage3Scene::update(float dt)
{
	if(_access->isStopWatch(delaytime))
	{
		// jst 4 test
		auto play = new PlayScene();
		SceneManager::getInstance()->replaceScene(play);
	}
}

int BeginStage3Scene::loadHighScore(const char* fileInfoPath)
{
	FILE* file;
	file = fopen(fileInfoPath, "r");
	int high = -1;
	if (file)
	{
		while (!feof(file))
		{
			fscanf(file, "%d", &high);
		}
	}
	fclose(file);
	return high;
}
bool BeginStage3Scene::saveHighScore(const char* fileInfoPath, int score)
{
	FILE* file;
	file = fopen(fileInfoPath, "w+");
	if (file)
	{
		//fscanf(file, "%s", &high);
		fprintf(file, "%d", score);
	}
	else
	{
		return false;
	}
	fclose(file);
	return true;
}


void BeginStage3Scene::draw(LPD3DXSPRITE spriteHandle)
{
	_waitscreen->render(spriteHandle);

	// vẽ rest
	// vẽ score
	// vẽ highscore
}

void BeginStage3Scene::release()
{
	SAFE_DELETE(_waitscreen);
}

void BeginStage3Scene::updateInput(float deltatime)
{
}
BeginStage3Scene::BeginStage3Scene(int score, int rest)
{
	this->_score = score;
	this->_rest = rest;
}

BeginStage3Scene::~BeginStage3Scene()
{
}