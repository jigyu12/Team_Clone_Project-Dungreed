#include "stdafx.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Weapon.h"
#include "ShortSword.h"
#include "HandCrossbow.h"
#include "PlayerUi.h"
#include "Room.h"
#include "ParticleGo.h"
#include "LightGo.h"
#include "WorldMapUi.h"
#include "UiAbility.h"
#include "PortalEffect.h"
#include "SkellBossUi.h"
#include "UiPause.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	{
		player = AddGo(new Player("Player"));
		//
		skellBossUi = AddGo(new SkellBossUi("SkellBossUi"));
		// �� ���� �߿� �ϳ��� SetWeaponToWeaponSlot->true�� ���� �־�� ���� ����� ���� 
		{
			ShortSword* shortSword = AddGo(new ShortSword());
			weaponList.push_back(shortSword);
			shortSword->SetPosition({ 120.f, -80.f });
			shortSword->SetOwnerPlayer(player);
			player->SetWeaponToWeaponSlot1(shortSword, true);
		}
		{
			HandCrossbow* handCrossbow = AddGo(new HandCrossbow());
			weaponList.push_back(handCrossbow);
			handCrossbow->SetPosition({ -120.f, -80.f });
			handCrossbow->SetOwnerPlayer(player);
			player->SetWeaponToWeaponSlot2(handCrossbow);
		}
		uiAbility = AddGo(new UiAbility());
		uiAbility->SetActive(false);
	}
	{
		PlayerUi* playerui = AddGo(new PlayerUi("playerUi"));
	}

	worldMapUi = AddGo(new WorldMapUi("WorldMapUi"));
	uiPause = AddGo(new UiPause("uiPause"));
	uiPause->SetActive(false);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);


	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	Scene::Enter();

	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	mouseCursor.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "ShootingCursor")));
	mouseCursor.setScale(4.f, 4.f);
	Utils::SetOrigin(mouseCursor, Origins::MC);

	ROOM_MGR.Start();

	SaveDataVC data = SAVELOAD_MGR.Load();

	while (ROOM_MGR.GetCurrentFloor() <= data.status.lastFloor)
	{
		ROOM_MGR.NextFloor();
	}

	if (ROOM_MGR.GetCurrentFloor() % 2 == 1)
	{
		SOUND_MGR.PlayBgm("sound/Bgm/1.JailField.wav");
	}
	else
	{
		SOUND_MGR.PlayBgm("sound/Sfx/boss/ambience_prison.wav");
	}

	uiPause->SetActive(false);

	worldMapUi->RefreshData();
}

void SceneGame::Exit()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);
	ClearTookObject();
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	worldView.setCenter(ROOM_MGR.GetCurrentRoom()->GetSubBGCenter());

	if (InputMgr::GetKeyDown(sf::Keyboard::F5))
	{
		SCENE_MGR.ChangeScene(SceneIds::MapEdit);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		bool paused = uiPause->IsActive();
		if (paused)
		{
			FRAMEWORK.SetTimeScale(1.f);
		}
		else
		{
			FRAMEWORK.SetTimeScale(0.f);
		}
		uiPause->SetActive(!paused);
		//SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SOUND_MGR.PlaySfx("sound/Sfx/player/OpenInventory.wav");
	}
	if (InputMgr::GetKey(sf::Keyboard::C))
	{
		uiAbility->SetActive(true);
		
		
	}
	if (InputMgr::GetKeyUp(sf::Keyboard::C))
	{
		uiAbility->SetActive(false);
	}
	if (InputMgr::GetKeyUp(sf::Keyboard::F7))
	{
		ROOM_MGR.NextFloor();
	}

	mouseCursor.setPosition(ScreenToUi(InputMgr::GetMousePosition()));
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	window.clear({ 51,49,67 });
	
	Scene::Draw(window);
	
	window.draw(mouseCursor);
}


ParticleGo* SceneGame::TakeObjectParticle()
{
	ParticleGo* objectParticle = particlePool.Take();
	objectParticle->SetReturnThis([this, objectParticle]() {ReturnObjectParticle(objectParticle);});
	particles.push_back(objectParticle);

	AddGo(objectParticle);

	return objectParticle;
}

void SceneGame::ReturnObjectParticle(ParticleGo* particle)
{
	RemoveGo((GameObject*)particle);
	particles.remove(particle);
	particlePool.Return(particle);
}

LightGo* SceneGame::TakeObjectLight()
{
	LightGo* objectLight = lightPool.Take();
	lights.push_back(objectLight);
	AddGo(objectLight);
	return objectLight;
}

void SceneGame::ReturnObjectLight(LightGo* light)
{
	RemoveGo((GameObject*)light);
	lights.remove(light);
	lightPool.Return(light);
}

PortalEffect* SceneGame::TakeObjectPortalEffect()
{
	PortalEffect* portalEffect = portalEffectPool.Take();
	portalEffects.push_back(portalEffect);
	AddGo(portalEffect);
	return portalEffect;
}

void SceneGame::ReturnObjectPortalEffect(PortalEffect* portalEffect)
{
	RemoveGo((GameObject*)portalEffect);
	portalEffects.remove(portalEffect);
	portalEffectPool.Return(portalEffect);
}

void SceneGame::ClearTookObject()
{
	for (auto particle : particles)
	{
		RemoveGo(particle);
		particlePool.Return(particle);
	}
	particles.clear();
	for (auto light : lights)
	{
		RemoveGo(light);
		lightPool.Return(light);
	}
	lights.clear();
}