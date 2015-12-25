
#include "BeginPlayScene.h"
#include "Stage3.h"

bool BeginPlayScene::init()
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

	_textStage = new TextSprite(eID::FONTFULL, "STAGE " + std::to_string(_stage), GVector2(95 * SCALE_FACTOR, 120 * SCALE_FACTOR));
	_textStage->init();
	_textStage->setScale(SCALE_FACTOR);
	_textStage->getSprite()->setOpacity(0.7f);
	_textStage->setOrigin(VECTOR2ZERO);

	auto name = "JUNGLE";

	switch (_stage)
	{
	case 1:
	{
		name = "JUNGLE";
		break;
	}
	case 3:
	{
		name = "WATERFALL";
		break;
	}
	default:
		break;
	}

	_textStageName = new TextSprite(eID::FONTFULL, name, GVector2(95 * SCALE_FACTOR, _textStage->getPosition().y + 32));
	_textStageName->init();
	_textStageName->setScale(SCALE_FACTOR);
	_textStageName->getSprite()->setOpacity(0.7f);
	_textStageName->setOrigin(VECTOR2ZERO);

	return true;
}

void BeginPlayScene::update(float dt)
{
	if(_access->isStopWatch(delaytime))
	{
		switch (_stage)
		{
		case 1:
		{
			auto play = new PlayScene();
			SceneManager::getInstance()->replaceScene(play);
			break;
		}
		case 3:
		{
			auto play = new Stage3();
			SceneManager::getInstance()->replaceScene(play);
			break;
		}
		default:
			break;
		}
	}
}

void BeginPlayScene::draw(LPD3DXSPRITE spriteHandle)
{
	_waitscreen->render(spriteHandle);

	// vẽ rest
	// vẽ score
	// vẽ highscore
	_textscore->draw(spriteHandle);
	_textrest->draw(spriteHandle);
	_texthighscore->draw(spriteHandle);
	_textStage->draw(spriteHandle);
	_textStageName->draw(spriteHandle);
}

void BeginPlayScene::release()
{
	SAFE_DELETE(_waitscreen);
	SAFE_DELETE(_texthighscore);
	SAFE_DELETE(_textrest);
	SAFE_DELETE(_textscore);
	SAFE_DELETE(_textStage);
	SAFE_DELETE(_textStageName);
}

void BeginPlayScene::updateInput(float deltatime)
{
}

BeginPlayScene::BeginPlayScene(int score, int rest, int stage)
{
	this->_score = score;
	this->_rest = rest;
	this->_stage = stage;
}

BeginPlayScene::~BeginPlayScene()
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