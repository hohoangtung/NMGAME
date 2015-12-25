
#include "BeginState3Scene.h"
#include "Stage3.h"

bool BeginStage3Scene::init()
{
	_waitscreen = SpriteManager::getInstance()->getSprite(eID::BEGIN_STAGE3);
	_waitscreen->setPosition(VECTOR2ZERO);
	_waitscreen->setScale(SCALE_FACTOR);
	_waitscreen->setOrigin(GVector2(0.0f, 1.0f));
	_highscore = HighScore::loadHighScore(HighScore::filehighscore);

	_access = new StopWatch();

	if (_score > _highscore)
	{
		_highscore = _score;
		HighScore::saveHighScore(HighScore::filehighscore, _highscore);
	}

	_textscore = new TextSprite(eID::FONTFULL, std::to_string(_score), GVector2(160, 75));
	_textscore->init();
	_textscore->setScale(SCALE_FACTOR);
	_textscore->getSprite()->setOpacity(0.7f);

	_texthighscore = new TextSprite(eID::FONTFULL, std::to_string(_highscore), GVector2(288, 150));
	_texthighscore->init();
	_texthighscore->setScale(SCALE_FACTOR);
	_texthighscore->getSprite()->setOpacity(0.7f);

	_textrest = new TextSprite(eID::FONTFULL, std::to_string(_rest), GVector2(160, 104));
	_textrest->init();
	_textrest->setScale(SCALE_FACTOR);
	_textrest->getSprite()->setOpacity(0.7f);
	return true;
}

void BeginStage3Scene::update(float dt)
{
	if(_access->isStopWatch(delaytime))
	{
		// jst 4 test
		auto play = new Stage3(this->_rest);
		SceneManager::getInstance()->replaceScene(play);
	}
}

void BeginStage3Scene::draw(LPD3DXSPRITE spriteHandle)
{
	_waitscreen->render(spriteHandle);

	// vẽ rest
	// vẽ score
	// vẽ highscore
	_textscore->draw(spriteHandle);
	_textrest->draw(spriteHandle);
	_texthighscore->draw(spriteHandle);
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

char* HighScore::filehighscore = "Resources\\highscore.txt";
int HighScore::loadHighScore(const char* fileInfoPath)
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
bool HighScore::saveHighScore(const char* fileInfoPath, int score)
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