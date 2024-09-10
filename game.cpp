#define is_down(b) input_array->buttons[b].isDown
#define pressed(b) (input_array->buttons[b].isDown && input_array->buttons[b].changed)
#define released(b) (!input_array->buttons[b].isDown && input_array->buttons[b].changed)//defining the syntax for checking the key state

#include <math.h>
#include <vector>
#include <stdlib.h>
#include <iostream>

float arenaHalfSizeY = .45f;
float arenaHalfSizeX = .85f;
bool changeGravity = false; //when true, game has gravity

struct Platform //structure defining the platform positions relative to the center of the screen
{
	float platformPositionX = 0;
	float platformPositionY = 0;
	float platformHalfSizeX = 0;
	float platformHalfSizeY = 0;
	float bouncyness;
	unsigned int color;
};

struct Bullet
{
	float positionX = 0;
	float positionY = 0;
	float halfSizeX = 0.015f;
	float halfSizeY = 0.015f;
	float speedX;
	struct Bullet* next;
};
struct Player
{
	float positionX;
	float positionY;
	float playerHalfSizeX = 0.015f;
	float playerHalfSizeY = 0.015f;
	float speedX;
	float speedY;
	float max_speed = .8f;
	int orientation = 1;
	Bullet* bullets = NULL;
	int bulletChamber = 10;
	int maxBulletchamber = 4;
	float gasTank = 2;
	float max_gasTank = 2; //suppose liters: if we consume one liter/s we should substract one liter times the delta time for each delta time passed;
	float gas_consumption =1;
	float gas_replenish = .1f;
};

void addBullet(Player& player) {
	Bullet* newBullet = new Bullet();
	newBullet->positionX = player.positionX;
	newBullet->positionY = player.positionY;
	newBullet->speedX = (player.orientation == 1) ? 0.3f : -0.3f;
	newBullet->next = player.bullets;
	player.bullets = newBullet;
	player.bulletChamber--;
}

void removeBullets(Player& player, Bullet* bulletToRemove)
{
	Bullet* temp = bulletToRemove;
	while (temp->next != bulletToRemove)
	{
		temp = temp->next;
	}
	Bullet* remove = temp->next;
	free(remove);
	temp->next = temp->next->next;
}

void handleBullets(Player& player, float delta_time)
{
	Bullet* temp = player.bullets;
	while (temp != NULL)
	{
		temp->positionX = temp->positionX + temp->speedX * delta_time;
		temp = temp->next;
	}
}

void drawBullets(Player& player)
{
	Bullet* temp = player.bullets;
	while (temp != NULL)
	{
		createRectangleSizeRelated(temp->positionX, temp->positionY, temp->halfSizeX, temp->halfSizeY, 0xFF00FF);
		temp = temp->next;
	}
}


Player player1;

#include "physics.cpp"
void simulateGame(Input* input_array, float delta_time)
{
	// Make the platforms for the game, contained as a vector of structures
	std::vector<Platform> platforms = {
		{-0.5f, -0.30f, 0.15f,	0.02f,	0.85f,	0x00FFFF},
		{-0.3f, -0.20f, 0.15f,	0.02f,	0.65f,	0x00FFFF},
		{-0.1f, -0.10f, 0.15f,	0.02f,	0.55f,	0x00FFFF},
		{0.1f,	-0.0f,	0.15f,	0.02f,	0.3f,	0x00FFFF}
	};

	if (pressed(BUTTON_B) && player1.bulletChamber > 0)
	{
		addBullet(player1);
	}
	handleBullets(player1, delta_time);
	

	
	float accelerationX = 0.f;
	float accelerationY = 0.f;

	if (pressed(BUTTON_G))
	{
		changeGravity = !changeGravity; //changeGravity == TRUE means gravity is on. Is set to false by default
	}
	
	if (is_down(BUTTON_UP)&&player1.speedY<player1.max_speed && !changeGravity)
	{
		accelerationY += 5;
	}
	if (is_down(BUTTON_DOWN) && player1.speedY > -1 * player1.max_speed && !changeGravity)
	{
		accelerationY -= 5;
	}
	if (is_down(BUTTON_RIGHT) && player1.speedX < player1.max_speed)
	{
		accelerationX += 5;
		player1.orientation = 1;
	}
	if (is_down(BUTTON_LEFT) && player1.speedX > -1 * player1.max_speed)
	{
		accelerationX -= 5;
		player1.orientation = -1;
	}
	
	if (is_down(BUTTON_SPACE) && player1.gasTank>0)
	{
		player1.gasTank -= player1.gas_consumption * delta_time;
		if(player1.gasTank > 0) accelerationY += 4.2f;
	}
	player1.gasTank += player1.gas_replenish * delta_time;
	player1.gasTank = clampFloat(0, player1.gasTank, player1.max_gasTank);
	
	//Acceleration for each case of gravity
	accelerationX -= player1.speedX * 4.f; // resistance
	if (changeGravity && (player1.positionY - player1.playerHalfSizeY*.75f > -arenaHalfSizeY || onPlatform(&player1, platforms)))
	{
		accelerationY -= 3.f; //gravity induced acceleration
		
	}
	else accelerationY -= player1.speedY * 4.f; // friction force
	
	if (is_down(BUTTON_UP) && changeGravity && (player1.positionY - player1.playerHalfSizeY == -arenaHalfSizeY || onPlatform(&player1, platforms)))
	{
		player1.speedY = .7f;
	}
	

	player1.positionX += player1.speedX * delta_time + accelerationX * delta_time * delta_time * .5f;
	player1.positionY += player1.speedY * delta_time + accelerationY * delta_time * delta_time * .5f;
	
	player1.speedX += accelerationX * delta_time;
	player1.speedY += accelerationY * delta_time;


	//Makes sure the speed remains under the max speed, inherently above the max speed for the negative axis
	player1.speedX = clampFloat(-player1.max_speed, player1.speedX, player1.max_speed);
	player1.speedY = clampFloat(-player1.max_speed, player1.speedY, player1.max_speed);
	
	//handles collisions with the arena with gravity off
	if (player1.positionX + player1.playerHalfSizeX > arenaHalfSizeX && !changeGravity)
	{
		player1.positionX = arenaHalfSizeX - player1.playerHalfSizeX;
		player1.speedX *= -.85f;
	}
	if (player1.positionX - player1.playerHalfSizeX < -arenaHalfSizeX && !changeGravity)
	{
		player1.positionX = -arenaHalfSizeX + player1.playerHalfSizeX;
		player1.speedX *= -.85f;
	}
	if (player1.positionY + player1.playerHalfSizeY > arenaHalfSizeY && !changeGravity)
	{
		player1.positionY = arenaHalfSizeY - player1.playerHalfSizeY;
		player1.speedY *= -.85f;
	}
	if (player1.positionY - player1.playerHalfSizeY < -arenaHalfSizeY && !changeGravity)
	{
		player1.positionY = -arenaHalfSizeY + player1.playerHalfSizeY;
		player1.speedY *= -.85f;
	}
	//handles collisions with arena with gravity
	if (player1.positionX + player1.playerHalfSizeX > arenaHalfSizeX && changeGravity)
	{
		player1.positionX = arenaHalfSizeX - player1.playerHalfSizeX;
		player1.speedX *= -.25f;
	}
	if (player1.positionX - player1.playerHalfSizeX < -arenaHalfSizeX && changeGravity)
	{
		player1.positionX = -arenaHalfSizeX + player1.playerHalfSizeX;
		player1.speedX *= -.2f;
	}
	if (player1.positionY + player1.playerHalfSizeY > arenaHalfSizeY && changeGravity)
	{
		player1.positionY = arenaHalfSizeY - player1.playerHalfSizeY;
		player1.speedY *= -.2f;
	}
	if (player1.positionY - player1.playerHalfSizeY < -arenaHalfSizeY && changeGravity)
	{
		player1.positionY = -arenaHalfSizeY + player1.playerHalfSizeY;
		player1.speedY *= -.2f;
	}
	
	for (const auto& platform : platforms)
	{
		handleCollisionPointer(&player1, &platform);
	}

	clearScreen(0xD35F00);//set the background to a solid color
	createRectangleSizeRelated(0, 0, arenaHalfSizeX, arenaHalfSizeY, 0xFFC494); // Create the arena, basically a rectangle that the player wont be able to get out of
	for (const auto& platform : platforms)
	{
		createRectangleSizeRelated(platform.platformPositionX, platform.platformPositionY, platform.platformHalfSizeX, platform.platformHalfSizeY, platform.color);//pass the vector of platforms to the draw function
	}
	createRectangleSizeAndPixel(10, 10, 0.07, 0.4, 0x000000);
	createRectangleSizeAndPixel(10, 10, 0.07, player1.gasTank/player1.max_gasTank * .4f, 0xffffff);
	drawBullets(player1);
	createRectangleSizeRelated(player1.positionX, player1.positionY, player1.playerHalfSizeX, player1.playerHalfSizeY, 0xff0000);
	
};