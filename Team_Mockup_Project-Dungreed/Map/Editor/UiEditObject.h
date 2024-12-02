#pragma once

class UiEditor;

class UiEditObject : public GameObject
{
protected:
	enum class EditStatus
	{
		Create,
		Move,
	};
	EditStatus status;

	sf::RectangleShape boxWindow;

	sf::RectangleShape* selectedObject;
	std::vector<sf::RectangleShape> objectList;
	std::unordered_map<sf::RectangleShape*, ObjectData> objectData;
	UiEditor* uieditor;

	sf::Vector2f startPos;

public:
	UiEditObject(const std::string& name = "");
	~UiEditObject() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;
	void SetSize(const sf::Vector2f& size);

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void ClearObjectData();
};
