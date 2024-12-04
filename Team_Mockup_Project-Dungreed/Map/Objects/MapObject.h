#pragma once

class Room;
class Scene;
class LightGo;

class MapObject : public GameObject
{
protected:
	enum class Status
	{
		Idle,
		Open,
		Close,
		Broken,
	};

	Status status;

	sf::Sprite body;
	Animator animator;

	ObjectData::Type type;

	Room* room;

public:
	MapObject(const std::string& name = "");
	~MapObject() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	virtual void Set(const ObjectData::Type& datum);
	virtual void SetStatus();
	virtual void OnDamaged(int damage);
};
