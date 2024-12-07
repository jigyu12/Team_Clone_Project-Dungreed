#include "stdafx.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Bat.h"
#include "SkeletonDog.h"
#include "Weapon.h"
#include "ShortSword.h"
#include "HandCrossbow.h"
#include "PlayerUi.h"
#include "Room.h"
#include "SkellBoss.h"
#include "SkellBossLeftHand.h"
#include "ParticleGo.h"
#include "LightGo.h"
#include "UiAbility.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	{
		player = AddGo(new Player("Player"));

		// 두 무기 중에 하나는 SetWeaponToWeaponSlot->true로 켜져 있어야 시작 무기로 사용됨 
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
	/*{
		skellBoss = AddGo(new SkellBoss());
		skellBoss->SetPosition({ -7.5f, 50.f });

		{
			leftHand = AddGo(new SkellBossLeftHand("SkellBossLeftHand"));
			leftHand->SetPosition({ -107.5f , 50.f });
		}
		{
			rightHand = AddGo(new SkellBossLeftHand("SkellBossRightHand"));
			rightHand->SetPosition({ 92.5f , -50.f });
			rightHand->SetScale({ -1.f, 1.f });
		}
	}*/
	{
		PlayerUi* playerui = AddGo(new PlayerUi("playerUi"));
	}
	/*{
		Bat* bat = AddGo(new Bat());
		bat->SetPosition({ 140.f, -80.f });
		batList.push_back(bat);
	}
	{
		SkeletonDog* skeletonDog = AddGo(new SkeletonDog());
		skeletonDog->SetPosition({ 0.f, 0.f });
		skeletonDogList.push_back(skeletonDog);
	}*/

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	


	Scene::Enter();
	
	
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);
	ROOM_MGR.Reset();
}

void SceneGame::Exit()
{
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
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		uiAbility->SetActive(true);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	window.clear({ 51,49,67 });
	
	Scene::Draw(window);
	

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