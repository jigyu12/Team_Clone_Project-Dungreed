#include "stdafx.h"
#include "SceneDev2.h"
#include "Bat.h"
#include "Room.h"

SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{

}

void SceneDev2::Init()
{
	{
		room = AddGo(new Room("tilemap"));
	}
	{
		player = AddGo(new Player());
	}
	{
		Bat* bat = AddGo(new Bat());
		bat->SetPosition({ 140.f, -80.f });
		batList.push_back(bat);
	}

	Scene::Init();
}

void SceneDev2::Enter()
{
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	Scene::Enter();
}

void SceneDev2::Exit()
{
	Scene::Exit();
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
