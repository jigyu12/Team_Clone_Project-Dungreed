#include "stdafx.h"
#include "MainTitleUi.h"

MainTitleUi::MainTitleUi(const std::string& name)
	:GameObject(name)
{
}

void MainTitleUi::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	SetPosition(position);
}

void MainTitleUi::SetRotation(float angle)
{
	rotation = angle;
	SetRotation(rotation);
}

void MainTitleUi::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	SetScale(this->scale);
}

void MainTitleUi::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
	}
}

void MainTitleUi::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
}

void MainTitleUi::Init()
{
	sortingLayer = SortingLayers::UI;
	
}

void MainTitleUi::Release()
{
	
}

void MainTitleUi::Reset()
{
	animator.SetTarget(&mainbody);
	mainbody.setPosition({1920.f/2,1080.f/2 });
	mainbody.setScale({ 3.f,3.f });
	animator.Play("animations/MainTitle.csv");

	Utils::SetOrigin(mainbody, Origins::MC);
}

void MainTitleUi::Update(float dt)
{
	animator.Update(dt);
	
}

void MainTitleUi::Draw(sf::RenderWindow& window)
{
	window.draw(mainbody);
}


