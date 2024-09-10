#include <vector>
void handleCollisionPointer(struct Player* player,const struct Platform* platform)
{
	if (player->positionY > platform->platformPositionY && player->positionY - player->playerHalfSizeY < platform->platformPositionY + platform->platformHalfSizeY && player->positionX - player->playerHalfSizeX <platform->platformPositionX + platform->platformHalfSizeX && player->positionX + player->playerHalfSizeX > platform->platformPositionX-platform->platformHalfSizeX)
	{
		player->positionY = platform->platformPositionY + platform->platformHalfSizeY + player->playerHalfSizeY;
		if (player->speedY < 0)player->speedY *= -platform->bouncyness;
	}
	else if (player->positionY < platform->platformPositionY && player->positionY + player->playerHalfSizeY > platform->platformPositionY - platform->platformHalfSizeY && player->positionX - player->playerHalfSizeX < platform->platformPositionX + platform->platformHalfSizeX && player->positionX + player->playerHalfSizeX > platform->platformPositionX - platform->platformHalfSizeX)
	{
		player->positionY = platform->platformPositionY - platform->platformHalfSizeY - player->playerHalfSizeY;
		player->speedY = 0;
	}

	if (player->positionX < platform->platformPositionX && player->positionX + player->playerHalfSizeX > platform->platformPositionX - platform->platformHalfSizeX && player->positionY - player->playerHalfSizeY < platform->platformPositionY + platform->platformHalfSizeY && player->positionY + player->playerHalfSizeY > platform->platformPositionY - platform->platformHalfSizeY)
	{
		player->positionX = platform->platformPositionX - platform->platformHalfSizeX - player->playerHalfSizeX;
	}
	else if (player->positionX > platform->platformPositionX && player->positionX - player->playerHalfSizeX < platform->platformPositionX + platform->platformHalfSizeX && player->positionY - player->playerHalfSizeY < platform->platformPositionY + platform->platformHalfSizeY && player->positionY + player->playerHalfSizeY > platform->platformPositionY - platform->platformHalfSizeY)
	{
		player->positionX = platform->platformPositionX + platform->platformHalfSizeX + player->playerHalfSizeX;
	}
}

bool onPlatform(struct Player* player, const std::vector<Platform>& platforms)
{
	for (const auto& platform : platforms)
	{
		if (player->positionY - player->playerHalfSizeY >= platform.platformPositionY + platform.platformHalfSizeY && player->positionY - player->playerHalfSizeY <= platform.platformPositionY + platform.platformHalfSizeY + 0.005f)
		{
			if(player->positionX - player->playerHalfSizeX < platform.platformPositionX+ platform.platformHalfSizeX && player->positionX + player->playerHalfSizeX > platform.platformPositionX - platform.platformHalfSizeX)
			return true;
		}
	}
	return false;
}

int handleBulletCollisions(struct Bullet* bullet, const struct Platform* platform)
{
	if (bullet->positionY > platform->platformPositionY && bullet->positionY - bullet->halfSizeY < platform->platformPositionY + platform->platformHalfSizeY && bullet->positionX - bullet->halfSizeX <platform->platformPositionX + platform->platformHalfSizeX && bullet->positionX + bullet->halfSizeX > platform->platformPositionX - platform->platformHalfSizeX)
	{
		return 1;
	}
	else if (bullet->positionY < platform->platformPositionY && bullet->positionY + bullet->halfSizeY > platform->platformPositionY - platform->platformHalfSizeY && bullet->positionX - bullet->halfSizeX < platform->platformPositionX + platform->platformHalfSizeX && bullet->positionX + bullet->halfSizeX > platform->platformPositionX - platform->platformHalfSizeX)
	{
		return 1;
	}

	if (bullet->positionX < platform->platformPositionX && bullet->positionX + bullet->halfSizeX > platform->platformPositionX - platform->platformHalfSizeX && bullet->positionY - bullet->halfSizeY < platform->platformPositionY + platform->platformHalfSizeY && bullet->positionY + bullet->halfSizeY > platform->platformPositionY - platform->platformHalfSizeY)
	{
		return 1;
	}
	else if (bullet->positionX > platform->platformPositionX && bullet->positionX - bullet->halfSizeX < platform->platformPositionX + platform->platformHalfSizeX && bullet->positionY - bullet->halfSizeY < platform->platformPositionY + platform->platformHalfSizeY && bullet->positionY + bullet->halfSizeY > platform->platformPositionY - platform->platformHalfSizeY)
	{
		return 1;
	}
}