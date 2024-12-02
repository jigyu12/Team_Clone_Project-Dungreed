#include "stdafx.h"
#include "Room.h"
#include "TileMap.h"
#include "Player.h"
#include "MapObject.h"
#include "Monster.h"
#include "Bat.h"
#include "SkeletonDog.h"
#include "Scene.h"

Room::Room(const std::string& name)
	: GameObject(name)
{
}

void Room::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	for (auto tileMap : tileMaps)
	{
		tileMap->SetPosition(position);
	}
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setPosition(tileMaps[0]->GetTransform().transformPoint(hitBox.second.origin));
	}
	for (auto& obj : objects)
	{
		obj.first->SetPosition(position);
	}
}

void Room::SetRotation(float angle)
{
	rotation = angle;

	for (auto tileMap : tileMaps)
	{
		tileMap->SetRotation(rotation);
	}
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setRotation(rotation);
	}
	for (auto& obj : objects)
	{
		obj.first->SetRotation(rotation);
	}
}

void Room::SetScale(const sf::Vector2f& s)
{
	scale = s;

	for (auto tileMap : tileMaps)
	{
		tileMap->SetScale(scale);
	}
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setScale(scale);
	}
	for (auto& obj : objects)
	{
		obj.first->SetScale(scale);
	}
}

void Room::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		Utils::SetOrigin(subBackground, originPreset);
		for (auto tileMap : tileMaps)
		{
			tileMap->SetOrigin(originPreset);
		}
		origin = tileMaps[0]->GetOrigin();
		//for (auto& hitBox : hitBoxes)
		//{
		//	hitBox.first->rect.setOrigin(origin);
		//}
		for (auto& obj : objects)
		{
			obj.first->SetOrigin(origin);
		}
	}
}

void Room::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	for (auto tileMap : tileMaps)
	{
		tileMap->SetOrigin(origin);
	}
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setOrigin(origin);
	}
}

void Room::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = 0;

	for (int i = 0; i < MapData::TileMapCount; ++i)
	{
		TileMap* tileMap = new TileMap("tileMap" + std::to_string(i));
		tileMaps.push_back(tileMap);
	}
}

void Room::Release()
{
	for (auto& hitbox : hitBoxes)
	{
		delete hitbox.first;
	}
	hitBoxes.clear();

	for (auto& obj : objects)
	{
		delete obj.first;
	}
	objects.clear();

	for (auto tileMap : tileMaps)
	{
		delete tileMap;
	}
	tileMaps.clear();


}

void Room::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	scene = SCENE_MGR.GetCurrentScene();
	subBackground.setTexture(TEXTURE_MGR.Get("graphics/map/SubBG.png"));
	SetOrigin(Origins::MC);
}

void Room::Update(float dt)
{
	if (player != nullptr)
	{
		subBGCenter = player->GetPosition();
		subBGCenter.x = Utils::Clamp(subBGCenter.x, viewbounds.left, viewbounds.left + viewbounds.width);
		subBGCenter.y = Utils::Clamp(subBGCenter.y, viewbounds.top, viewbounds.top + viewbounds.height);

		subBackground.setPosition(subBGCenter);
		for (auto& hitbox : hitBoxes)
		{
			if (hitbox.second.type > HitBoxData::Type::PortalRight)
			{
				continue;
			}
			if (Utils::PointInTransformBounds(hitbox.first->rect, hitbox.first->rect.getLocalBounds(), player->GetHitBox().GetCenter()))
			{
				if (ROOM_MGR.RoomChange(hitbox.second.type))
				{
					for (Monster* monster : monsters)
					{
						monster->SetActive(false);
					}
				}
				break;
			}
		}
	}
}

void Room::Draw(sf::RenderWindow& window)
{
	window.draw(subBackground);
	for (auto tileMap : tileMaps)
	{
		tileMap->Draw(window);
	}

	for (auto& obj : objects)
	{
		obj.first->Draw(window);
	}
	if (Variables::isDrawHitBox)
	{
		for (auto& hitBox : hitBoxes)
		{
			window.draw(hitBox.first->rect);
		}
	}
}

void Room::LoadMapData(const std::string& path)
{
	sf::Vector2f worldViewSize = SCENE_MGR.GetCurrentScene()->GetWorldView().getSize();

	monsters.clear();

	for (auto& hitbox : hitBoxes)
	{
		delete hitbox.first;
	}
	hitBoxes.clear();

	mapData = MapDataLoader::Load(path);

	for (int i = 0; i < tileMaps.size(); ++i)
	{
		tileMaps[i]->Set(mapData.tileMapData[i]);
	}
	
	for (const ObjectData& objData : mapData.objectData)
	{
		MapObject* obj = new MapObject();

		obj->Init();
		obj->Reset();
		obj->Set(objData.type);

		objects.push_back({ obj,objData });
	}

	for (const HitBoxData& hitBoxDatum : mapData.hitBoxData)
	{
		HitBox* hitbox = new HitBox();

		hitbox->rect.setSize(hitBoxDatum.size);
		hitbox->rect.setPosition(hitBoxDatum.origin);
		//hitbox->rect.setOrigin(-hitBoxDatum.origin);
		hitbox->rect.setRotation(hitBoxDatum.rotation);
		switch (hitBoxDatum.type)
		{
		case HitBoxData::Type::PortalUp:
		case HitBoxData::Type::PortalDown:
		case HitBoxData::Type::PortalLeft:
		case HitBoxData::Type::PortalRight:
			hitbox->rect.setOutlineColor(sf::Color::Cyan);
			break;
		case HitBoxData::Type::Immovable:
			hitbox->rect.setOutlineColor(sf::Color::Red);
			break;
		case HitBoxData::Type::Downable:
			hitbox->rect.setOutlineColor(sf::Color::Yellow);
			break;
		default:
			break;
		}
		hitBoxes.push_back({ hitbox,hitBoxDatum });
	}

	Scene* scene = SCENE_MGR.GetCurrentScene();

	if (path == "maps/1fenter1LR.json" && scene != nullptr)
	{
		sf::FloatRect bounds = tileMaps[0]->GetGlobalBounds();

		Bat* bat = scene->AddGo(new Bat());
		bat->Init();
		bat->Reset();
		bat->SetActive(false);
		bat->SetPosition({ Utils::RandomRange(bounds.left,bounds.left + bounds.width), Utils::RandomRange(bounds.top,bounds.top + bounds.height) });
		monsters.push_back(bat);

		SkeletonDog* skeletonDog = scene->AddGo(new SkeletonDog());
		skeletonDog->Init();
		skeletonDog->Reset();
		skeletonDog->SetActive(false);
		skeletonDog->SetPosition({ 40.f, 0.f });
		monsters.push_back(skeletonDog);
	}

	SetOrigin(originPreset);
	SetPosition(position);

	viewbounds = tileMaps[0]->GetGlobalBounds();

	viewbounds.left += worldViewSize.x * 0.5f;
	viewbounds.top += worldViewSize.y * 0.5f;
	viewbounds.width -= worldViewSize.x;
	viewbounds.height -= worldViewSize.y;
}

const std::vector<std::pair<HitBox*, HitBoxData>>& Room::GetHitBoxes() const
{
	return hitBoxes;
}

void Room::EnterRoom(HitBoxData::Type connection)
{
	if (player != nullptr)
	{
		player->SetPosition(mapData.playerStartPoint[(int)connection]);
	}

	for (Monster* monster : monsters)
	{
		monster->SetActive(true);
	}
}