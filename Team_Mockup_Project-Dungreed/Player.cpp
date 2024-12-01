#include "stdafx.h"
#include "Player.h"
#include "SceneDev1.h"
#include "SceneDev3.h"
#include "Room.h"
#include "Weapon.h"
#include "PlayerUi.h"
#include "Monster.h"

Player::Player(const std::string& name)
	:Character(name), velocity({ 0.f,0.f })
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(angle);
}

void Player::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		Utils::SetOrigin(body, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = Utils::SetOrigin(body, originPreset);
}

void Player::Init()
{
	speed = 100.f;
	jumpForce = 200.f;
	gravity = 300.f;
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 2;
	hp = 90;
}

void Player::Release()
{

}

void Player::Reset()
{

	playerui = dynamic_cast<PlayerUi*>(SCENE_MGR.GetCurrentScene()->FindGo("playerUi"));

	animator.SetTarget(&body);
	animator.Play("animations/player Idle.csv");
	/*body.setTexture(TEXTURE_MGR.Get(playerId));*/

	originalPlayerColor = body.getColor();

	hitbox.SetColor(sf::Color::Blue);

	SetPosition({ 0.f,0.f });
	SetOrigin(Origins::BC);
	SetRotation(0.f);


}

void Player::SetStatus(Status status)
{
	this->status = (status);
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	switch (status)
	{
	case Player::Status::Ground:
		break;
	case Player::Status::Jump:
		break;
	case Player::Status::Dash:
		velocity = look * dashSpeed;
		dashTimer = 0.f;
		dashCoolTimer = 0.f;
		animator.Play("animations/player Dash.csv");
		break;
	case Player::Status::DownJump:
		velocity.y = downSpeed;
		break;
	case Player::Status::Dead:
		break;
	default:
		break;

	}
}

void Player::Update(float dt)
{
	if (hp <= 0)
	{
		return;
	}


	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		SwitchWeaponSlot(sf::Keyboard::Num1);
	}
	else if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		SwitchWeaponSlot(sf::Keyboard::Num2);
	}


	animator.Update(dt);

	dashCoolTimer += dt;


	switch (status)
	{
	case Player::Status::Ground:
		UpdateGrounded(dt);
		break;
	case Player::Status::Jump:
		UpdateJump(dt);
		break;
	case Player::Status::Dash:
		UpdateDash(dt);
		break;
	case Player::Status::DownJump:
		UpdateDownJump(dt);
		break;
	case Player::Status::Dead:
		return;
	default:
		break;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::LShift) && dashCoolTimer >= 1.f)
	{
		SetStatus(Player::Status::Dash);
	}
	if (InputMgr::GetKey(sf::Keyboard::S) && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetStatus(Player::Status::DownJump);
	}



	SetPosition(position + velocity * dt);


	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseworldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseworldPos - position);
	SetOrigin(Origins::BC);


	hitbox.UpdateTr(body, { 8,12,16,21 });

}

void Player::LateUpdate(float dt)
{
	auto playerGlobalBounds = hitbox.rect.getGlobalBounds();
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);

	auto hitboxBounds = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
	bool collided = false;
	for (auto& startHitBox : hitboxBounds)
	{
		CollisionState state;
		if (Utils::CheckCollision(hitbox, *startHitBox.first, state))
		{
			if (state.Up)
			{
				switch (startHitBox.second.type)
				{
				case HitBoxData::Type::Immovable:
					if (velocity.y < 0)
						velocity.y = 50.f;
					collided = true;
					break;
				case HitBoxData::Type::Downable:
					break;
				case HitBoxData::Type::PortalDown:
					break;
				default:
					break;
				}
			}
			if (state.Down)
			{
				if (velocity.y >= 0.f && state.area.height < 5.f
					&& (state.AspectRatio() < 1.f || (startHitBox.first->rect.getRotation() != 0.f || startHitBox.first->rect.getRotation() != 360.f)))
				{
					switch (startHitBox.second.type)
					{
					case HitBoxData::Type::Immovable:
						SetStatus(Status::Ground);
						position.y = std::min(position.y, state.area.top);
						velocity.y = 0.f;
						SetPosition(position);
						collided = true;
						break;
					case HitBoxData::Type::Downable:
						if (status != Status::Dash
							&& (status != Status::DownJump || startHitBox.first != DownPlatform))
						{
							SetStatus(Status::Ground);
							position.y = std::min(position.y, state.area.top);
							velocity.y = 0.f;
							SetPosition(position);
							DownPlatform = startHitBox.first;
							collided = true;
						}
						break;
					}
				}
			}
			if (state.Right)
			{
				if (status == Status::Dash)
				{
					SetStatus(Status::Jump);
				}

				if (startHitBox.second.type == HitBoxData::Type::Immovable
					&& state.area.height > 5.f
					&& (status == Status::Jump || (state.AspectRatio() > 2.f && state.area.width < 5.f)))
				{
					if (horizontalInput >= 0)
					{
						position.x = std::min(position.x, position.x - state.area.width);
						SetPosition(position);
						collided = true;
					}
				}
			}
			if (state.Left)
			{
				if (status == Status::Dash)
				{
					SetStatus(Status::Jump);
				}
				if (startHitBox.second.type == HitBoxData::Type::Immovable
					&& state.area.height > 5.f
					&& (status == Status::Jump || (state.AspectRatio() > 2.f && state.area.width < 5.f)))
				{
					if (horizontalInput <= 0)
					{
						position.x = std::max(position.x, position.x + state.area.width);
						SetPosition(position);
						collided = true;
					}
				}
			}
		}
	}
	if (!collided && status == Status::Ground)
	{
		SetStatus(Status::Jump);
	}

	auto& gameobjects = SCENE_MGR.GetCurrentScene()->GetGameObjects();
	for (auto& gameobject : gameobjects)
	{
		if (auto* monster = dynamic_cast<Monster*>(gameobject))
		{
			if (Utils::CheckCollision(monster->GetHitBox(), hitbox))
			{
				if (!isDamaged && !isDead)
				{
					isDamaged = true;
					invincibilityTimer = 0.f;
					OnDamage(monster->GetOriginalDamage());
					sf::Color currColor = body.getColor();
					body.setColor({ currColor.r, currColor.g, currColor.b, 80 });
				}
			}
		}
	}

	if (isDamaged)
	{
		invincibilityTimer += dt;
		if (invincibilityTimer > 1.5f)
		{
			invincibilityTimer = 0.f;
			isDamaged = false;
			body.setColor(originalPlayerColor);
		}
	}

	if (hp <= 0)
	{
		isDead = true;
		playerui->SetHp(0, 90);
		animator.Play("animations/player Dead.csv");
		SetStatus(Status::Dead);
	}
}





void Player::UpdateGrounded(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.x = horizontalInput;
	velocity.x = direction.x * speed;
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		Jump();
	}
	if (horizontalInput == 0 && animator.GetCurrentClipId() != "Idle")
	{
		animator.Play("animations/player Idle.csv");
	}

	if (horizontalInput != 0 && animator.GetCurrentClipId() != "Walk")
	{
		animator.Play("animations/player Walk.csv");
	}
}

void Player::UpdateJump(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.x = horizontalInput;
	velocity.x = direction.x * speed;
	jumpTimer += dt;

	if (jumpTimer > 1.f || !InputMgr::GetKey(sf::Keyboard::Space) || velocity.y > 0.f)
	{
		velocity.y += gravity * dt;
	}
}

void Player::UpdateDownJump(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.x = horizontalInput;
	velocity.x = direction.x * speed;
	if (velocity.y > 0.f)
	{
		velocity.y += gravity * dt;
	}
}

void Player::UpdateDash(float dt)
{
	dashTimer += dt;
	if (dashTimer > 0.3f)
	{
		velocity = { 0.f,0.f };
		SetStatus(Status::Jump);
	}
}

void Player::Jump()
{
	velocity.y = -jumpForce;
	jumpTimer = 0.f;
	SetStatus(Status::Jump);
}

void Player::OnDamage(int monsterDamage)
{
	if (playerui != nullptr)
		playerui->SetHp(hp -= monsterDamage, playerui->GetMaxHp());
}

void Player::SetWeaponToWeaponSlot1(Weapon* weapon, bool isCurrentWeapon)
{
	weaponSlot1 = weapon;
	weaponSlot1->SetIsCurrentWeapon(isCurrentWeapon);
}

void Player::SetWeaponToWeaponSlot2(Weapon* weapon, bool isCurrentWeapon)
{
	weaponSlot2 = weapon;
	weaponSlot2->SetIsCurrentWeapon(isCurrentWeapon);
}

void Player::SwitchWeaponSlot(sf::Keyboard::Key key)
{
	if (key != sf::Keyboard::Num1 && key != sf::Keyboard::Num2)
		return;

	if (key == sf::Keyboard::Num1)
	{
		if (!weaponSlot1->IsCurrentWeapon())
		{
			weaponSlot1->SetIsCurrentWeapon(true);
			weaponSlot1->SetAttackSpeedAccumTime(weaponSlot1->GetAttackSpeed() - 0.5f);
			weaponSlot2->SetIsCurrentWeapon(false);
		}
	}
	else
	{
		if (!weaponSlot2->IsCurrentWeapon())
		{
			weaponSlot2->SetIsCurrentWeapon(true);
			weaponSlot2->SetAttackSpeedAccumTime(weaponSlot2->GetAttackSpeed() - 0.5f);
			weaponSlot1->SetIsCurrentWeapon(false);
		}
		
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitbox.Draw(window);
}