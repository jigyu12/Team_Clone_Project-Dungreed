#include "stdafx.h"
#include "DoorMo.h"

DoorMo::DoorMo(const std::string& name)
	: MapObject(name)
{
}

void DoorMo::Update(float dt)
{
	MapObject::Update(dt);

	hitbox.UpdateTr(body, body.getLocalBounds());
	hitbox.rect.setOutlineColor(sf::Color::Red);
}

void DoorMo::Draw(sf::RenderWindow& window)
{
	MapObject::Draw(window);
	hitbox.Draw(window);
}

void DoorMo::Set(const ObjectData::Type& type)
{
	MapObject::Set(type);
	animator.AddEvent("dooropen", 7, [this]() {SetStatus(Status::Broken);});
}

void DoorMo::SetStatus(const Status& status)
{
	MapObject::SetStatus(status);
	switch (status)
	{
	case MapObject::Status::Idle:
		SetActive(false);
		break;
	case MapObject::Status::Open:
		animator.Play(RESOURCEID_TABLE->Get("Animation", "DoorOpen"));
		break;
	case MapObject::Status::Close:
		SetActive(true);
		animator.Play(RESOURCEID_TABLE->Get("Animation", "DoorClose"));
		animator.PlayQueue(RESOURCEID_TABLE->Get("Animation", "DoorIdle"));
		SetOrigin(originPreset);
		break;
	case MapObject::Status::Broken:
		SetActive(false);
		break;
	}
	SetOrigin(Origins::BC);
}