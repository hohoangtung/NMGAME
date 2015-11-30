

#include "SoundManager.h"

SoundManager* SoundManager::_instance;

SoundManager* SoundManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SoundManager();
	}
	return _instance;
}

void SoundManager::loadSound(HWND hWnd)
{
	// Khởi tạo CSoundManager.
	DirectSound_Init(hWnd);

	CSound* sound = nullptr;

	sound = LoadSound("Resources//sound//BG_Map1.wav");
	_listSound[eSoundId::BACKGROUND_STAGE1] = sound;

	sound = LoadSound("Resources//sound//BG_Map2.wav");
	_listSound[eSoundId::BACKGROUND_STAGE2] = sound;
	
	sound = LoadSound("Resources//sound//BG_Map3.wav");
	_listSound[eSoundId::BACKGROUND_STAGE3] = sound;

	sound = LoadSound("Resources//sound//Attack_Cannon.wav");
	_listSound[eSoundId::ATTACK_CANNON] = sound;

	sound = LoadSound("Resources//sound//Base_Bullet.wav");
	_listSound[eSoundId::BASE_BULLET_FIRE] = sound;

	sound = LoadSound("Resources//sound//FBullet.wav");
	_listSound[eSoundId::FBULLET_FIRE] = sound;

	sound = LoadSound("Resources//sound//SBullet.wav");
	_listSound[eSoundId::SBULLET_FIRE] = sound;

	sound = LoadSound("Resources//sound//MBullet.wav");
	_listSound[eSoundId::MBULLET_FIRE] = sound;
	
	sound = LoadSound("Resources//sound//LBullet.wav");
	_listSound[eSoundId::LBULLET_FIRE] = sound;

	sound = LoadSound("Resources//sound//Boom.wav");
	_listSound[eSoundId::BOOM] = sound;

	sound = LoadSound("Resources//sound//Boss_1.wav");
	_listSound[eSoundId::BOSS_SOUNG1] = sound;

	sound = LoadSound("Resources//sound//Dead.wav");
	_listSound[eSoundId::DEAD] = sound;

	sound = LoadSound("Resources//sound//Destroy_Boss.wav");
	_listSound[eSoundId::DESTROY_BOSS] = sound;

	sound = LoadSound("Resources//sound//Destroy_Bridge.wav");
	_listSound[eSoundId::DESTROY_BRIDGE] = sound;

	sound = LoadSound("Resources//sound//Destroy_Enemy.wav");
	_listSound[eSoundId::DESTROY_ENEMY] = sound;

	sound = LoadSound("Resources//sound//Bridge_Burn.wav");
	_listSound[eSoundId::BRIDGE_BURN] = sound;

	sound = LoadSound("Resources//sound//Eat_Item.wav");
	_listSound[eSoundId::EAT_ITEM] = sound;

	sound = LoadSound("Resources//sound//Enemy_Attack.wav");
	_listSound[eSoundId::ENEMY_ATTACK] = sound;

	sound = LoadSound("Resources//sound//Game_Over.wav");
	_listSound[eSoundId::GAME_OVER] = sound;

	sound = LoadSound("Resources//sound//GoBoss.wav");
	_listSound[eSoundId::GAMEOVER_BOSS] = sound;

	sound = LoadSound("Resources//sound//Intro.wav");
	_listSound[eSoundId::INTRO] = sound;

	sound = LoadSound("Resources//sound//Jump.wav");
	_listSound[eSoundId::JUMP_SOUND] = sound;

	sound = LoadSound("Resources//sound//Pass_Boss.wav");
	_listSound[eSoundId::PASS_BOSS] = sound;

	sound = LoadSound("Resources//sound//WinGame.wav");
	_listSound[eSoundId::WINGAME] = sound;
}

bool SoundManager::IsPlaying(eSoundId soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr)
	{
		return sound->IsSoundPlaying();
	}
	return false;
}

void SoundManager::Play(eSoundId soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr)
	{
		PlaySound(sound);
		this->currentSound = sound;
	}
}
void SoundManager::PlayLoop(eSoundId soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr)
	{
		sound->Play(0, DSBPLAY_LOOPING);
	}
}
void SoundManager::Stop(eSoundId soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr)
	{
		StopSound(sound);
	}
}

SoundManager::SoundManager()
{
	currentSound = nullptr;
}

SoundManager::~SoundManager()
{
}