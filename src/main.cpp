#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

//Random machine
std::random_device rd;
std::mt19937 mr(rd());
std::uniform_int_distribution<> dist(148, 662);

void countDown(std::unordered_map<short, sf::Sprite&>& nums, std::unordered_map<std::string, sf::Sprite&>& elements, sf::Clock& clock,bool& isGameOn)
{
    if (clock.getElapsedTime().asSeconds() >= 1)
    {
        if (elements.contains("ready"))
        {
            elements.erase("ready");
            elements.insert({ "3",nums.at(3) });
            clock.restart();
        }
        else if (elements.contains("3"))
        {
            elements.erase("3");
            elements.insert({ "2",nums.at(2) });
            clock.restart();
        }
        else if (elements.contains("2"))
        {
            elements.erase("2");
            elements.insert({ "1",nums.at(1) });
            clock.restart();
        }
        else if (elements.contains("1"))
        {
            elements.erase("1");
            elements.insert({ "0",nums.at(0) });
        }
        else { 
            isGameOn = true; clock.reset(); 
        }
    }
}

void moveDown(int move, sf::Sprite& bird, sf::Clock& clock)
{
    clock.start();
    auto dt = clock.restart().asSeconds();
    bird.move({ 0,move * dt });
}

void moveUp(int move, sf::Vector2f& initPos, sf::Sprite& bird, sf::Clock& clock, bool& isGameOn, bool& isJumping)
{
    if (bird.getPosition().y <= initPos.y-80)
    {
        isGameOn = true;
        isJumping = false;
    }
    else
    {
        clock.start();
        auto dt = clock.restart().asSeconds();
        bird.move({ 0,-move * dt });
    }
}

void birdAnim(sf::Sprite& bird, short& texID,sf::Texture& up, sf::Texture& mid, sf::Texture& down,sf::Clock& clock)
{
    if (clock.getElapsedTime().asMilliseconds() >= 200)
    {
        if (texID == 0) { bird.setTexture(mid); texID++; clock.restart(); }
        else if (texID == 1) { bird.setTexture(down); texID++; clock.restart(); }
        else if (texID == 2) { bird.setTexture(up); texID -= 2; clock.restart(); }
    }
}

void collisionCheck(sf::Sprite& bird, std::vector<sf::Sprite> pipes, bool& isDead)
{
    sf::FloatRect pipeBounds;
    auto birdBounds = bird.getGlobalBounds();
    for (auto& pipe : pipes)
    {
        pipeBounds = pipe.getGlobalBounds();
        if (const std::optional intersection = birdBounds.findIntersection(pipeBounds)) isDead = true;
    }
}

void dead(sf::Clock& clock, bool& isMenu, std::unordered_map<std::string, sf::Sprite&>& elements)
{
    if (clock.getElapsedTime().asSeconds() >= 2)
    {
        isMenu = true;
        clock.reset();
        elements.erase("topPipeA");
        elements.erase("topPipeB");
        elements.erase("topPipeC");
        elements.erase("bottomPipeA");
        elements.erase("bottomPipeB");
        elements.erase("bottomPipeC");
    }
    
}

void changePipesGapPos(std::vector<sf::Sprite>& pipes, short index, float topY)
{
    //int middle = (2 * topY + 200) / 2;

    if (index == 0)
    {
        pipes.at(0).setPosition({ pipes.at(0).getPosition().x,topY });
        pipes.at(3).setPosition({ pipes.at(3).getPosition().x,topY + 200 });
    }
    else if (index == 1)
    {
        pipes.at(1).setPosition({ pipes.at(1).getPosition().x,topY });
        pipes.at(4).setPosition({ pipes.at(4).getPosition().x,topY + 200 });
    }
    else if (index == 2)
    {
        pipes.at(2).setPosition({ pipes.at(2).getPosition().x,topY });
        pipes.at(5).setPosition({ pipes.at(5).getPosition().x,topY + 200 });
    }else{}
}

void movePipes(int move, std::vector<sf::Sprite>& pipes, sf::Clock& clock, bool& isDead, sf::Sprite& bird)
{
    collisionCheck(bird, pipes, isDead);
    clock.start();
    auto dt = clock.restart().asSeconds();
    for (short i = 0; i < pipes.size(); ++i)
    {
        if (pipes.at(i).getPosition().x <= -104)
            {
                pipes.at(i).setPosition({ 1108,pipes.at(i).getPosition().y });
                changePipesGapPos(pipes, i, dist(mr));
            }
        else pipes.at(i).move({ move * dt,0 });
    }
}

int main() {
    //Maps of elements
    std::unordered_map<std::string, sf::Sprite&> elements; // UI
    std::unordered_map<short, sf::Sprite&> nums; // Nums
    sf::Texture zeroTex("assets/UI/Numbers/0.png");
    sf::Sprite zero(zeroTex);
    zero.setScale({ 2,2 });
    zero.setPosition({ 279,100 });
    nums.insert({ 0, zero });
    sf::Texture oneTex("assets/UI/Numbers/1.png");
    sf::Sprite one(oneTex);
    one.setScale({ 2,2 });
    one.setPosition({ 279,100 });
    nums.insert({ 1, one});
    sf::Texture twoTex("assets/UI/Numbers/2.png");
    sf::Sprite two(twoTex);
    two.setScale({ 2,2 });
    two.setPosition({ 279,100 });
    nums.insert({ 2, two });
    sf::Texture threeTex("assets/UI/Numbers/3.png");
    sf::Sprite three(threeTex);
    three.setScale({ 2,2 });
    three.setPosition({ 279,100 });
    nums.insert({ 3, three });
    sf::Texture fourTex("assets/UI/Numbers/4.png");
    sf::Sprite four(fourTex);
    four.setScale({ 2,2 });
    four.setPosition({ 279,100 });
    nums.insert({ 4, four });
    sf::Texture fiveTex("assets/UI/Numbers/5.png");
    sf::Sprite five(fiveTex);
    five.setScale({ 2,2 });
    five.setPosition({ 279,100 });
    nums.insert({ 5, five });
    sf::Texture sixTex("assets/UI/Numbers/6.png");
    sf::Sprite six(sixTex);
    six.setScale({ 2,2 });
    six.setPosition({ 279,100 });
    nums.insert({ 6, six});
    sf::Texture sevenTex("assets/UI/Numbers/7.png");
    sf::Sprite seven(sevenTex);
    seven.setScale({ 2,2 });
    seven.setPosition({ 279,100 });
    nums.insert({ 7, seven});
    sf::Texture eightTex("assets/UI/Numbers/8.png");
    sf::Sprite eight(eightTex);
    eight.setScale({ 2,2 });
    eight.setPosition({ 279,100 });
    nums.insert({ 8, eight });
    sf::Texture nineTex("assets/UI/Numbers/9.png");
    sf::Sprite nine(nineTex);
    nine.setScale({ 2,2 });
    nine.setPosition({ 279,100 });
    nums.insert({ 9, nine});
    // Main Window 
    sf::RenderWindow window(sf::VideoMode({ 606,1010 }), "Clappy Bird", sf::Style::Titlebar, sf::State::Windowed);
    sf::Image icon("assets/GameObjects/yellowbird-upflap.png");
    window.setIcon(icon);
    window.setFramerateLimit(60);
    window.setPosition({ 657,0 });
    // BG  
    sf::Texture bgTex("assets/GameObjects/background-day.png");
    sf::Sprite bg(bgTex);
    bg.setScale({ 2.1f,2.1f });
    // Title
    sf::Texture titles("assets/UI/message.png");
    sf::Sprite title(titles);
    title.setTextureRect({ {0,0},{184,48} });
    title.setScale({ 2,2 });
    title.setPosition({ 119,100 });
    elements.insert({"title", title});
    // Ready
    sf::Sprite ready(titles);
    ready.setTextureRect({ {0,102},{183,50} });
    ready.setScale({ 2,2 });
    ready.setPosition({ 120,100 });
    // Tap
    sf::Sprite tap(titles);
    tap.setTextureRect({ { 35,169 }, { 114,98 } });
    tap.setScale({ 2,2 });
    tap.setPosition({ 189,700 });
    elements.insert({ "tap",tap });
    //Game Over
    sf::Texture gameOverTex("assets/UI/gameover.png");
    sf::Sprite gameOver(gameOverTex);
    gameOver.setScale({ 2,2 });
    gameOver.setPosition({ 111,106 });
    //Bird
    sf::Texture birdUpTex(icon);
    sf::Texture birdMidTex("assets/GameObjects/yellowbird-midflap.png");
    sf::Texture birdDownTex("assets/GameObjects/yellowbird-downflap.png");
    sf::Sprite bird(birdUpTex);
    short texID = 0;
    bird.setScale({ 2,2 });
    bird.setPosition({ 269,457 });
    elements.insert({ "bird",bird });
    sf::Vector2f initPos = bird.getPosition();
    //Pipes 
    sf::Texture pipeTex("assets/GameObjects/pipe-green.png");
    std::vector<sf::Sprite> pipes;
    int move = 0;
    //Tops
    sf::Sprite topPipeA(pipeTex);
    topPipeA.setScale({ 2,-2.8f });
    topPipeA.setPosition({ 606,409 });
    pipes.push_back(topPipeA);
    sf::Sprite topPipeB(pipeTex);
    topPipeB.setScale({ 2,-2.8f });
    topPipeB.setPosition({ 1004,409 });
    pipes.push_back(topPipeB);
    sf::Sprite topPipeC(pipeTex);
    topPipeC.setScale({ 2,-2.8f });
    topPipeC.setPosition({ 1402,409 });
    pipes.push_back(topPipeC);
    //Bottoms
    sf::Sprite bottomPipeA(pipeTex);
    bottomPipeA.setScale({ 2,2.8f });
    bottomPipeA.setPosition({ 606,573 });
    pipes.push_back(bottomPipeA);
    sf::Sprite bottomPipeB(pipeTex);
    bottomPipeB.setScale({ 2,2.8f });
    bottomPipeB.setPosition({ 1004,573 });
    pipes.push_back(bottomPipeB);
    sf::Sprite bottomPipeC(pipeTex);
    bottomPipeC.setScale({ 2,2.8f });
    bottomPipeC.setPosition({ 1402,573 });
    pipes.push_back(bottomPipeC);
    //Sounds: Hit | Point | Jump
    sf::SoundBuffer hitBuff("assets/SoundEfects/hit.wav");
    sf::Sound hitSound(hitBuff);
    hitSound.setVolume(30);
    sf::SoundBuffer pointBuff("assets/SoundEfects/point.wav");
    sf::Sound pointSound(pointBuff);
    pointSound.setVolume(20);
    sf::SoundBuffer jumpBuff("assets/SoundEfects/wing.wav");
    sf::Sound jumpSound(jumpBuff);
    jumpSound.setVolume(10);
    //Game Loop & Clock & Phases & Others
    bool isMenu = true;
    bool isGameOn = false;
    bool isCountDown = false;
    bool isJumping = false;
    bool isDead = false;

    sf::Clock gameClock;
    gameClock.stop();
    sf::Clock countClock;
    countClock.stop();
    sf::Clock jumpClock;
    jumpClock.stop();
    sf::Clock animation;
    animation.stop();
    sf::Clock pipeClock;
    pipeClock.stop();

    bool firstDeath = true;
    int score = 0;
    int moveDownVar = 450;
;   while (window.isOpen())
    {
        // Events
        while (const std::optional event = window.pollEvent())
        {
            // Close window 
            if (event->is<sf::Event::Closed>()) window.close();
            // Click
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    if (elements.contains("title") || elements.contains("tap"))
                    {
                        elements.erase("title"); elements.erase("tap");
                        elements.insert({ "ready",ready });

                        pipes.at(0).setPosition({ 606,409 });
                        pipes.at(1).setPosition({ 1004,409 });
                        pipes.at(2).setPosition({ 1402,409 });
                        pipes.at(3).setPosition({ 606,573 });
                        pipes.at(4).setPosition({ 1004,573 });
                        pipes.at(5).setPosition({ 1402,573 });

                        changePipesGapPos(pipes, 0, dist(mr));
                        changePipesGapPos(pipes, 1, dist(mr));
                        changePipesGapPos(pipes, 2, dist(mr));

                        jumpSound.setVolume(10);
                        firstDeath = true;
                        score = 0;
                        moveDownVar = 350;

                        isMenu = false;
                        isCountDown = true;
                    }
                    else if (isCountDown) {}
                    else if (isDead) jumpSound.setVolume(0);
                    else
                    {
                        moveDownVar = 450;
                        isJumping = true;
                        initPos = bird.getPosition();
                        jumpClock.reset();
                        jumpSound.play();
                    }
                }
                else if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Right)
                {
                    moveDownVar = 800;
                }
                
            }
        }
        // Game functions
        movePipes(move, pipes, pipeClock, isDead, bird);
        // Phases
        if (isCountDown)
        {
            countClock.start();
            countDown(nums, elements, countClock, isGameOn);
            elements.erase("0");
            elements.insert({ "topPipeA",pipes.at(0) });
            elements.insert({ "topPipeB",pipes.at(1) });
            elements.insert({ "topPipeC",pipes.at(2) });
            elements.insert({ "bottomPipeA",pipes.at(3) });
            elements.insert({ "bottomPipeB",pipes.at(4) });
            elements.insert({ "bottomPipeC",pipes.at(5) });
        }
        if (isDead)
        {
            isJumping = false;
            isGameOn = false;
            countClock.start();
            gameClock.reset();
            pipeClock.reset();
            elements.insert({ "gameOver",gameOver });
            move = 0;

            if (firstDeath)
            {
                hitSound.play();
                firstDeath = false;
            }

            dead(countClock,isMenu, elements);
        }
        if (isGameOn)
        {
            isCountDown = false;
            isDead = false;
            bird.setTexture(birdUpTex);
            move = -300;
            moveDown(moveDownVar, bird, gameClock);
        }
        if (isJumping)
        {
            isGameOn = false;
            gameClock.reset();
            animation.start();
            birdAnim(bird, texID, birdUpTex, birdMidTex, birdDownTex, animation);
            moveUp(800, initPos, bird, jumpClock, isGameOn,isJumping);
        }
        if (isMenu)
        {
            isGameOn = false;
            isDead = false;
            elements.at("bird").setPosition({ 269,457 });
            if (elements.contains("gameOver")) elements.erase("gameOver");
            if (elements.contains("topPipeA"))
            {
                std::cout << "Hello\n";
                pipes.at(0).setPosition({ 606,409 });
                pipes.at(1).setPosition({ 961,409 });
                pipes.at(2).setPosition({ 1316,409 });
                pipes.at(3).setPosition({ 606,573 });
                pipes.at(4).setPosition({ 961,573 });
                pipes.at(5).setPosition({ 1316,573 });
            }
            elements.insert({ "title", title });
            elements.insert({ "tap",tap });
        }
        // Clear screen
        window.clear();

        //Drawing
        window.draw(bg);
        for (const auto& pair : elements)
        {
            window.draw(pair.second);
        }
        // Update the window
        window.display();
    }

	return 0;
}