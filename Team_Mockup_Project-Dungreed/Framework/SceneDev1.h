#pragma once
#include "Scene.h"

class Player;

struct person {
	std::string name;
	std::string address;
	int age;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(person, name, address, age)
};

class SceneDev1 : public Scene
{
protected:
	Player* player;

public:
	SceneDev1();
	~SceneDev1() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

