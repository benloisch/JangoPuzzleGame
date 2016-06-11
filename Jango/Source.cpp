#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace sf;

void Randomize();
bool LoadTxtFile();
bool SaveTxtFile();
void AssignArrayPuzzleEmptySpace();//Assigns empty spot to EmptySpace[i][j]
void SetArrayXYCoord();//Sets x and y of predifined button spaces to an arrayx and arrayy
void SetButtonNumber();//Assigns buttons 1 to 15 the number in spots ArrayPuzzle(1 - 15) excluding the blank spot
void SetButtonTextureRect();//Assigns a the corresponding rect to the number that each button holds
void TestButtonLeftRightUpDown();//find out where the button is and test whether it can move left right up or down
void TestButtonSelection();//find out which button the mouse clicked
void TestResetButtonSelection();
bool SetTextureSprites();
bool loadMusic();
bool loadFont();

int txtfileNumber = 4;
int trackNum = 0;
int trackNumCounter = 0;
int ArrayPuzzle[4][4] = {0};
int EmptySpace[4][4] = {0};

float xMotion = 0;
float yMotion = 0;

bool CloseWindow = false;
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
bool SelectButton = true;
bool ResetTime = false;
bool ResetTimeTwo = false;
bool buttonClickOne = false;
bool buttonClickTwo = false;
bool bpageTurn = false;

float ArrayValueX[4][4] = {0};
float ArrayValueY[4][4] = {0};

Music ButtonClick;
Music ResetButton;
Music SolvedPuzzle;
Music pageTurn;
Music Track1;
Music Track2;
Music Track3;
Music Track4;

bool Track1Play = false;
bool Track2Play = false;
bool Track3Play = false;
bool Track4Play = false;

Sprite ResetButtonPressed;
Sprite ResetButtonUnPressed;
Sprite ScreenOneSprite;
Sprite ScreenTwoSprite;
Texture ResetButtonTexturePressed;
Texture ResetButtonTextureUnPressed;
Texture ScreenOneTexture;
Texture ScreenTwoTexture;

Font font;

int TimeSeconds = 0;
int BestTimeSeconds = 0;

class Button
{
private:
	Texture texture;
	Sprite sprite;
	int Number;
	float xCoord;
	float yCoord;
	float xCoordStart;
	float yCoordStart;
	bool Right;
	bool Left;
	bool Up;
	bool Down;
public:
	Button();
	Sprite DisplayButton();
	void SetTextureRect(int x, int y, int w, int h);
	void SetCoord(float X, float Y);
	void SetNumber(int n);
	int GetNumber();
	float GetCoordX();
	float GetCoordY();
	void SetCoordxStart(float x);
	void SetCoordyStart(float y);
	void SetLRUD(bool left, bool right, bool up, bool down);
	void RunLRUD();
};

Button::Button()
{
	if(!texture.loadFromFile("Images/Dark-brown-fine-wood-textureEdit.png"))
		CloseWindow = true;
	texture.setSmooth(true);
	sprite.setTexture(texture);
	Number = 0;
	xCoord = 0;
	yCoord = 0;//these coordinates will change the actuall position
	xCoordStart = 0;
	yCoordStart = 0;
	Right = false;
	Left = false;
	Up = false;
	Down = false;
}

Sprite Button::DisplayButton()
{
	return sprite;
}

void Button::SetTextureRect(int x, int y, int w, int h)
{
	sprite.setTextureRect(IntRect(x, y, w, h));
}

void Button::SetCoord(float X, float Y)
{
	xCoord = X;
	yCoord = Y;

	sprite.setPosition(xCoord, yCoord);
}

void Button::SetNumber(int n)
{
	Number = n;
}

int Button::GetNumber()
{
	return Number;
}

float Button::GetCoordX()
{
	return xCoord;
}

float Button::GetCoordY()
{
	return yCoord;
}

void Button::SetCoordxStart(float x)
{
	xCoordStart = x;
}

void Button::SetCoordyStart(float y)
{
	yCoordStart = y;
}

void Button::SetLRUD(bool left, bool right, bool up, bool down)
{
	Left = left;
	Right = right;
	Up = up;
	Down = down;
}

void Button::RunLRUD()
{
	if(Left)
	{
		if(xCoord > (xCoordStart - 100))
			xCoord -= 10;

		sprite.setPosition(xCoord, yCoord);

		if(xCoord == (xCoordStart - 100))
		{
			Left = false;
			SelectButton = true;
		}
	}

	if(Right)
	{
		if(xCoord < (xCoordStart + 100))
			xCoord += 10;

		sprite.setPosition(xCoord, yCoord);

		if(xCoord == (xCoordStart + 100))
		{
			Right = false;
			SelectButton = true;
		}
	}

	if(Down)
	{
		if(yCoord < (yCoordStart + 100))
			yCoord += 10;

		sprite.setPosition(xCoord, yCoord);

		if(yCoord == (yCoordStart + 100))
		{
			Down = false;
			SelectButton = true;
		}
	}

	if(Up)
	{
		if(yCoord > (yCoordStart - 100))
			yCoord -= 10;

		sprite.setPosition(xCoord, yCoord);

		if(yCoord == (yCoordStart - 100))
		{
			Up = false;
			SelectButton = true;
		}
	}
}

void SetArrayXYCoord();
void SetButtonNumber(Button& Button1, Button& Button2, Button& Button3, Button& Button4,
					 Button& Button5, Button& Button6, Button& Button7, Button& Button8, 
					 Button& Button9, Button& Button10, Button& Button11, Button& Button12,
					 Button& Button13, Button& Button14, Button& Button15);
void SetButtonTextureRect(Button& Button1, Button& Button2, Button& Button3, Button& Button4,
					 Button& Button5, Button& Button6, Button& Button7, Button& Button8, 
					 Button& Button9, Button& Button10, Button& Button11, Button& Button12,
					 Button& Button13, Button& Button14, Button& Button15);

void TestButtonSelection(Button& Button1, Button& Button2, Button& Button3, Button& Button4,
					 Button& Button5, Button& Button6, Button& Button7, Button& Button8, 
					 Button& Button9, Button& Button10, Button& Button11, Button& Button12,
					 Button& Button13, Button& Button14, Button& Button15, Vector2i mouseCoord);

void TestResetButtonSelection(Button& Button1, Button& Button2, Button& Button3, Button& Button4,
					 Button& Button5, Button& Button6, Button& Button7, Button& Button8, 
					 Button& Button9, Button& Button10, Button& Button11, Button& Button12,
					 Button& Button13, Button& Button14, Button& Button15, Vector2i MouseCoord);

int main()
{
	if(!LoadTxtFile())
		return 1;

	if(!SetTextureSprites())
		return 1;

	if(!loadMusic())
		return 1;

	if(!loadFont())
		return 1;

	Randomize();

	bool bResetButtonPressed = false;
	bool bResetButtonOK = false;

	Vector2i MouseCoord;

	Clock clock;
	Time TimeSecondsTime;
	
	TimeSeconds = TimeSecondsTime.asSeconds();
	
	Text textYourTime;
	Text textBestTime;
	textYourTime.setFont(font);
	textBestTime.setFont(font);

	string sYourTimeSec;
	string sBestTimeSec;

	ostringstream convert;

	convert << TimeSeconds;
	
	sYourTimeSec = convert.str();

	convert.str("");

	convert << BestTimeSeconds;

	sBestTimeSec = convert.str();

	textYourTime.setString("Your Time:" + sYourTimeSec);
	textYourTime.setCharacterSize(24);
	textYourTime.setColor(Color(0, 0, 70));
	textYourTime.setPosition(200, 10);

	textBestTime.setString("Best Time:" + sBestTimeSec);
	textBestTime.setCharacterSize(24);
	textBestTime.setColor(Color::White);
	textBestTime.setPosition(400, 10);

	//Set an extra outlines board underneath the buttons for a grid board -Next five lines
	Texture backgroundBoardTexture;
	backgroundBoardTexture.loadFromFile("Images/wood-texture-seamless-mtgcy225.png");
	Sprite backgroundBoard;
	backgroundBoard.setTexture(backgroundBoardTexture);
	backgroundBoard.setPosition(0, 0);

	ResetButtonPressed.setPosition(300, 500);
	ResetButtonUnPressed.setPosition(300, 500);

	Event event;
	RenderWindow window(VideoMode(800, 600), "Jango");
	window.setFramerateLimit(60);
	
	Button Button1;
	Button Button2;
	Button Button3;
	Button Button4;
	Button Button5;
	Button Button6;
	Button Button7;
	Button Button8;
	Button Button9;
	Button Button10;
	Button Button11;
	Button Button12;
	Button Button13;
	Button Button14;
	Button Button15;

	SetArrayXYCoord();

	SetButtonNumber(Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8,
		Button9, Button10, Button11, Button12, Button13, Button14, Button15);

	SetButtonTextureRect(Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8,
		Button9, Button10, Button11, Button12, Button13, Button14, Button15);

	if(trackNum == 1)
	{
		Track2Play = true;
		Track3Play = true;
		Track4Play = true;
		Track1.play();
	}

	if(trackNum == 2)
	{
		Track1Play = true;
		Track3Play = true;
		Track4Play = true;
		Track2.play();
	}

	if(trackNum == 3)
	{
		Track1Play = true;
		Track2Play = true;
		Track4Play = true;
		Track3.play();
	}

	if(trackNum == 4)
	{
		Track1Play = true;
		Track2Play = true;
		Track3Play = true;
		Track4.play();
	}

    while(window.isOpen())
    {
		window.clear(sf::Color(200, 50, 50, 255));

		if(CloseWindow)
			window.close();

		MouseCoord = Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
			if (event.type == Event::Closed)
                window.close();

			if(event.key.code == Keyboard::Escape)
				window.close();
			
			if(bpageTurn == false)
			if(buttonClickOne)
				if(event.type == Event::MouseButtonPressed)
					if(event.mouseButton.button == Mouse::Left)
					{
						buttonClickTwo = true;
						pageTurn.play();
					}

			if(bpageTurn == false)
			if(buttonClickOne == false)
				if(event.type == Event::MouseButtonPressed)
					if(event.mouseButton.button == Mouse::Left)
					{
						buttonClickOne = true;
						pageTurn.play();
					}

			if(buttonClickTwo)
			{
				TimeSecondsTime = clock.getElapsedTime();
				TimeSeconds = TimeSecondsTime.asSeconds();

				AssignArrayPuzzleEmptySpace();
				bpageTurn = true;
			
				if(event.type == Event::MouseButtonPressed)
				{
					if(event.mouseButton.button == Mouse::Left)
					{
						if(MouseCoord.x > 300 && MouseCoord.x < 500 && MouseCoord.y > 500 && MouseCoord.y < 550)
						{
							bResetButtonPressed = true;
							bResetButtonOK = true;
						}
						else
						{
							bResetButtonPressed = false;
							bResetButtonOK = false;
						}
					
						if(SelectButton)
							TestButtonSelection(Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8,
							Button9, Button10, Button11, Button12, Button13, Button14, Button15, Mouse::getPosition(window));	
					}
				}

				if(event.type == Event::MouseButtonReleased)
					if(event.mouseButton.button == Mouse::Left)
						if(MouseCoord.x > 300 && MouseCoord.x < 500 && MouseCoord.y > 500 && MouseCoord.y < 550)
						{	
							bResetButtonPressed = false;
							if(SelectButton)
							if(bResetButtonOK)
							{
								TestResetButtonSelection(Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8,
								Button9, Button10, Button11, Button12, Button13, Button14, Button15, Mouse::getPosition(window));
								bResetButtonOK = false;
								ResetButton.play();
								clock.restart();
								ResetTimeTwo = true;
							}
						}
						else
							bResetButtonPressed = false;

				if(ArrayPuzzle[0][0] == 1 && ArrayPuzzle[0][1] == 2 && ArrayPuzzle[0][2] == 3 && ArrayPuzzle[0][3] == 4
					&& ArrayPuzzle[1][0] == 5 && ArrayPuzzle[1][1] == 6 && ArrayPuzzle[1][2] == 7 && ArrayPuzzle[1][3] == 8
					&& ArrayPuzzle[2][0] == 9 && ArrayPuzzle[2][1] == 10 && ArrayPuzzle[2][2] == 11 && ArrayPuzzle[2][3] == 12
					&& ArrayPuzzle[3][0] == 13 && ArrayPuzzle[3][1] == 14 && ArrayPuzzle[3][2] == 15)
				{
					if(TimeSeconds < BestTimeSeconds)
						BestTimeSeconds = TimeSeconds;
					if(ResetTime == false)
					{
						convert.str("");

						convert << TimeSeconds;
	
						sYourTimeSec = convert.str();

						convert.str("");

						convert << BestTimeSeconds;
	
						sBestTimeSec = convert.str();
						ResetTime = true;
						ResetTimeTwo = false;

						SolvedPuzzle.play();
					}

					if(ResetTimeTwo)
					{
						if(ArrayPuzzle[0][0] == 1 && ArrayPuzzle[0][1] == 2 && ArrayPuzzle[0][2] == 3 && ArrayPuzzle[0][3] == 4
							&& ArrayPuzzle[1][0] == 5 && ArrayPuzzle[1][1] == 6 && ArrayPuzzle[1][2] == 7 && ArrayPuzzle[1][3] == 8
							&& ArrayPuzzle[2][0] == 9 && ArrayPuzzle[2][1] == 10 && ArrayPuzzle[2][2] == 11 && ArrayPuzzle[2][3] == 12
							&& ArrayPuzzle[3][0] == 13 && ArrayPuzzle[3][1] == 14 && ArrayPuzzle[3][2] == 15)
						{
							ResetTime = false;
						}
					}
				}
			}
        }

		if(buttonClickTwo)
		{
			Button1.RunLRUD();
			Button2.RunLRUD();
			Button3.RunLRUD();
			Button4.RunLRUD();
			Button5.RunLRUD();
			Button6.RunLRUD();
			Button7.RunLRUD();
			Button8.RunLRUD();
			Button9.RunLRUD();
			Button10.RunLRUD();
			Button11.RunLRUD();
			Button12.RunLRUD();
			Button13.RunLRUD();
			Button14.RunLRUD();
			Button15.RunLRUD();

			window.draw(backgroundBoard);
			window.draw(Button1.DisplayButton());
			window.draw(Button2.DisplayButton());
			window.draw(Button3.DisplayButton());
			window.draw(Button4.DisplayButton());
			window.draw(Button5.DisplayButton());
			window.draw(Button6.DisplayButton());
			window.draw(Button7.DisplayButton());
			window.draw(Button8.DisplayButton());
			window.draw(Button9.DisplayButton());
			window.draw(Button10.DisplayButton());
			window.draw(Button11.DisplayButton());
			window.draw(Button12.DisplayButton());
			window.draw(Button13.DisplayButton());
			window.draw(Button14.DisplayButton());
			window.draw(Button15.DisplayButton());
			textYourTime.setString("Your Time: " + sYourTimeSec);
			textBestTime.setString("Best Time: " + sBestTimeSec);
			window.draw(textYourTime);
			window.draw(textBestTime);

			if(bResetButtonPressed)
				window.draw(ResetButtonPressed);
			else
				window.draw(ResetButtonUnPressed);

			window.display();
		}

		if(buttonClickOne == false)
		{
			window.draw(ScreenOneSprite);
			window.display();
		}

		if(buttonClickTwo == false && buttonClickOne == true)
		{
			window.draw(ScreenTwoSprite);
			window.display();
		}

		if(Track1Play == false)
		{
			if(Track1.getStatus() == Music::Stopped)
			{
				Track2.play();
				Track1Play = true;
				Track2Play = false;
				Track3Play = true;
				Track4Play = true;
				trackNum++;
			}
		}

		if(trackNum > 4)
			trackNum = 1;

		if(Track2Play == false)
		{
			if(Track2.getStatus() == Music::Stopped)
			{
				Track3.play();
				Track1Play = true;
				Track2Play = true;
				Track3Play = false;
				Track4Play = true;
				trackNum++;
			}
		}

		if(trackNum > 4)
			trackNum = 1;

		if(Track3Play == false)
		{
			if(Track3.getStatus() == Music::Stopped)
			{
				Track4.play();
				Track1Play = true;
				Track2Play = true;
				Track3Play = true;
				Track4Play = false;
				trackNum++;
			}
		}

		if(trackNum > 4)
			trackNum = 1;

		if(Track4Play == false)
		{
			if(Track4.getStatus() == Music::Stopped)
			{
				Track1.play();
				Track1Play = false;
				Track2Play = true;
				Track3Play = true;
				Track4Play = true;
				trackNum++;
			}
		}

		if(trackNum > 4)
			trackNum = 1;
		/*
		if(ArrayPuzzle[0][0] == 1 && ArrayPuzzle[0][1] == 2 && ArrayPuzzle[0][2] == 3 && ArrayPuzzle[0][3] == 4
			&& ArrayPuzzle[1][0] == 5 && ArrayPuzzle[1][1] == 6 && ArrayPuzzle[1][2] == 7 && ArrayPuzzle[1][3] == 8
			&& ArrayPuzzle[2][0] == 9 && ArrayPuzzle[2][1] == 10 && ArrayPuzzle[2][2] == 11 && ArrayPuzzle[2][3] == 12
			&& ArrayPuzzle[3][0] == 13 && ArrayPuzzle[3][1] == 14 && ArrayPuzzle[3][2] == 15)
		{
			window.close();
		}
		*/
    }

	if(!SaveTxtFile())
		return 1;

    return 0;
}

void Randomize()
{
	int pickedRandomSpotForAP00 = 0;
	int pickedRandomSpotForRandomSpot = 0;
	int cycleCounter = 0;
	int completelyCycledCount = 0;
	bool one = true, two = true, three = true, four = true, five = true, six = true, seven = true;
	bool eight = true, nine = true, ten = true, eleven = true, twelve = true, thirteen = true, fourteen = true, fifteen = true, sixteen = true;
	bool completelyCycled = false;
	while(completelyCycled == false)
	{
		if(one)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				one = false;
				txtfileNumber += 2;//This mixes up the patter and makes every single number other than ArrayPuzzle[0][0] random. 
				//ArrayPuzzle [0][1] has a hardly noticable pattern though.
				//ArrayPuzzle [0][0] has a distinct pattert going as such; 1, 2, 3, 4, 5 ect. 
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 1;
					break;
				case 2:
					ArrayPuzzle[0][1] = 1;
					break;
				case 3:
					ArrayPuzzle[0][2] = 1;
					break;
				case 4:
					ArrayPuzzle[0][3] = 1;
					break;
				case 5:
					ArrayPuzzle[1][0] = 1;
					break;
				case 6:
					ArrayPuzzle[1][1] = 1;
					break;
				case 7:
					ArrayPuzzle[1][2] = 1;
					break;
				case 8:
					ArrayPuzzle[1][3] = 1;
					break;
				case 9:
					ArrayPuzzle[2][0] = 1;
					break;
				case 10:
					ArrayPuzzle[2][1] = 1;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 1;
					break;
				case 12:
					ArrayPuzzle[2][3] = 1;
					break;
				case 13:
					ArrayPuzzle[3][0] = 1;
					break;
				case 14:
					ArrayPuzzle[3][1] = 1;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 1;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(two)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				two = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 2;
					break;
				case 2:
					ArrayPuzzle[0][1] = 2;
					break;
				case 3:
					ArrayPuzzle[0][2] = 2;
					break;
				case 4:
					ArrayPuzzle[0][3] = 2;
					break;
				case 5:
					ArrayPuzzle[1][0] = 2;
					break;
				case 6:
					ArrayPuzzle[1][1] = 2;
					break;
				case 7:
					ArrayPuzzle[1][2] = 2;
					break;
				case 8:
					ArrayPuzzle[1][3] = 2;
					break;
				case 9:
					ArrayPuzzle[2][0] = 2;
					break;
				case 10:
					ArrayPuzzle[2][1] = 2;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 2;
					break;
				case 12:
					ArrayPuzzle[2][3] = 2;
					break;
				case 13:
					ArrayPuzzle[3][0] = 2;
					break;
				case 14:
					ArrayPuzzle[3][1] = 2;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 2;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(three)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				three = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 3;
					break;
				case 2:
					ArrayPuzzle[0][1] = 3;
					break;
				case 3:
					ArrayPuzzle[0][2] = 3;
					break;
				case 4:
					ArrayPuzzle[0][3] = 3;
					break;
				case 5:
					ArrayPuzzle[1][0] = 3;
					break;
				case 6:
					ArrayPuzzle[1][1] = 3;
					break;
				case 7:
					ArrayPuzzle[1][2] = 3;
					break;
				case 8:
					ArrayPuzzle[1][3] = 3;
					break;
				case 9:
					ArrayPuzzle[2][0] = 3;
					break;
				case 10:
					ArrayPuzzle[2][1] = 3;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 3;
					break;
				case 12:
					ArrayPuzzle[2][3] = 3;
					break;
				case 13:
					ArrayPuzzle[3][0] = 3;
					break;
				case 14:
					ArrayPuzzle[3][1] = 3;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 3;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(four)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				four = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 4;
					break;
				case 2:
					ArrayPuzzle[0][1] = 4;
					break;
				case 3:
					ArrayPuzzle[0][2] = 4;
					break;
				case 4:
					ArrayPuzzle[0][3] = 4;
					break;
				case 5:
					ArrayPuzzle[1][0] = 4;
					break;
				case 6:
					ArrayPuzzle[1][1] = 4;
					break;
				case 7:
					ArrayPuzzle[1][2] = 4;
					break;
				case 8:
					ArrayPuzzle[1][3] = 4;
					break;
				case 9:
					ArrayPuzzle[2][0] = 4;
					break;
				case 10:
					ArrayPuzzle[2][1] = 4;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 4;
					break;
				case 12:
					ArrayPuzzle[2][3] = 4;
					break;
				case 13:
					ArrayPuzzle[3][0] = 4;
					break;
				case 14:
					ArrayPuzzle[3][1] = 4;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 4;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(five)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				five = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 5;
					break;
				case 2:
					ArrayPuzzle[0][1] = 5;
					break;
				case 3:
					ArrayPuzzle[0][2] = 5;
					break;
				case 4:
					ArrayPuzzle[0][3] = 5;
					break;
				case 5:
					ArrayPuzzle[1][0] = 5;
					break;
				case 6:
					ArrayPuzzle[1][1] = 5;
					break;
				case 7:
					ArrayPuzzle[1][2] = 5;
					break;
				case 8:
					ArrayPuzzle[1][3] = 5;
					break;
				case 9:
					ArrayPuzzle[2][0] = 5;
					break;
				case 10:
					ArrayPuzzle[2][1] = 5;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 5;
					break;
				case 12:
					ArrayPuzzle[2][3] = 5;
					break;
				case 13:
					ArrayPuzzle[3][0] = 5;
					break;
				case 14:
					ArrayPuzzle[3][1] = 5;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 5;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(six)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				six = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 6;
					break;
				case 2:
					ArrayPuzzle[0][1] = 6;
					break;
				case 3:
					ArrayPuzzle[0][2] = 6;
					break;
				case 4:
					ArrayPuzzle[0][3] = 6;
					break;
				case 5:
					ArrayPuzzle[1][0] = 6;
					break;
				case 6:
					ArrayPuzzle[1][1] = 6;
					break;
				case 7:
					ArrayPuzzle[1][2] = 6;
					break;
				case 8:
					ArrayPuzzle[1][3] = 6;
					break;
				case 9:
					ArrayPuzzle[2][0] = 6;
					break;
				case 10:
					ArrayPuzzle[2][1] = 6;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 6;
					break;
				case 12:
					ArrayPuzzle[2][3] = 6;
					break;
				case 13:
					ArrayPuzzle[3][0] = 6;
					break;
				case 14:
					ArrayPuzzle[3][1] = 6;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 6;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(seven)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				seven = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 7;
					break;
				case 2:
					ArrayPuzzle[0][1] = 7;
					break;
				case 3:
					ArrayPuzzle[0][2] = 7;
					break;
				case 4:
					ArrayPuzzle[0][3] = 7;
					break;
				case 5:
					ArrayPuzzle[1][0] = 7;
					break;
				case 6:
					ArrayPuzzle[1][1] = 7;
					break;
				case 7:
					ArrayPuzzle[1][2] = 7;
					break;
				case 8:
					ArrayPuzzle[1][3] = 7;
					break;
				case 9:
					ArrayPuzzle[2][0] = 7;
					break;
				case 10:
					ArrayPuzzle[2][1] = 7;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 7;
					break;
				case 12:
					ArrayPuzzle[2][3] = 7;
					break;
				case 13:
					ArrayPuzzle[3][0] = 7;
					break;
				case 14:
					ArrayPuzzle[3][1] = 7;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 7;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(eight)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				eight = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 8;
					break;
				case 2:
					ArrayPuzzle[0][1] = 8;
					break;
				case 3:
					ArrayPuzzle[0][2] = 8;
					break;
				case 4:
					ArrayPuzzle[0][3] = 8;
					break;
				case 5:
					ArrayPuzzle[1][0] = 8;
					break;
				case 6:
					ArrayPuzzle[1][1] = 8;
					break;
				case 7:
					ArrayPuzzle[1][2] = 8;
					break;
				case 8:
					ArrayPuzzle[1][3] = 8;
					break;
				case 9:
					ArrayPuzzle[2][0] = 8;
					break;
				case 10:
					ArrayPuzzle[2][1] = 8;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 8;
					break;
				case 12:
					ArrayPuzzle[2][3] = 8;
					break;
				case 13:
					ArrayPuzzle[3][0] = 8;
					break;
				case 14:
					ArrayPuzzle[3][1] = 8;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 8;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(nine)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				nine = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 9;
					break;
				case 2:
					ArrayPuzzle[0][1] = 9;
					break;
				case 3:
					ArrayPuzzle[0][2] = 9;
					break;
				case 4:
					ArrayPuzzle[0][3] = 9;
					break;
				case 5:
					ArrayPuzzle[1][0] = 9;
					break;
				case 6:
					ArrayPuzzle[1][1] = 9;
					break;
				case 7:
					ArrayPuzzle[1][2] = 9;
					break;
				case 8:
					ArrayPuzzle[1][3] = 9;
					break;
				case 9:
					ArrayPuzzle[2][0] = 9;
					break;
				case 10:
					ArrayPuzzle[2][1] = 9;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 9;
					break;
				case 12:
					ArrayPuzzle[2][3] = 9;
					break;
				case 13:
					ArrayPuzzle[3][0] = 9;
					break;
				case 14:
					ArrayPuzzle[3][1] = 9;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 9;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}
		
		if(ten)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				ten = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 10;
					break;
				case 2:
					ArrayPuzzle[0][1] = 10;
					break;
				case 3:
					ArrayPuzzle[0][2] = 10;
					break;
				case 4:
					ArrayPuzzle[0][3] = 10;
					break;
				case 5:
					ArrayPuzzle[1][0] = 10;
					break;
				case 6:
					ArrayPuzzle[1][1] = 10;
					break;
				case 7:
					ArrayPuzzle[1][2] = 10;
					break;
				case 8:
					ArrayPuzzle[1][3] = 10;
					break;
				case 9:
					ArrayPuzzle[2][0] = 10;
					break;
				case 10:
					ArrayPuzzle[2][1] = 10;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 10;
					break;
				case 12:
					ArrayPuzzle[2][3] = 10;
					break;
				case 13:
					ArrayPuzzle[3][0] = 10;
					break;
				case 14:
					ArrayPuzzle[3][1] = 10;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 10;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(eleven)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				eleven = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 11;
					break;
				case 2:
					ArrayPuzzle[0][1] = 11;
					break;
				case 3:
					ArrayPuzzle[0][2] = 11;
					break;
				case 4:
					ArrayPuzzle[0][3] = 11;
					break;
				case 5:
					ArrayPuzzle[1][0] = 11;
					break;
				case 6:
					ArrayPuzzle[1][1] = 11;
					break;
				case 7:
					ArrayPuzzle[1][2] = 11;
					break;
				case 8:
					ArrayPuzzle[1][3] = 11;
					break;
				case 9:
					ArrayPuzzle[2][0] = 11;
					break;
				case 10:
					ArrayPuzzle[2][1] = 11;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 11;
					break;
				case 12:
					ArrayPuzzle[2][3] = 11;
					break;
				case 13:
					ArrayPuzzle[3][0] = 11;
					break;
				case 14:
					ArrayPuzzle[3][1] = 11;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 11;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(twelve)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				twelve = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 12;
					break;
				case 2:
					ArrayPuzzle[0][1] = 12;
					break;
				case 3:
					ArrayPuzzle[0][2] = 12;
					break;
				case 4:
					ArrayPuzzle[0][3] = 12;
					break;
				case 5:
					ArrayPuzzle[1][0] = 12;
					break;
				case 6:
					ArrayPuzzle[1][1] = 12;
					break;
				case 7:
					ArrayPuzzle[1][2] = 12;
					break;
				case 8:
					ArrayPuzzle[1][3] = 12;
					break;
				case 9:
					ArrayPuzzle[2][0] = 12;
					break;
				case 10:
					ArrayPuzzle[2][1] = 12;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 12;
					break;
				case 12:
					ArrayPuzzle[2][3] = 12;
					break;
				case 13:
					ArrayPuzzle[3][0] = 12;
					break;
				case 14:
					ArrayPuzzle[3][1] = 12;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 12;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(thirteen)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				thirteen = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 13;
					break;
				case 2:
					ArrayPuzzle[0][1] = 13;
					break;
				case 3:
					ArrayPuzzle[0][2] = 13;
					break;
				case 4:
					ArrayPuzzle[0][3] = 13;
					break;
				case 5:
					ArrayPuzzle[1][0] = 13;
					break;
				case 6:
					ArrayPuzzle[1][1] = 13;
					break;
				case 7:
					ArrayPuzzle[1][2] = 13;
					break;
				case 8:
					ArrayPuzzle[1][3] = 13;
					break;
				case 9:
					ArrayPuzzle[2][0] = 13;
					break;
				case 10:
					ArrayPuzzle[2][1] = 13;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 13;
					break;
				case 12:
					ArrayPuzzle[2][3] = 13;
					break;
				case 13:
					ArrayPuzzle[3][0] = 13;
					break;
				case 14:
					ArrayPuzzle[3][1] = 13;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 13;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(fourteen)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				fourteen = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 14;
					break;
				case 2:
					ArrayPuzzle[0][1] = 14;
					break;
				case 3:
					ArrayPuzzle[0][2] = 14;
					break;
				case 4:
					ArrayPuzzle[0][3] = 14;
					break;
				case 5:
					ArrayPuzzle[1][0] = 14;
					break;
				case 6:
					ArrayPuzzle[1][1] = 14;
					break;
				case 7:
					ArrayPuzzle[1][2] = 14;
					break;
				case 8:
					ArrayPuzzle[1][3] = 14;
					break;
				case 9:
					ArrayPuzzle[2][0] = 14;
					break;
				case 10:
					ArrayPuzzle[2][1] = 14;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 14;
					break;
				case 12:
					ArrayPuzzle[2][3] = 14;
					break;
				case 13:
					ArrayPuzzle[3][0] = 14;
					break;
				case 14:
					ArrayPuzzle[3][1] = 14;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 14;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}

		if(fifteen)
		{
			cycleCounter++;
			if(cycleCounter == txtfileNumber)
			{
				cycleCounter = 0;
				completelyCycledCount++;
				fifteen = false;
				txtfileNumber++;
				switch(completelyCycledCount)
				{
				case 1:
					ArrayPuzzle[0][0] = 15;
					break;
				case 2:
					ArrayPuzzle[0][1] = 15;
					break;
				case 3:
					ArrayPuzzle[0][2] = 15;
					break;
				case 4:
					ArrayPuzzle[0][3] = 15;
					break;
				case 5:
					ArrayPuzzle[1][0] = 15;
					break;
				case 6:
					ArrayPuzzle[1][1] = 15;
					break;
				case 7:
					ArrayPuzzle[1][2] = 15;
					break;
				case 8:
					ArrayPuzzle[1][3] = 15;
					break;
				case 9:
					ArrayPuzzle[2][0] = 15;
					break;
				case 10:
					ArrayPuzzle[2][1] = 15;
					break;
				case 11: 
					ArrayPuzzle[2][2] = 15;
					break;
				case 12:
					ArrayPuzzle[2][3] = 15;
					break;
				case 13:
					ArrayPuzzle[3][0] = 15;
					break;
				case 14:
					ArrayPuzzle[3][1] = 15;
					break;
				case 15:
					{
						ArrayPuzzle[3][2] = 15;
						completelyCycled = true;
						break;
					}
				default:
					completelyCycled = true;
				}
			}
		}
	}
	//Asign the random number in ArrayPuzzle[1][2] to the random slot of 1 to 15 where the blank space will go
	pickedRandomSpotForAP00 = ArrayPuzzle[1][2];
	pickedRandomSpotForRandomSpot = ArrayPuzzle[1][1];

	switch(pickedRandomSpotForAP00)
	{
		case 1:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 2:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[0][1];
			ArrayPuzzle[0][1] = 0;
			ArrayPuzzle[0][1] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 3:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[0][2];
			ArrayPuzzle[0][2] = 0;
			ArrayPuzzle[0][2] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 4:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[0][3];
			ArrayPuzzle[0][3] = 0;
			ArrayPuzzle[0][3] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 5:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[1][0];
			ArrayPuzzle[1][0] = 0;
			ArrayPuzzle[1][0] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 6:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[1][1];
			ArrayPuzzle[1][1] = 0;
			ArrayPuzzle[1][1] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 7:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[1][2];
			ArrayPuzzle[1][2] = 0;
			ArrayPuzzle[1][2] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 8:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[1][3];
			ArrayPuzzle[1][3] = 0;
			ArrayPuzzle[1][3] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 9:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[2][0];
			ArrayPuzzle[2][0] = 0;
			ArrayPuzzle[2][0] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 10:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[2][1];
			ArrayPuzzle[2][1] = 0;
			ArrayPuzzle[2][1] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 11:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[2][2];
			ArrayPuzzle[2][2] = 0;
			ArrayPuzzle[2][2] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 12:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[2][3];
			ArrayPuzzle[2][3] = 0;
			ArrayPuzzle[2][3] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 13:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[3][0];
			ArrayPuzzle[3][0] = 0;
			ArrayPuzzle[3][0] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 14:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[3][1];
			ArrayPuzzle[3][1] = 0;
			ArrayPuzzle[3][1] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		case 15:
		{
			ArrayPuzzle[3][3] = ArrayPuzzle[3][2];
			ArrayPuzzle[3][2] = 0;
			ArrayPuzzle[3][2] = ArrayPuzzle[0][0];
			ArrayPuzzle[0][0] = 0;
			break;
		}
		default:
			ArrayPuzzle[3][3] = 0;
	}

	switch(pickedRandomSpotForRandomSpot)
	{
	case 1:
		{
			ArrayPuzzle[0][0] = 0;
			break;
		}
	case 2:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[0][1];
			ArrayPuzzle[0][1] = 0;
			break;
		}
	case 3:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[0][2];
			ArrayPuzzle[0][2] = 0;
			break;
		}
	case 4:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[0][3];
			ArrayPuzzle[0][3] = 0;
			break;
		}
	case 5:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[1][0];
			ArrayPuzzle[1][0] = 0;
			break;
		}
	case 6:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[1][1];
			ArrayPuzzle[1][1] = 0;
			break;
		}
	case 7:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[1][2];
			ArrayPuzzle[1][2] = 0;
			break;
		}
	case 8 :
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[1][3];
			ArrayPuzzle[1][3] = 0;
			break;
		}
	case 9:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[2][0];
			ArrayPuzzle[2][0] = 0;
			break;
		}
	case 10:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[2][1];
			ArrayPuzzle[2][0] = 0;
			break;
		}
	case 11:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[2][2];
			ArrayPuzzle[2][2] = 0;
			break;
		}
	case 12:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[2][3];
			ArrayPuzzle[2][3] = 0;
			break;
		}
	case 13:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[3][0];
			ArrayPuzzle[3][0] = 0;
			break;
		}
	case 14:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[3][1];
			ArrayPuzzle[3][1] = 0;
			break;
		}
	case 15:
		{
			ArrayPuzzle[0][0] = ArrayPuzzle[3][2];
			ArrayPuzzle[3][2] = 0;
			break;
		}
	default:
		ArrayPuzzle[0][0] = 0;
	}
}

bool LoadTxtFile()
{
	ifstream OptionsFile;
	OptionsFile.open("Options/SeedGenerator.txt", ios::in);

	if(OptionsFile.is_open())
	{
		OptionsFile >> txtfileNumber;
		OptionsFile >> BestTimeSeconds;
		OptionsFile >> trackNum;
		OptionsFile.close();
	}
	else
		return false;

	return true;
}

bool SaveTxtFile()
{
	ofstream OptionsFile;
	OptionsFile.open("Options/SeedGenerator.txt", ios::out);
	
	if(OptionsFile.is_open())
	{
		OptionsFile << txtfileNumber << endl;
		OptionsFile << BestTimeSeconds << endl;
		OptionsFile << trackNum;
		OptionsFile.close();
	}
	else
		return false;

	return true;
}

void AssignArrayPuzzleEmptySpace()
{
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			EmptySpace[i][j] = 1;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			if(ArrayPuzzle[i][j] == 0)
			{
				EmptySpace[i][j] = 0;
			}
		}
}

void SetArrayXYCoord()
{
	ArrayValueX[0][0] = 200;
	ArrayValueY[0][0] = 50;

	ArrayValueX[0][1] = 300;
	ArrayValueY[0][1] = 50;

	ArrayValueX[0][2] = 400;
	ArrayValueY[0][2] = 50;

	ArrayValueX[0][3] = 500;
	ArrayValueY[0][3] = 50;

	ArrayValueX[1][0] = 200;
	ArrayValueY[1][0] = 150;

	ArrayValueX[1][1] = 300;
	ArrayValueY[1][1] = 150;

	ArrayValueX[1][2] = 400;
	ArrayValueY[1][2] = 150;

	ArrayValueX[1][3] = 500;
	ArrayValueY[1][3] = 150;

	ArrayValueX[2][0] = 200;
	ArrayValueY[2][0] = 250;

	ArrayValueX[2][1] = 300;
	ArrayValueY[2][1] = 250;

	ArrayValueX[2][2] = 400;
	ArrayValueY[2][2] = 250;

	ArrayValueX[2][3] = 500;
	ArrayValueY[2][3] = 250;
	
	ArrayValueX[3][0] = 200;
	ArrayValueY[3][0] = 350;

	ArrayValueX[3][1] = 300;
	ArrayValueY[3][1] = 350;

	ArrayValueX[3][2] = 400;
	ArrayValueY[3][2] = 350;

	ArrayValueX[3][3] = 500;
	ArrayValueY[3][3] = 350;
}

void SetButtonNumber(Button& Button1, Button& Button2, Button& Button3, Button& Button4,
					 Button& Button5, Button& Button6, Button& Button7, Button& Button8, 
					 Button& Button9, Button& Button10, Button& Button11, Button& Button12,
					 Button& Button13, Button& Button14, Button& Button15)
{
	int cycleCount = 0;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			if(ArrayPuzzle[i][j] != 0)
			{
				cycleCount++;

				switch(cycleCount)
				{
				case 1:
					{
						Button1.SetNumber(ArrayPuzzle[i][j]);
						Button1.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
						break;
					}
				case 2:
					{
						Button2.SetNumber(ArrayPuzzle[i][j]);
						Button2.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
						break;
					}
				case 3:
				{
					Button3.SetNumber(ArrayPuzzle[i][j]);
					Button3.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 4:
				{
					Button4.SetNumber(ArrayPuzzle[i][j]);
					Button4.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 5:
				{
					Button5.SetNumber(ArrayPuzzle[i][j]);
					Button5.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 6:
				{
					Button6.SetNumber(ArrayPuzzle[i][j]);
					Button6.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 7:
				{
					Button7.SetNumber(ArrayPuzzle[i][j]);
					Button7.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 8:
				{
					Button8.SetNumber(ArrayPuzzle[i][j]);
					Button8.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 9:
				{
					Button9.SetNumber(ArrayPuzzle[i][j]);
					Button9.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 10:
				{
					Button10.SetNumber(ArrayPuzzle[i][j]);
					Button10.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 11:
				{
					Button11.SetNumber(ArrayPuzzle[i][j]);
					Button11.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 12:
				{
					Button12.SetNumber(ArrayPuzzle[i][j]);
					Button12.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 13:
				{
					Button13.SetNumber(ArrayPuzzle[i][j]);
					Button13.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 14:
				{
					Button14.SetNumber(ArrayPuzzle[i][j]);
					Button14.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}
				case 15:
				{
					Button15.SetNumber(ArrayPuzzle[i][j]);
					Button15.SetCoord(ArrayValueX[i][j], ArrayValueY[i][j]);
					break;
				}

				//default:
					
				}
			}
		}
}

void SetButtonTextureRect(Button& Button1, Button& Button2, Button& Button3, Button& Button4,
					 Button& Button5, Button& Button6, Button& Button7, Button& Button8, 
					 Button& Button9, Button& Button10, Button& Button11, Button& Button12,
					 Button& Button13, Button& Button14, Button& Button15)
{
	switch(Button1.GetNumber())
	{
	case 1:
		Button1.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button1.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button1.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button1.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button1.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button1.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button1.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button1.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button1.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button1.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button1.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button1.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button1.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button1.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button1.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button1.SetNumber(0);
	}

	switch(Button2.GetNumber())
	{
	case 1:
		Button2.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button2.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button2.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button2.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button2.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button2.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button2.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button2.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button2.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button2.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button2.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button2.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button2.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button2.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button2.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button2.SetNumber(0);
	}

	switch(Button3.GetNumber())
	{
	case 1:
		Button3.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button3.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button3.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button3.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button3.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button3.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button3.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button3.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button3.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button3.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button3.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button3.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button3.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button3.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button3.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button3.SetNumber(0);
	}

	switch(Button4.GetNumber())
	{
	case 1:
		Button4.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button4.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button4.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button4.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button4.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button4.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button4.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button4.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button4.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button4.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button4.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button4.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button4.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button4.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button4.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button4.SetNumber(0);
	}
	
	switch(Button5.GetNumber())
	{
	case 1:
		Button5.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button5.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button5.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button5.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button5.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button5.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button5.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button5.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button5.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button5.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button5.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button5.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button5.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button5.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button5.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button5.SetNumber(0);
	}

	switch(Button6.GetNumber())
	{
	case 1:
		Button6.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button6.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button6.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button6.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button6.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button6.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button6.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button6.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button6.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button6.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button6.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button6.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button6.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button6.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button6.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button6.SetNumber(0);
	}
	
	switch(Button7.GetNumber())
	{
	case 1:
		Button7.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button7.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button7.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button7.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button7.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button7.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button7.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button7.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button7.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button7.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button7.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button7.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button7.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button7.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button7.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button7.SetNumber(0);
	}
	
	switch(Button8.GetNumber())
	{
	case 1:
		Button8.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button8.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button8.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button8.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button8.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button8.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button8.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button8.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button8.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button8.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button8.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button8.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button8.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button8.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button8.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button8.SetNumber(0);
	}
	
	switch(Button9.GetNumber())
	{
	case 1:
		Button9.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button9.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button9.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button9.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button9.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button9.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button9.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button9.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button9.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button9.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button9.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button9.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button9.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button9.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button9.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button9.SetNumber(0);
	}
	
	switch(Button10.GetNumber())
	{
	case 1:
		Button10.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button10.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button10.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button10.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button10.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button10.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button10.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button10.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button10.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button10.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button10.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button10.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button10.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button10.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button10.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button10.SetNumber(0);
	}
	
	switch(Button11.GetNumber())
	{
	case 1:
		Button11.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button11.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button11.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button11.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button11.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button11.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button11.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button11.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button11.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button11.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button11.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button11.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button11.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button11.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button11.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button11.SetNumber(0);
	}
	
	switch(Button12.GetNumber())
	{
	case 1:
		Button12.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button12.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button12.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button12.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button12.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button12.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button12.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button12.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button12.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button12.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button12.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button12.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button12.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button12.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button12.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button12.SetNumber(0);
	}
	
	switch(Button13.GetNumber())
	{
	case 1:
		Button13.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button13.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button13.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button13.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button13.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button13.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button13.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button13.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button13.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button13.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button13.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button13.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button13.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button13.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button13.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button13.SetNumber(0);
	}
	
	switch(Button14.GetNumber())
	{
	case 1:
		Button14.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button14.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button14.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button14.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button14.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button14.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button14.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button14.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button14.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button14.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button14.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button14.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button14.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button14.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button14.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button14.SetNumber(0);
	}
	
	switch(Button15.GetNumber())
	{
	case 1:
		Button15.SetTextureRect(0, 0, 100, 100);
		break;
	case 2:
		Button15.SetTextureRect(100, 0, 100, 100);
		break;
	case 3:
		Button15.SetTextureRect(200, 0, 100, 100);
		break;
	case 4:
		Button15.SetTextureRect(300, 0, 100, 100);
		break;
	case 5:
		Button15.SetTextureRect(0, 100, 100, 100);
		break;
	case 6:
		Button15.SetTextureRect(100, 100, 100, 100);
		break;
	case 7:
		Button15.SetTextureRect(200, 100, 100, 100);
		break;
	case 8:
		Button15.SetTextureRect(300, 100, 100, 100);
		break;
	case 9:
		Button15.SetTextureRect(0, 200, 100, 100);
		break;
	case 10:
		Button15.SetTextureRect(100, 200, 100, 100);
		break;
	case 11:
		Button15.SetTextureRect(200, 200, 100, 100);
		break;
	case 12:
		Button15.SetTextureRect(300, 200, 100, 100);
		break;
	case 13:
		Button15.SetTextureRect(0, 300, 100, 100);
		break;
	case 14:
		Button15.SetTextureRect(100, 300, 100, 100);
		break;
	case 15:
		Button15.SetTextureRect(200, 300, 100, 100);
		break;
	default:
		Button15.SetNumber(0);
	}
}

void TestButtonLeftRightUpDown(Button& Button)
{
	int n = 0;
	int m = 0;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			if(ArrayPuzzle[i][j] == Button.GetNumber())
			{
				n = i;
				m = j;

				if(i == 0 && j == 0)
				{
					if(EmptySpace[0][1] == 0)
						Right = true;

					if(EmptySpace[1][0] == 0)
						Down = true;
				}
			
				if(i == 0 && j == 1)
				{
					if(EmptySpace[0][0] == 0)
						Left = true;

					if(EmptySpace[0][2] == 0)
						Right = true;

					if(EmptySpace[1][1] == 0)
						Down = true;
				}

				if(i == 0 && j == 2)
				{
					if(EmptySpace[0][1] == 0)
						Left = true;
					
					if(EmptySpace[0][3] == 0)
						Right = true;

					if(EmptySpace[1][2] == 0)
						Down = true;
				}

				if(i == 0 && j == 3)
				{
					if(EmptySpace[0][2] == 0)
						Left = true;

					if(EmptySpace[1][3] == 0)
						Down = true;
				}

				if(i == 1 && j == 0)
				{
					if(EmptySpace[0][0] == 0)
						Up = true;

					if(EmptySpace[1][1] == 0)
						Right = true;

					if(EmptySpace[2][0] == 0)
						Down = true;
				}

				if(i == 1 && j == 1)
				{
					if(EmptySpace[1][0] == 0)
						Left = true;

					if(EmptySpace[0][1] == 0)
						Up = true;

					if(EmptySpace[1][2] == 0)
						Right = true;

					if(EmptySpace[2][1] == 0)
						Down = true;
				}

				if(i == 1 && j == 2)
				{
					if(EmptySpace[1][1] == 0)
						Left = true;

					if(EmptySpace[0][2] == 0)
						Up = true;

					if(EmptySpace[1][3] == 0)
						Right = true;

					if(EmptySpace[2][2] == 0)
						Down = true;
				}

				if(i == 1 && j == 3)
				{
					if(EmptySpace[1][2] == 0)
						Left = true;

					if(EmptySpace[0][3] == 0)
						Up = true;

					if(EmptySpace[2][3] == 0)
						Down = true;
				}

				if(i == 2 && j == 0)
				{
					if(EmptySpace[1][0] == 0)
						Up = true;

					if(EmptySpace[2][1] == 0)
						Right = true;

					if(EmptySpace[3][0] == 0)
						Down = true;
				}

				if(i == 2 && j == 1)
				{
					if(EmptySpace[2][0] == 0)
						Left = true;

					if(EmptySpace[1][1] == 0)
						Up = true;

					if(EmptySpace[2][2] == 0)
						Right = true;

					if(EmptySpace[3][1] == 0)
						Down = true;
				}

				if(i == 2 && j == 2)
				{
					if(EmptySpace[2][1] == 0)
						Left = true;

					if(EmptySpace[1][2] == 0)
						Up = true;

					if(EmptySpace[2][3] == 0)
						Right = true;

					if(EmptySpace[3][2] == 0)
						Down = true;
				}

				if(i == 2 && j == 3)
				{
					if(EmptySpace[2][2] == 0)
						Left = true;

					if(EmptySpace[1][3] == 0)
						Up = true;

					if(EmptySpace[3][3] == 0)
						Down = true;
				}

				if(i == 3 && j == 0)
				{
					if(EmptySpace[2][0] == 0)
						Up = true;

					if(EmptySpace[3][1] == 0)
						Right = true;
				}

				if(i == 3 && j == 1)
				{
					if(EmptySpace[3][0] == 0)
						Left = true;

					if(EmptySpace[2][1] == 0)
						Up = true;

					if(EmptySpace[3][2] == 0)
						Right = true;
				}

				if(i == 3 && j == 2)
				{
					if(EmptySpace[3][1] == 0)
						Left = true;

					if(EmptySpace[2][2] == 0)
						Up = true;

					if(EmptySpace[3][3] == 0)
						Right = true;
				}

				if(i == 3 && j == 3)
				{
					if(EmptySpace[3][2] == 0)
						Left = true;

					if(EmptySpace[2][3] == 0)
						Up = true;
				}
			}
		}

		Button.SetCoordxStart(Button.GetCoordX());
		Button.SetCoordyStart(Button.GetCoordY());

	if(Right)
	{
		Button.SetLRUD(false, true, false, false);
		//Button.SetCoord(Button.GetCoordX() + 100, Button.GetCoordY()); 
		ArrayPuzzle[n][m + 1] = ArrayPuzzle[n][m];
		ArrayPuzzle[n][m] = 0;
		SelectButton = false;
		ButtonClick.play();
	}

	if(Left)
	{
		Button.SetLRUD(true, false, false, false);
		//Button.SetCoord(Button.GetCoordX() - 100, Button.GetCoordY());
		ArrayPuzzle[n][m - 1] = ArrayPuzzle[n][m];
		ArrayPuzzle[n][m] = 0;
		SelectButton = false;
		ButtonClick.play();
	}

	if(Down)
	{
		Button.SetLRUD(false, false, false, true);
		//Button.SetCoord(Button.GetCoordX(), Button.GetCoordY() + 100);
		ArrayPuzzle[n + 1][m] = ArrayPuzzle[n][m];
		ArrayPuzzle[n][m] = 0;
		SelectButton = false;
		ButtonClick.play();
	}

	if(Up)
	{
		Button.SetLRUD(false, false, true, false);
		//Button.SetCoord(Button.GetCoordX(), Button.GetCoordY() - 100);
		ArrayPuzzle[n - 1][m] = ArrayPuzzle[n][m];
		ArrayPuzzle[n][m] = 0;
		SelectButton = false;
		ButtonClick.play();
	}

	Right = false;
	Left = false;
	Up = false;
	Down = false;
}

void TestButtonSelection(Button& Button1, Button& Button2, Button& Button3, Button& Button4,
					 Button& Button5, Button& Button6, Button& Button7, Button& Button8, 
					 Button& Button9, Button& Button10, Button& Button11, Button& Button12,
					 Button& Button13, Button& Button14, Button& Button15, Vector2i MouseCoord)
{
	if(MouseCoord.x > Button1.GetCoordX() && MouseCoord.x < (Button1.GetCoordX() + 100) && MouseCoord.y > Button1.GetCoordY() && MouseCoord.y < (Button1.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button1);
	}

	if(MouseCoord.x > Button2.GetCoordX() && MouseCoord.x < (Button2.GetCoordX() + 100) && MouseCoord.y > Button2.GetCoordY() && MouseCoord.y < (Button2.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button2);
	}

	if(MouseCoord.x > Button3.GetCoordX() && MouseCoord.x < (Button3.GetCoordX() + 100) && MouseCoord.y > Button3.GetCoordY() && MouseCoord.y < (Button3.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button3);
	}

	if(MouseCoord.x > Button4.GetCoordX() && MouseCoord.x < (Button4.GetCoordX() + 100) && MouseCoord.y > Button4.GetCoordY() && MouseCoord.y < (Button4.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button4);
	}

	if(MouseCoord.x > Button5.GetCoordX() && MouseCoord.x < (Button5.GetCoordX() + 100) && MouseCoord.y > Button5.GetCoordY() && MouseCoord.y < (Button5.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button5);
	}

	if(MouseCoord.x > Button6.GetCoordX() && MouseCoord.x < (Button6.GetCoordX() + 100) && MouseCoord.y > Button6.GetCoordY() && MouseCoord.y < (Button6.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button6);
	}

	if(MouseCoord.x > Button7.GetCoordX() && MouseCoord.x < (Button7.GetCoordX() + 100) && MouseCoord.y > Button7.GetCoordY() && MouseCoord.y < (Button7.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button7);
	}

	if(MouseCoord.x > Button8.GetCoordX() && MouseCoord.x < (Button8.GetCoordX() + 100) && MouseCoord.y > Button8.GetCoordY() && MouseCoord.y < (Button8.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button8);
	}

	if(MouseCoord.x > Button9.GetCoordX() && MouseCoord.x < (Button9.GetCoordX() + 100) && MouseCoord.y > Button9.GetCoordY() && MouseCoord.y < (Button9.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button9);
	}

	if(MouseCoord.x > Button10.GetCoordX() && MouseCoord.x < (Button10.GetCoordX() + 100) && MouseCoord.y > Button10.GetCoordY() && MouseCoord.y < (Button10.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button10);
	}

	if(MouseCoord.x > Button11.GetCoordX() && MouseCoord.x < (Button11.GetCoordX() + 100) && MouseCoord.y > Button11.GetCoordY() && MouseCoord.y < (Button11.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button11);
	}

	if(MouseCoord.x > Button12.GetCoordX() && MouseCoord.x < (Button12.GetCoordX() + 100) && MouseCoord.y > Button12.GetCoordY() && MouseCoord.y < (Button12.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button12);
	}

	if(MouseCoord.x > Button13.GetCoordX() && MouseCoord.x < (Button13.GetCoordX() + 100) && MouseCoord.y > Button13.GetCoordY() && MouseCoord.y < (Button13.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button13);
	}

	if(MouseCoord.x > Button14.GetCoordX() && MouseCoord.x < (Button14.GetCoordX() + 100) && MouseCoord.y > Button14.GetCoordY() && MouseCoord.y < (Button14.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button14);
	}

	if(MouseCoord.x > Button15.GetCoordX() && MouseCoord.x < (Button15.GetCoordX() + 100) && MouseCoord.y > Button15.GetCoordY() && MouseCoord.y < (Button15.GetCoordY() + 100))
	{
		TestButtonLeftRightUpDown(Button15);
	}
}

void TestResetButtonSelection(Button& Button1, Button& Button2, Button& Button3, Button& Button4,
					 Button& Button5, Button& Button6, Button& Button7, Button& Button8, 
					 Button& Button9, Button& Button10, Button& Button11, Button& Button12,
					 Button& Button13, Button& Button14, Button& Button15, Vector2i MouseCoord)
{
	if(MouseCoord.x > 300 && MouseCoord.x < 500 && MouseCoord.y > 500 && MouseCoord.y < 550)
	{
		Randomize();
		SetArrayXYCoord();

	SetButtonNumber(Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8,
		Button9, Button10, Button11, Button12, Button13, Button14, Button15);

	SetButtonTextureRect(Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8,
		Button9, Button10, Button11, Button12, Button13, Button14, Button15);

	AssignArrayPuzzleEmptySpace();
	}
}

bool SetTextureSprites()
{
	if(!ResetButtonTexturePressed.loadFromFile("Images/ResetButtonIn.png"))
		return false;

	if(!ResetButtonTextureUnPressed.loadFromFile("Images/ResetButtonOut.png"))
		return false;

	if(!ScreenOneTexture.loadFromFile("Images/StepOne.png"))
		return false;

	if(!ScreenTwoTexture.loadFromFile("Images/StepTwo.png"))
		return false;

	ResetButtonPressed.setTexture(ResetButtonTexturePressed);
	ResetButtonUnPressed.setTexture(ResetButtonTextureUnPressed);
	ScreenOneSprite.setTexture(ScreenOneTexture);
	ScreenTwoSprite.setTexture(ScreenTwoTexture);

	return true;
}

bool loadMusic()
{
	if(!ButtonClick.openFromFile("Sound/button-20.wav"))
		return false;

	if(!ResetButton.openFromFile("Sound/2_dice_throw_on_game_board.wav"))
		return false;

	if(!SolvedPuzzle.openFromFile("Sound/elevator_bell.wav"))
		return false;

	if(!pageTurn.openFromFile("Sound/book_page_turn_001.wav"))
		return false;

	pageTurn.setVolume(40);

	if(!Track1.openFromFile("Sound/classical_piano_music_track_the_tempest_.wav"))
		return false;

	if(!Track2.openFromFile("Sound/classical_piano_music_track_a_short_walk_.wav"))
		return false;

	if(!Track3.openFromFile("Sound/classical_piano_music_track_and_if_a_boat_came_by_.wav"))
		return false;

	if(!Track4.openFromFile("Sound/classical_piano_music_track_great_glen_.wav"))
		return false;

	return true;
}

bool loadFont()
{
	if(!font.loadFromFile("Fonts/Leo Arrow.ttf"))
		return false;

	return true;
}