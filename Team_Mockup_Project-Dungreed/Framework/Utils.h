#pragma once

class Utils
{
private:
	static std::mt19937 generator;

public:
	static const float PI;

	static void Init();

	static int RandomRange(int min, int max);
	static float RandomRange(float min, float max);
	static float RandomValue();	// 0.0f ~ 1.0f
	static sf::Vector2f OnUnitCircle(float angle);
	static sf::Vector2f RandomOnUnitCircle();
	static sf::Vector2f RandomInUnitCircle();
	static sf::Color RandomColor(bool opaque = true);
	static sf::Vector2f RandomPointInRect(const sf::FloatRect& rect);

	// Origin
	static sf::Vector2f SetOrigin(sf::Transformable &obj, Origins preset, const sf::FloatRect bound);
	static sf::Vector2f SetOrigin(sf::Shape& obj, Origins preset);
	static sf::Vector2f SetOrigin(sf::Text& obj, Origins preset);
	static sf::Vector2f SetOrigin(sf::Sprite& obj, Origins preset);

	// Math
	static float Clamp(float value, float min, float max);
	static float Clamp01(float value);

	static float Lerp(float min, float max, float t, bool clamp = true);
	static sf::Vector2f Lerp(const sf::Vector2f& min, const sf::Vector2f& max, float t, bool clamp = true);
	static sf::Color Lerp(const sf::Color& min, const sf::Color& max, float t, bool clamp = true);

	static float SqrMagnitude(const sf::Vector2f& vec);
	static float Magnitude(const sf::Vector2f& vec);

	static void Normailize(sf::Vector2f& vec);
	static sf::Vector2f GetNormal(const sf::Vector2f& vec);
	static float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	static float RadianToDegree(float radian);
	static float DegreeToRadian(float degree);

	static float AngleRadian(const sf::Vector2f& vec);
	static float Angle(const sf::Vector2f& vec);

	static float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

	/// <summary>
	/// A 직선과 B 직선의 교차여부 반환,
	/// </summary>
	static bool LineIntersect(const sf::Vector2f& a1,const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2, sf::Vector2f& result);

	static sf::Vector2f GetCenter(const sf::FloatRect& rect);
	static sf::Vector2f GetCenter(const sf::Transformable& transformable, const sf::FloatRect& localBounds);
	
	// 충돌
	static bool CheckCollision(const HitBox& boxA, const HitBox& boxB);
	static bool CheckCollision(const HitBox& boxA, const HitBox& boxB, CollisionState& collisionState);
	static bool CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB);
	static bool CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB, CollisionState& collisionState);
	static bool CheckCollision(const sf::Sprite& shapeA, const sf::Sprite& shapeB);
	static bool PointInTransformBounds(const sf::Transformable& transformable,
		const sf::FloatRect& localBounds, const sf::Vector2f point);

	static std::vector<sf::Vector2f> GetShapePoints(const sf::RectangleShape& shape);
	static std::vector<sf::Vector2f> GetShapePoints(const sf::Sprite& shape);

	static std::vector<sf::Vector2f> GetRectanglePointsFromBounds(const sf::FloatRect& localBounds);
	static bool PolygonsIntersect(const std::vector<sf::Vector2f>& polygonA, const sf::Transform& transformA,
		const std::vector<sf::Vector2f>& polygonB, const sf::Transform& transformB);
};

