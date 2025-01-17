#include "stdafx.h"
#include "SceneDev1.h"
#include "Player.h"
#include "Room.h";

SceneDev1::SceneDev1() : Scene(SceneIds::Dev1)
{
}

void SceneDev1::Init()
{
	player = AddGo(new Player("player"));

	Scene::Init();
}

void SceneDev1::Enter()
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

void SceneDev1::Exit()
{
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);

	//if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	//{
	//	SaveDataV1 test;
	//	test.status.level = 1;
	//	SaveLoadMgr::Instance().SaveV1(test);
	//}

	//if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	//{
	//	SaveDataVC test = SaveLoadMgr::Instance().Load();

	//	SavePlayerStatus sksldffkldsm = test.status;
	//	std::cout << test.version << test.status.level << std::endl;
	//}
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
