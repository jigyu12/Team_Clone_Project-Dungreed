#include "stdafx.h"
#include "UiEditTile.h"
#include "TileMap.h"
#include "MapData.h"
#include "SceneMapEdit.h"

UiEditTile::UiEditTile(const std::string& name)
	: GameObject(name)
{
}

void UiEditTile::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiEditTile::SetRotation(float angle)
{
	rotation = angle;
}

void UiEditTile::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiEditTile::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiEditTile::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiEditTile::Init()
{
	tileMap = new TileMap();
	tileMap->Init();
}

void UiEditTile::Release()
{
}

void UiEditTile::Reset()
{
	tileMap->Reset();

	std::vector<sf::Vector2f> vectiledata;

	const std::unordered_map<int, TileDatum>& tiletable = TILE_TABLE->GetTable();

	tileIndexes.resize(10);
	for (int j = 0;j < tileIndexes.size();++j)
	{
		tileIndexes[j].resize(5);
	}
	int cnt = 0;
	for (auto& tiledata : tiletable)
	{
		int j = cnt / tileIndexes[0].size();
		int i = cnt % tileIndexes[0].size();

		tileIndexes[j][i] = tiledata.first;
		if (++cnt == tileIndexes.size() * tileIndexes[0].size())
		{
			break;
		}
	}
	selectedTile.setTexture(TEXTURE_MGR.Get("graphics/map/Map.png"));
	selectedTile.setTextureRect({ (sf::Vector2i)TILE_TABLE->Undefined.startpos , {16,16} });
	selectedTile.setPosition(50.f, 100.f);
	selectedTile.setScale({ 3.f, 3.f });

	saveButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonsave.png"));
	saveButton.setPosition(150.f, 100.f);
	saveButton.setScale(0.25f, 0.25f);

	loadButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonload.png"));
	loadButton.setPosition(250.f, 100.f);
	loadButton.setScale(0.25f, 0.25f);

	tileMap->SetTexture("graphics/map/Map.png");
	tileMap->Set({ 5,10 }, { 16.f,16.f }, tileIndexes);
	tileMap->SetScale({ 3.f, 3.f });

	tileMap->SetPosition({ 50.f,200.f });
}

void UiEditTile::Update(float dt)
{
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());
		if (tileMap->GetGlobalBounds().contains(mousepos))
		{
			sf::Vector2i tilepos = tileMap->GetTileIndex(mousepos);
			selectedTileIndex = tileIndexes[tilepos.y][tilepos.x];
			selectedTile.setTextureRect({ (sf::Vector2i)TILE_TABLE->Get(selectedTileIndex).startpos , {16,16} });
		}
		else if (saveButton.getGlobalBounds().contains(mousepos))
		{
			((SceneMapEdit*)SCENE_MGR.GetCurrentScene())->Save();
		}
		else if (loadButton.getGlobalBounds().contains(mousepos))
		{
			((SceneMapEdit*)SCENE_MGR.GetCurrentScene())->Load();

		}
	}
}

void UiEditTile::Draw(sf::RenderWindow& window)
{
	window.draw(selectedTile);
	window.draw(loadButton);
	window.draw(saveButton);
	tileMap->Draw(window);
}