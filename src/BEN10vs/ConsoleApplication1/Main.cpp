#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>
using namespace std;
using namespace sf;



sf::RenderWindow window(sf::VideoMode(1680, 1050), "Ben 10");

CircleShape mouse((5.f)); //mouse sprite variable i.e sahm

CircleShape volumeup(20, 3);

CircleShape volumedown(20, 3);

enum Transition {
    MainMenu,
    secmenu,
    Cntinue,
    Options,
	difficulty,
    Gameplay,
};

Transition current = MainMenu;

enum difi {
	Easy,
	Medium,
	Hard,
};

static difi currentdifi = Easy;




Sprite bg1;
Sprite bg2;



Texture bgtexture;
Texture bg2texture;



Font font;
Font font1;


Text Startgame;
Text newgame;
Text Continue;
Text options;
Text Volume;
Text Difficulty;
Text easy;
Text normal;
Text hard;
Text Backtostart;
Text Backtosecmenu;

Music bentenmusic;

Clock volupcooldown;
Clock voldowncooldown; 

static int volume = 100;







void Update();
void Start();
void Draw();
float deltaTime;
int main()
{
    Start();
    Clock clock;
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        Update();
        Draw();
    }
    return 0;
}
void Start()
{

    bgtexture.loadFromFile("C:/Users/pc/Desktop/Benten/Assets/Images/bg1f.jpg");
    bg1.setTexture(bgtexture);
    bg1.setPosition(0.f, 0.f);
    Vector2u textureSize = bgtexture.getSize();
    Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    bg1.setScale(scaleX, scaleY);


    bg2texture.loadFromFile("C:/Users/pc/Desktop/Benten/Assets/Images/bg2.jpg");
    bg2.setTexture(bg2texture);
    bg2.setPosition(0.f, 0.f);
    Vector2u textureSize2 = bg2texture.getSize();
    Vector2u windowSize2 = window.getSize();
    float scaleX2 = static_cast<float>(windowSize2.x) / textureSize2.x;
    float scaleY2 = static_cast<float>(windowSize2.y) / textureSize2.y;
    bg2.setScale(scaleX2, scaleY2);




    // code here is only executed at the start of the program
    mouse.setFillColor(Color::Transparent);//set the circle fill color in START functio i.e color of sahm 10

    font.loadFromFile("C:/Users/pc/Desktop/Benten/Assets/Font/ObelixProB-cyr.ttf");
    font1.loadFromFile("C:/Users/pc/Desktop/Benten/Assets/Font/FSEX300.ttf");

    
	bentenmusic.openFromFile("C:/Users/pc/Desktop/Benten/Assets/Sound/bentensound.ogg");

   

 


    Startgame.setFont(font1);
    Startgame.setString("Start game");
    Startgame.setCharacterSize(100);
    Startgame.setFillColor(Color :: White);

    newgame.setFont(font);
    newgame.setString("New Game");
    newgame.setCharacterSize(35) ;
    newgame.setFillColor(Color::White);

    Continue.setFont(font);
    Continue.setString("Continue");
    Continue.setCharacterSize(35);
    Continue.setFillColor(Color :: White);

    options.setFont(font);
    options.setString("Options");
    options.setCharacterSize(35);
    options.setFillColor(Color::White);

    Volume.setFont(font);
    Volume.setString("Volume : 100%");
    Volume.setCharacterSize(35);
    Volume.setFillColor(Color::White);

    Difficulty.setFont(font);
    Difficulty.setString("Difficulty");
    Difficulty.setCharacterSize(35);
    Difficulty.setFillColor(Color::White);

    easy.setFont(font);
    easy.setString("Easy");
    easy.setCharacterSize(35);
    easy.setFillColor(Color::White);

	normal.setFont(font);
	normal.setString("Normal");
	normal.setCharacterSize(35);
	normal.setFillColor(Color::White);

	hard.setFont(font);
	hard.setString("Hard");
	hard.setCharacterSize(35);
	hard.setFillColor(Color::White);

	Backtostart.setFont(font);
	Backtostart.setString("Back");
	Backtostart.setCharacterSize(35);
	Backtostart.setFillColor(Color::White);

	Backtosecmenu.setFont(font);
	Backtosecmenu.setString("Back");
	Backtosecmenu.setCharacterSize(35);
	Backtosecmenu.setFillColor(Color::White);

   


 

    float StartWidth = Startgame.getGlobalBounds().width;
    float StartHeight = Startgame.getGlobalBounds().height;
    Startgame.setPosition(((window.getSize().x - StartWidth) / 2.f) - 20.f, window.getSize().y - StartHeight - 100.f);

    float newgameWidth = newgame.getGlobalBounds().width;
    float newgameHeight = newgame.getGlobalBounds().height;
    newgame.setPosition(window.getSize().x - newgameWidth - 210.f, ((window.getSize().y - newgameHeight) / 2.f)-50);


    float Continuewsidth = Continue.getGlobalBounds().width;
    Continue.setPosition(newgame.getPosition().x + 10.f , newgame.getPosition().y + 85.f);


    options.setPosition(Continue.getPosition().x +10.f, Continue.getPosition().y + 85.f);


	Backtostart.setPosition(options.getPosition().x + 40.f, options.getPosition().y + 85.f);


    float volumeWidth = Volume.getGlobalBounds().width;
    float volumeHeight = Volume.getGlobalBounds().height;
    Volume.setPosition(window.getSize().x - volumeWidth - 150.f, ((window.getSize().y - volumeHeight) / 2.f) - 50);

    volumeup.setFillColor(Color :: White);
    volumeup.setPosition(
        Volume.getPosition().x + volumeWidth / 2.f - volumeup.getGlobalBounds().width / 2.f - 5.f,
        Volume.getPosition().y - volumeup.getGlobalBounds().height - 40.f
    );


    volumedown.setFillColor(Color::White);
    volumedown.setOrigin(volumedown.getRadius(), volumedown.getRadius());
    volumedown.setRotation(180.f);
    volumedown.setPosition(Volume.getPosition().x + volumeWidth / 2.f,
        Volume.getPosition().y + Volume.getGlobalBounds().height + (volumedown.getGlobalBounds().height / 2.f) + 20.f);

    float Difficultywidth = Difficulty.getGlobalBounds().width;
    Difficulty.setPosition(volumedown.getPosition().x - Difficultywidth / 2.f,
        volumedown.getPosition().y + volumedown.getGlobalBounds().height / 2.f + 30.f);

	Backtosecmenu.setPosition(Difficulty.getPosition().x + 60.f, Difficulty.getPosition().y + 85.f);

    float easyWidth = easy.getGlobalBounds().width;
    float easyHeight = easy.getGlobalBounds().height;
    easy.setPosition(window.getSize().x - easyWidth - 250.f, ((window.getSize().y - easyHeight) / 2.f) - 50);
	


    float normalWidth = normal.getGlobalBounds().width;
	normal.setPosition(easy.getPosition().x - 30.f, easy.getPosition().y + 85.f);

	float hardWidth = hard.getGlobalBounds().width;
	hard.setPosition(normal.getPosition().x + 30.f, normal.getPosition().y + 85.f);

    bentenmusic.play();
   bentenmusic.setLoop(true);


}
void Update()
{
    // code here is executed every frame since the start of the program
    Vector2i mousePosScreen = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosScreen);
    mouse.setPosition(mouseWorldPos); // Set the circle follow


    if (current == MainMenu) {
        if (mouse.getGlobalBounds().intersects(Startgame.getGlobalBounds())) {
            Startgame.setFillColor(Color::Yellow);
        }
        if ((Keyboard::isKeyPressed(Keyboard::Enter))) {
            current = secmenu;

        }
        if (mouse.getGlobalBounds().intersects(Startgame.getGlobalBounds())) {
            if (Mouse::isButtonPressed(Mouse::Left))
            {

                current = secmenu;
            }
        }

        else {
            Startgame.setFillColor(Color::White);
        }
    }
    if (current == secmenu) 
    {
        if (mouse.getGlobalBounds().intersects(newgame.getGlobalBounds())) {
            newgame.setFillColor(Color::Yellow);
        }
        if (mouse.getGlobalBounds().intersects(newgame.getGlobalBounds())) {
            if (Mouse::isButtonPressed(Mouse::Left))
            {

                current = Gameplay;
            }
        }
        else {
            newgame.setFillColor(Color::White);
        }

    }
    if (current == secmenu)
    {
        if (mouse.getGlobalBounds().intersects(Continue.getGlobalBounds())) {
            Continue.setFillColor(Color::Yellow);
        }
        if (mouse.getGlobalBounds().intersects(Continue.getGlobalBounds())) {
            if (Mouse::isButtonPressed(Mouse::Left))
            {

                current = Cntinue;
            }
        }

        else {
            Continue.setFillColor(Color::White);
        }

    }
    if (current == secmenu)
    {
        if (mouse.getGlobalBounds().intersects(options.getGlobalBounds())) {
            options.setFillColor(Color::Yellow);
        }
        if (mouse.getGlobalBounds().intersects(options.getGlobalBounds())) {
            if (Mouse::isButtonPressed(Mouse::Left))
            {

                current = Options;
            }
        }

        else {
            options.setFillColor(Color::White);
        }
		if (current == secmenu)
		{
			if (mouse.getGlobalBounds().intersects(Backtostart.getGlobalBounds())) {
				Backtostart.setFillColor(Color::Yellow);
			}
			if (mouse.getGlobalBounds().intersects(Backtostart.getGlobalBounds())) {
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					current = MainMenu;
				}
			}
			else {
				Backtostart.setFillColor(Color::White);
			}
		}

    }
    if (current == Options)
    {
        if (mouse.getGlobalBounds().intersects(Volume.getGlobalBounds())) {
            Volume.setFillColor(Color::Yellow);
            volumedown.setFillColor(Color::Yellow);
            volumeup.setFillColor(Color::Yellow);
        }
        else if (mouse.getGlobalBounds().intersects(volumeup.getGlobalBounds())) {
            volumeup.setFillColor(Color::Yellow);
            Volume.setFillColor(Color::Yellow);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if (volupcooldown.getElapsedTime().asMilliseconds() > 100) {
                    
                    if (volume < 100)
                        volume += 10;

                    ostringstream sv;
                    sv << "Volume : " << volume << "%";
                    Volume.setString(sv.str());
                    bentenmusic.setVolume(volume);
                }
                volupcooldown.restart();
            }
        }
        else if (mouse.getGlobalBounds().intersects(volumedown.getGlobalBounds())) {
            volumedown.setFillColor(Color::Yellow);
            Volume.setFillColor(Color::Yellow);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if (voldowncooldown.getElapsedTime().asMilliseconds() > 100) {
                   
                    if (volume > 0)
                        volume -= 10;

                    ostringstream sv;
                    sv << "Volume : " << volume << "%";
                    Volume.setString(sv.str());
                    bentenmusic.setVolume(volume);

                }
				voldowncooldown.restart();
            }
        }
        else if (mouse.getGlobalBounds().intersects(Difficulty.getGlobalBounds())) {
            Difficulty.setFillColor(Color::Yellow);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				current = difficulty;
			}

        }

        else {
            Volume.setFillColor(Color::White);
            volumeup.setFillColor(Color::White);
            volumedown.setFillColor(Color::White);
            Difficulty.setFillColor(Color::White);
        }

    }
    if (current == Options) {
		if (mouse.getGlobalBounds().intersects(Backtosecmenu.getGlobalBounds())) {
			Backtosecmenu.setFillColor(Color::Yellow);
		}
		if (mouse.getGlobalBounds().intersects(Backtosecmenu.getGlobalBounds())) {
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				current = secmenu;
			}
		}
		else {
			Backtosecmenu.setFillColor(Color::White);
		}
    }
    if (current == difficulty) {

        if (mouse.getGlobalBounds().intersects(easy.getGlobalBounds())) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                currentdifi = Easy;
                current = Options;
            }
        }
 
        if (mouse.getGlobalBounds().intersects(normal.getGlobalBounds())) 
        {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                currentdifi = Medium;
                current = Options;
            }
        }

        if (mouse.getGlobalBounds().intersects(hard.getGlobalBounds())) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                currentdifi = Hard;
                current = Options;
            }
        }


        easy.setFillColor(currentdifi == Easy ? Color(128, 128, 128) : Color::White);
        normal.setFillColor(currentdifi == Medium ? Color(128, 128, 128) : Color::White);
        hard.setFillColor(currentdifi == Hard ? Color(128, 128, 128) : Color::White);
    }
  
}

void Draw()
{
    // code here is executed every frame since the start of the program
    window.clear(); //clear every pixel on the screen
    window.draw(mouse);

    switch (current)
    {
    case MainMenu:
        window.draw(bg1);
        window.draw(Startgame);
        break;

    case secmenu:
        window.draw(bg2);
        window.draw(newgame);
        window.draw(Continue);
        window.draw(options);
		window.draw(Backtostart);
        break;

    case Cntinue:
        //load
        break;

    case Options:
        window.draw(bg2);
        window.draw(Volume);
        window.draw(volumeup);
        window.draw(volumedown);
        window.draw(Difficulty);
		window.draw(Backtosecmenu);
        break;
	case difficulty:
		window.draw(bg2);
		window.draw(easy);
		window.draw(normal);
		window.draw(hard);

    case Gameplay:
        switch (currentdifi)
        {
        case Easy:
            //load easy level
            break;
        case Medium:
            //load medium level
            break;
        case Hard:
            //load hard level
            break;
        }
        break;

    }


    window.display(); //Display sprites on screen
}
