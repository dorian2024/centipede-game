#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<cstdlib>
using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // 30 Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // 30 Total columns on grid 
// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock, float player[]);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void movePlayer(float player[], sf::Clock& playerClock, sf::Sprite& playerSprite);
int initialiseMushroom(float mushroom[3][42]);
void drawMushroom(sf::RenderWindow& window, sf::Sprite& mushroomSprite, sf::Sprite& halfMushroomSprite, float mushroom[3][42], int mushroomCount, sf::Sprite& poisonousMushroomSprite);
void checkMushroomBulletCollisions(float bullet[], sf::Clock& mushroomClock,  float mushroom[3][42], int mushroomCount, int* score);
void initialiseCentipede(float centipede[3][12], sf::RenderWindow& window, sf::Sprite& centipedeHeadSprite, bool direction []);
void checkCentipedePosition(float centipede[3][12], float player[], float mushroom[3][42], bool direction[], bool directionY[12], int mushroomCount);
void moveCentipede(float centipede[3][12], sf::RenderWindow& window, sf::Clock centipedeClock, bool direction[],  float mushroom[3][42], int mushroomCount, float player[],  bool directionY[12]);
void drawCentipede(float centipede[3][12], sf::RenderWindow& window, sf:: Sprite& centipedeHeadSprite, sf:: Sprite& centipedeSprite,  bool direction, bool head[12]);
void checkCentipedeBulletCollisions(float bullet[], float centipede[3][12], bool head[], float mushroom[3][42], int &mushroomCount,  sf::Clock& mushroomClock, bool direction[], int* score);
float absoluteValue(float value1, float value2);
bool checkPlayerCollision(float centipede[3][12], float player[], float mushroom[3][42], int mushroomCount);
bool checkCentipedeExists(float centipede[3][12]);

int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays. 
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	//window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/bgmusic.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/bg5.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.75)); // Reduces Opacity to 25%
	sf::Texture endGameTexture;
	sf::Sprite endGameSprite;
	endGameTexture.loadFromFile("Textures/bg5.png");
	endGameSprite.setTexture(endGameTexture);
	endGameSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%
	
	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;  //480
	player[y] = (gameColumns - 5) * boxPixelsY;  // 800
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	sf::Clock playerClock;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x]; //480
	bullet[y] = player[y] - boxPixelsY; // 800 - 32 = 768
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bullet[exists] = false;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	
	//Initialising mushrooms.	
	float mushroom[3][42]; //max mushroom 30 and 12 centipede caused mushrooms
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite;
	mushroomTexture.loadFromFile("Textures/happystar.png");
	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(sf::IntRect(0,0, boxPixelsX, boxPixelsY));
	sf::Texture halfMushroomTexture;
	sf::Sprite halfMushroomSprite;
	halfMushroomTexture.loadFromFile("Textures/coolstar.png");
	halfMushroomSprite.setTexture(halfMushroomTexture);
	halfMushroomSprite.setTextureRect(sf::IntRect(0,0, boxPixelsX, boxPixelsY));
	sf::Texture poisonousMushroomTexture;
	sf::Sprite poisonousMushroomSprite;
	poisonousMushroomTexture.loadFromFile("Textures/blackstar.png");
	poisonousMushroomSprite.setTexture(poisonousMushroomTexture);
	poisonousMushroomSprite.setTextureRect(sf::IntRect(0,0, boxPixelsX, boxPixelsY));

	//initialises random amount of mushrooms between 20 and 30
	int mushroomCount = initialiseMushroom(mushroom);
	
	sf::Clock mushroomClock;
	
	
	//Initialising centipede.
	float centipede[3][12]; 
	sf::Clock centipedeClock;
	sf::Texture centipedeTexture;
	sf::Sprite centipedeSprite;
	sf::Texture centipedeHeadTexture;
	sf::Sprite centipedeHeadSprite;
	centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
	centipedeSprite.setTexture(centipedeTexture);
	centipedeSprite.setTextureRect(sf::IntRect(0,0, boxPixelsX, boxPixelsY));
	centipedeHeadTexture.loadFromFile("Textures/c_head_left_walk.png");
	centipedeHeadSprite.setTexture(centipedeHeadTexture);
	centipedeHeadSprite.setTextureRect(sf::IntRect(0,0, boxPixelsX, boxPixelsY));
	bool direction [12];
	initialiseCentipede(centipede, window, centipedeHeadSprite, direction);
	bool head[12];
	bool directionY[12];
	
	for(int i = 0; i < 12; i ++)
	{
	head[i] = false;
	directionY[i] = true;
	} 
	head[0] = true;
	 
	
	//initialising spider
	sf::Texture spiderTexture;
	sf::Sprite spiderSprite;
	spiderTexture.loadFromFile("Textures/tarantula.png");
	spiderSprite.setTexture(spiderTexture);
	spiderSprite.setTextureRect(sf::IntRect(0,0, boxPixelsX*2, boxPixelsY*2));
	
	//initialising font
	sf::Font font;
	font.loadFromFile("Textures/Crima.ttf");
	
	sf::Text gameOverLoseText;
	gameOverLoseText.setFont(font);
	gameOverLoseText.setString("Game Over\n  You Lost!!");
	gameOverLoseText.setCharacterSize(60);
	gameOverLoseText.setFillColor(sf::Color::White);
	gameOverLoseText.setStyle(sf::Text::Bold);
	gameOverLoseText.setPosition(300, 700);
    	
    	sf::Text gameOverWinText;
	gameOverWinText.setFont(font);
	gameOverWinText.setString("Game Over\n  You Won!!");
	gameOverWinText.setCharacterSize(60);
	gameOverWinText.setFillColor(sf::Color::White);
	gameOverWinText.setStyle(sf::Text::Bold);
	gameOverWinText.setPosition(300, 700);
	
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition(50, 50);
	
	sf::Text finalScoreText;
	finalScoreText.setFont(font);
	finalScoreText.setCharacterSize(50);
	finalScoreText.setFillColor(sf::Color::White);
	finalScoreText.setStyle(sf::Text::Bold);
	finalScoreText.setPosition(50, 50);
	
	bool gameOver1 = false;
	bool gameOver2 = false;
	int score = 0;
	
	while(window.isOpen()) {

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////
		window.display();
		
		//exits when window is closed
		sf::Event e;
		while (window.pollEvent(e)) 
		{
			if (e.type == sf::Event::Closed) 
			{
				return 0;
			}
		}
		
		//display game over text and background
		if(gameOver1 || gameOver2)
		{
			window.draw(endGameSprite);
			if(gameOver1)
			{
				window.draw(gameOverLoseText);
				window.draw(finalScoreText);
			}
			if(gameOver2)
			{
				window.draw(gameOverWinText);
				window.draw(finalScoreText);
			}
		
			continue;
		}
		
		window.clear();
		
		//display game background
		window.draw(backgroundSprite);
		window.draw(scoreText);
		
		scoreText.setString("Centipede Game\nUmaima 23i-0790\nScore: "+std::to_string(score));
		finalScoreText.setString("Score "+ std::to_string(score));
		
		//move player and centipede
		movePlayer(player, playerClock, playerSprite);
		moveCentipede(centipede, window, centipedeClock, direction, mushroom, mushroomCount, player, directionY);
		
		//draw player, mushroom and centipede
		drawPlayer(window, player, playerSprite);
		drawMushroom(window, mushroomSprite, halfMushroomSprite, mushroom, mushroomCount, poisonousMushroomSprite);
		drawCentipede(centipede, window, centipedeHeadSprite, centipedeSprite, direction, head);
		
		//function to control centipede when it reaches player area
		checkCentipedePosition(centipede, player, mushroom, direction, directionY, mushroomCount);
	
		//initialise bullet when spacebar is pressed
		sf::Event event;		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bullet[exists])
		{
//			if(bullet[exists] == false)
			{			
				if (bulletClock.getElapsedTime().asMilliseconds() > 20)
				{							
						bulletClock.restart();
				}
				bullet[exists] = true;
				bullet[x] = player[x];
				bullet[y] = player[y]; // - boxPixelsY;
			}
		}
		
		if (bullet[exists] == true) 
		{
		
			//move and draw bullet
			moveBullet(bullet, bulletClock, player);
			drawBullet(window, bullet, bulletSprite);

			//check mushroom bullet collision
			checkMushroomBulletCollisions(bullet, mushroomClock, mushroom, mushroomCount, &score);		
		
			//check centipede bullet collision
			checkCentipedeBulletCollisions(bullet, centipede, head, mushroom, mushroomCount, mushroomClock, direction, &score);
		}
		
		//checks player collision with centipede and mushroom
		gameOver1 = checkPlayerCollision(centipede, player, mushroom, mushroomCount);
	
		//checks whether centipede has been killed
		gameOver2 = !checkCentipedeExists (centipede);
		cout << score << endl;
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) 
{
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

void movePlayer(float player[], sf::Clock& playerClock, sf::Sprite& playerSprite)
{
	//checks player movement rate
	if (playerClock.getElapsedTime().asMilliseconds() < 20)
		return;
		playerClock.restart();
	
	//player movement
	int move = 10; // pixels
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if((player[x] - move) >= 0)
		{
			player[x] -= move;
		}
	}
	
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if((player[x] + move) < ((gameRows - 1)*boxPixelsX))
		{
			player[x] += move;
		}	
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if((player[y] - move) >= (gameRows - 5)*boxPixelsY)
		{
			player[y] -= move;
		}
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if((player[y] + move) < ((gameColumns - 1)*boxPixelsY))
		{
			player[y] += move;
		}	
	}
	
}

void moveBullet(float bullet[], sf::Clock& bulletClock, float player[]) 
{

	if (bulletClock.getElapsedTime().asMilliseconds() < 12)
		return;
		bulletClock.restart();
		
	bullet[y] -= 10;	
	if (bullet[y] < 5*boxPixelsY)
	{
		bullet[exists] = false;
	}		
}

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) 
{

	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

int initialiseMushroom(float mushroom[3][42]) 
{
	int Count = 0;
	int totalMushrooms = (rand()% 10) + 20; //20 to 29 chosen at random
	while (Count < totalMushrooms) 
	{
		int tempX = rand() % (gameColumns-1); // the - 1 takes away the 30th value
		int tempY = rand() % (gameRows - 13) + 5; // 5 player rows and 5 rows on top for score 
		
		mushroom[x][Count] = tempX*boxPixelsX;
		mushroom[y][Count] = tempY*boxPixelsY;
		mushroom[exists][Count] = 2; //full life
		Count++;	
	}
	
	//remaining mushroom array empty
	for(int i = 0; i < (42 - totalMushrooms); i++)
	{
		mushroom[exists][Count+i] = 0;
	}	
	return totalMushrooms;
}
void drawMushroom(sf::RenderWindow& window, sf::Sprite& mushroomSprite, sf::Sprite& halfMushroomSprite, float mushroom[3][42], int mushroomCount, sf::Sprite& poisonousMushroomSprite)
{

		
	for(int j = 0; j < mushroomCount; j++)
	{	
		if(mushroom[y][j] < (gameRows - 5)*boxPixelsY)
		{	
			// full life
			if(mushroom[exists][j] == 2)
			{
				mushroomSprite.setPosition(mushroom[x][j], mushroom[y][j]);
				window.draw(mushroomSprite);
			}
		
			// half life
			if(mushroom[exists][j] == 1)
			{
				halfMushroomSprite.setPosition(mushroom[x][j], mushroom[y][j]);
				window.draw(halfMushroomSprite);
			}	
		}
		else
		{
			//poisonous mushrooms
			if(mushroom[exists][j] == 2 || mushroom[exists][j] == 1)
			{
				poisonousMushroomSprite.setPosition(mushroom[x][j], mushroom[y][j]);
				window.draw(poisonousMushroomSprite);
			}
		}
				
	}
		
}

void checkMushroomBulletCollisions(float bullet[], sf::Clock& mushroomClock, float mushroom[3][42], int mushroomCount, int *score)
{
	 // this is to prevent a single bullet from hitting the mushroom twice 
	if (mushroomClock.getElapsedTime().asMilliseconds() < 10)
		return;
		mushroomClock.restart();
	
	for(int i = 0; i < mushroomCount; i++)
	{
		if((absoluteValue(bullet[x], mushroom[x][i]) < 32.0f) && (absoluteValue(bullet[y], mushroom[y][i]) < 32.0f)&& mushroom[exists][i] !=0)
		{
			mushroom[exists][i] -= 1;
			bullet[exists] = false;
			*score+= 1;
		}
	}		
}

void initialiseCentipede(float centipede[3][12], sf::RenderWindow& window, sf::Sprite& centipedeHeadSprite, bool direction [12])
{
	// it starts from a random row between 5 and 15
	int start = (rand() % 20) + 4; //range 0 to 10 plus 5 rows

	centipede[x][0] = resolutionX + 1;
	centipede[y][0] = start*boxPixelsY;
	centipedeHeadSprite.setPosition(centipede[x][0], centipede[y][0]);
	window.draw(centipedeHeadSprite);
	
	//centipede exists and initial direction is left
	for(int i = 0; i<12; i++)
	{
		centipede[exists][i] = true;
		direction[i] = true;
	}
	
	int segments = 12;
	//initialisation of values
	for(int i = 1; i< segments; i++)
	{
		centipede[x][i] = (centipede[x][i-1] + 32);
		centipede[y][i] = centipede[y][i-1];
		
	}
	
}


void moveCentipede(float centipede[3][12], sf::RenderWindow& window, sf::Clock centipedeClock,  bool direction[12], float mushroom[3][42], int mushroomCount, float player[], bool directionY[12])
{
	if (centipedeClock.getElapsedTime().asMilliseconds() > 100)
	{
		centipedeClock.restart();
		//modify to only move head
		
		int segments = 12;
		for(int i = 0; i < segments; i++)
		{ 
			if(centipede[y][i] < (gameRows - 5)*boxPixelsY)
			{
				if(centipede[exists][i])
				{	
					if(direction[i])
					{
						centipede[x][i] -= 0.5;
					
					}
		
					else
					{
						centipede[x][i]+=0.5;	
					}
		
					if(centipede[x][i] <= 0)
					{
						centipede[y][i] += 32;
						direction[i] = false;
					}
				
					else if (centipede[x][i] == resolutionX)
					{
						centipede[y][i] += 32;
						direction[i] = true;
					}
				}
				
		
				for(int j = 0; j < mushroomCount; j++)
				{
					if((absoluteValue(centipede[x][i], mushroom[x][j]) < 32.0f) && (absoluteValue(centipede[y][i], mushroom[y][j]) < 32.0f)&& mushroom[exists][j] !=0)
					{	
					centipede[y][i] += 32;
					direction[i] = !direction[i];
					}
				}		
	
			}
			
		}
	
	}


}	

void drawCentipede(float centipede[3][12], sf::RenderWindow& window, sf:: Sprite& centipedeHeadSprite, sf:: Sprite& centipedeSprite,  bool direction, bool head[])
{
	for(int i = 0; i< 12; i++)
	{
		if(centipede[exists][i])
		{
			if(head[i] == true)
			{
				centipedeHeadSprite.setPosition(centipede[x][i], centipede[y][i]);
				window.draw(centipedeHeadSprite);
			}		 
			else 
			{
				centipedeSprite.setPosition(centipede[x][i], centipede[y][i]);
				window.draw(centipedeSprite);
			}
		}
	}
}

void checkCentipedeBulletCollisions(float bullet[3], float centipede[3][12], bool head[], float mushroom[3][42], int &mushroomCount,  sf::Clock& mushroomClock, bool direction[], int* score)
{
	
	for(int i = 0; i< 12; i++)
	{
		//check collision
		if((absoluteValue(bullet[x], centipede[x][i]) < 10.0f) && (absoluteValue(bullet[y], centipede[y][i]) < 32.0f) && centipede[exists][i])
		{	//check head
			if(head[i] == true && bullet[exists])
			{
				bullet[exists] = false;
				
				//erasing entire segment upon collision with head
				int j = i;
				while(centipede[exists][j] && j <12)
				{
					centipede[exists][j] = false;
					j++;
				}
				*score +=20;
			}
			
			else 
			{
				bullet[exists] = false;
				*score+=10;
				centipede[exists][i] = false;
			}
			
			//mushroom spawn
			mushroom[x][mushroomCount] = centipede[x][i];
			mushroom[y][mushroomCount] = centipede[y][i];
			mushroom[exists][mushroomCount] = 2;
			mushroomCount++;
			
			//creating head
			if(centipede[exists][i+1])
			{
				head[i+1] = true;
			}
			
		}
	}
}

void checkCentipedePosition(float centipede[3][12], float player[], float mushroom[3][42], bool direction[], bool directionY[12], int mushroomCount)
{
	for(int i = 0; i < 12; i++)
	{
		//check player area
		if(centipede[y][i] >= (gameRows - 5)*boxPixelsY)
		{
			if(centipede[x][i] >= 0 && centipede[x][i] <= resolutionX )
			{
				//movement
				if(centipede[exists][i])
				{			
					if(direction[i])
					{
						centipede[x][i] -= 0.5;	
					}
					else
					{
						centipede[x][i]+=0.5;	
					}
					
					//borders
					if(centipede[x][i] == 0)
					{
						if(directionY[i] == true) //down
						{
							centipede[y][i] += 32;
						}
						else
						{
							centipede[y][i] -= 32;
						}
						direction[i] = false;
					}
			
					else if (centipede[x][i] == resolutionX)
					{
						if(directionY[i] == true) //down
						{
							centipede[y][i] += 32;
						}
						else
						{
							centipede[y][i] -= 32;
						}
						direction[i] = true;
					}
					
					//vertical movement
					if(centipede[y][i] == (gameRows - 5)*boxPixelsY)
					{
						directionY[i]= true; // down
						
					}
					if(centipede[y][i] == (gameRows-1)*boxPixelsY)
					{
						directionY[i] = false;
						
					}
					
					//check for mushrooms
					for(int j = 0; j < mushroomCount; j++)
					{
						if((absoluteValue(centipede[x][i], mushroom[x][j]) < 32.0f) && (absoluteValue(centipede[y][i], mushroom[y][j]) < 32.0f)&& mushroom[exists][j] !=0)
						{	
							centipede[y][i] += 32;
							direction[i] = !direction[i];
						}
					}	
				}		
			}
		}
	}
}

float absoluteValue(float value1, float value2)
{
	//returns absolute value of function
	float temp = value1 - value2;
	
	if(temp < 0)
	{
		temp = temp*(-1);
	}
	return temp;
}

bool checkPlayerCollision(float centipede[3][12], float player[], float mushroom[3][42], int mushroomCount)
{
	bool gameOver = false;

	//with centipede
	for(int i = 0; i<12; i++)
	{
		if(centipede[exists][i])
		{
			if(absoluteValue(player[x],  centipede[x][i]) < 32.0f && absoluteValue(player[y], centipede[y][i]) < 32.0f)
			{
				gameOver = true;
				return gameOver;	
			}
		}
	
	}
	
	//with mushroom
	for(int i = 0; i < mushroomCount; i++)
	{
		if(absoluteValue(player[x],  mushroom[x][i]) < 10.0f && absoluteValue(player[y], mushroom[y][i]) < 10.0f && mushroom[exists][i])
		{
			gameOver = true;
			return gameOver;	
		}
	}
	
return gameOver;
}

bool checkCentipedeExists(float centipede[3][12])
{
	for(int i = 0; i< 12; i++)
	{
		if(centipede[exists][i])
		{
			return true; // exists
		}
	}
}

