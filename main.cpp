#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include<iostream>
#include<string>
#include <windows.h>
using namespace sf;
const int BOK = 320;
const int NIZ = 480;
const int Width = 20;
const int Height = 20;
int ActiveF = 0, Score=0;
RenderWindow window(VideoMode(BOK, NIZ+50), "TETRIS", Style::Close);
struct Kord {   //Структура для зберігання координат кожного квадрата блоку
    int x;
    int y;
    int active; /// Якщо 0 то неактивний, якщо 1 то активний, якщо -1 то видалений
};

void LineDelete(Kord** mas,int y) {    //////////////////Видаляємо стрічку і зміщуємо стрічки, які вище
    for (int i = 0; i <= ActiveF; i++)
        for (int g = 0; g < 4; g++)

            if (mas[i][g].y == y && mas[i][g].active != -1)
            {
                mas[i][g].active = -1;
                Score++;
            }
            else if (mas[i][g].y < y)
                mas[i][g].y += Height;


}
int CheckLine(Kord** mas) {
    int Count = 0; bool Flag = false; /////////////Перевіряємо чи не зібралась повна стрічка
    for (int i = 0; i < 4; i++)

    {
        Count = 0;
    for (int j = 0; j <= ActiveF; j++)
        for (int g = 0; g < 4; g++)
            if (mas[j][g].y == mas[ActiveF][i].y && mas[j][g].active!=-1)
                Count++;
    if (Count == BOK/Width)
    {
        Flag = true;
        LineDelete(mas, mas[ActiveF][i].y);
        
    }
    }
    if (Flag)
        return 1;
    else return 0;
}

int CheckY(Kord** mas, int x, int y) {//Перевірка чи немає знизу блоків
    for (int i = 0; i <= ActiveF; i++)
       
        for (int j = 0; j < 4; j++)
            for (int g = 0; g < 4; g++)
            {
               
                if (mas[i][g].active == -1)
                {
                    continue;
                }
                else
                {
                    if (mas[ActiveF][j].y + y + Height / 2 >= NIZ)               //Перевірка низу екрана
                        return 0;
                    if (mas[ActiveF][j].y + y + Height == mas[i][g].y && mas[ActiveF][j].x + x == mas[i][g].x &&  // Перевірка чи немає знизу блока
                        ActiveF > 0)
                        return 0;
                }
            }
    return 1;
}
int CheckX(Kord** mas, int x,int y,char Side) {              //Перевірка чи немає зліва та справа блоків. 
    for (int i = 0; i < ActiveF; i++)
        for (int j = 0; j < 4; j++)
        {
            if ((x - Width == mas[i][j].x && mas[i][j].y+Height>y && mas[i][j].y - Height<y &&mas[i][j].active!=-1 && Side=='L') ||
                (x + Width == mas[i][j].x && mas[i][j].y + Height > y && mas[i][j].y - Height < y && mas[i][j].active != -1 && Side=='R'))
                return 1;
            
        }
    return 0;
}

int CheckLeft(Kord** mas, int x,int y)   //Перевірка чи немає зліва блоків, а також чи не досяг блок межі екрану
{
    char Side = 'L';
    for (int i = 0; i < 4; i++)
        if (mas[ActiveF][i].x + x-Width/2 <= 0 || CheckX(mas,mas[ActiveF][i].x + x, mas[ActiveF][i].y + y, Side)==1) ////Викликаємо функцію CheckX
            return 1;
    return 0;
}
int CheckRight(Kord** mas, int x, int y)   //Перевірка чи немає зліва справа блоків, а також чи не досяг блок межі екрану
{
    char Side = 'R';
    for (int i = 0; i < 4; i++)
        if (mas[ActiveF][i].x + x + Width / 2 >= BOK || CheckX(mas, mas[ActiveF][i].x + x, mas[ActiveF][i].y + y,Side) == 1)
            return 1;
    return 0;
}
void DrawKvadrat(int x, int y, Sprite *sprite, Kord **mas) {//Виводим на екран всі готові блоки з масиву
  
    for(int i=0;i<=ActiveF;i++)
    
    for (int j = 0; j < 4; j++)
    { 
        if (mas[i][j].active==1)
            sprite->setPosition(x + mas[i][j].x, y + mas[i][j].y);///Задання позиції активного блока
        else  if (mas[i][j].active != -1)
            sprite->setPosition(mas[i][j].x,mas[i][j].y);///Задання позиції решти блоків
        window.draw(*sprite);//Малюємо блок!
    }
}
int CheckRotate(Kord** mas, int x, int y) //Перевірка можливості повороту блоку
{
    if (x<Width/2 || x>BOK-Width/2 || y > NIZ-Height/2)
        return 1;
    else 
    for (int i = 0; i < ActiveF; i++)
                for (int j = 0; j < 4; j++)
                 {
                    if ((y > mas[i][j].y - Height  && y < mas[i][j].y + Height && x < mas[i][j].x+Width && x>mas[i][j].x-Width) )
                        return 1;

                   }
    return 0;
}
int Rotate(Kord** mas,int x,int y) {  //Функція повороту блоку
    for (int i = 0; i < 4; i++)
    {
       
            if (CheckRotate(mas, x + mas[ActiveF][i].y, y - mas[ActiveF][i].x))
                return 0;
    }
            int ch;
            ch = -mas[ActiveF][0].x;
            mas[ActiveF][0].x = mas[ActiveF][0].y;
            mas[ActiveF][0].y = ch;
            ch = -mas[ActiveF][2].x;
            mas[ActiveF][2].x = mas[ActiveF][2].y;
            mas[ActiveF][2].y = ch;
            ch = -mas[ActiveF][3].x;
            mas[ActiveF][3].x = mas[ActiveF][3].y;
            mas[ActiveF][3].y = ch;
  
}
void Generator(Kord** mas, int j, int N) { //Генератор випадкових фігур
    switch (N) {
         case 0:
            mas[j][0].x = -Height;
            mas[j][0].y = 0;
            mas[j][1].x = 0;    /////ОСЬОВИЙ БЛОК
            mas[j][1].y = 0;
            mas[j][2].x = 0;
            mas[j][2].y = Height;
            mas[j][3].x = 0;
            mas[j][3].y = -Height;
            mas[j][0].active = 0;
            mas[j][1].active = 0;
            mas[j][2].active = 0;
            mas[j][3].active = 0;
            break;
         case 1:
             mas[j][0].x = -Height;
             mas[j][0].y = 0;
             mas[j][1].x = 0;
             mas[j][1].y = 0;
             mas[j][2].x = Height;
             mas[j][2].y = 0;
             mas[j][3].x = Height;
             mas[j][3].y = -Height;
             mas[j][0].active = 0;
             mas[j][1].active = 0;
             mas[j][2].active = 0;
             mas[j][3].active = 0;
             break;
         case 2:
             mas[j][0].x = -Height;
             mas[j][0].y = 0;
             mas[j][1].x = 0;
             mas[j][1].y = 0;
             mas[j][2].x = Height;
             mas[j][2].y = 0;
             mas[j][3].x = 2*Height;
             mas[j][3].y = 0;
             mas[j][0].active = 0;
             mas[j][1].active = 0;
             mas[j][2].active = 0;
             mas[j][3].active = 0;
             break;
         case 3:
             mas[j][0].x = -Height;
             mas[j][0].y = 0;
             mas[j][1].x = 0;
             mas[j][1].y = 0;
             mas[j][2].x = 0;
             mas[j][2].y = Height;
             mas[j][3].x = Height;
             mas[j][3].y = Height;
             mas[j][0].active = 0;
             mas[j][1].active = 0;
             mas[j][2].active = 0;
             mas[j][3].active = 0;
             break;
         case 4:
             mas[j][0].x = -Height;
             mas[j][0].y = 0;
             mas[j][1].x = 0;
             mas[j][1].y = 0;
             mas[j][2].x = Height;
             mas[j][2].y = 0;
             mas[j][3].x = Height;
             mas[j][3].y = Height;
             mas[j][0].active = 0;
             mas[j][1].active = 0;
             mas[j][2].active = 0;
             mas[j][3].active = 0;
             break;
    }
}
int StartScreen() { //Функція виводу стартового екрану
    Event event;
    Text text;
    Font font;
    font.loadFromFile("PixdorTwo.ttf");
    text.setFont(font);
    text.setStyle(Text::Bold);
    text.setCharacterSize(15);
    text.setFillColor(Color::Black);
    Texture Zastavka, TETRIS;
    Zastavka.loadFromFile("Zast.png");
    TETRIS.loadFromFile("TETRISS.png");
    Sprite TETRIS1(TETRIS, IntRect(0, 0, 320, 104));
    Sprite Zastavka1(Zastavka, IntRect(0, 0, 320, 530));
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }
        window.clear();
        if (event.type == Event::MouseButtonPressed)
            break;
        window.draw(Zastavka1);
        window.draw(TETRIS1);
        text.setString(L"Up - Rotate");
        text.setPosition(30, 430);
        window.draw(text);
        text.setString(L"Left,Right - Move");
        text.setPosition(30, 450);
        window.draw(text);
        text.setString(L"Down - Fast drop");
        text.setPosition(30, 470);
        window.draw(text);
        text.setString(L"LControl - Pause");
        text.setPosition(30, 490);
        window.draw(text);
        text.setString(L"Press mouse to continue!");
        text.setPosition(70, 510);
        window.draw(text);
        window.display();
    }
    return 0;
}
int main()
{
    StartScreen();
    
    SoundBuffer Buffer,Buffer2,Buffer3;
    Buffer.loadFromFile("SoundBox.ogg");// Загрузка звуків в буфер
    Buffer2.loadFromFile("SoundGO.ogg");
    Buffer3.loadFromFile("SoundDelete.ogg");
    Sound SoundBox,SoundGO,SoundDelete; //Створення і привязка звука до буфера
    SoundBox.setBuffer(Buffer);
    SoundGO.setBuffer(Buffer2);
    SoundDelete.setBuffer(Buffer3);
    Text text;
    Font font;
    font.loadFromFile("PixdorTwo.ttf");
    text.setFont(font);
    text.setStyle(Text::Bold);
    text.setCharacterSize(30);
    text.setFillColor(Color::White);
    srand(time(NULL));
    Texture GameOver,Panel;
    Vector2f barsize(Width, Height);
    bool pressed = true;
    window.setKeyRepeatEnabled(false);
    Texture textureKvadr;
    textureKvadr.loadFromFile("Kvadr.png");
    Panel.loadFromFile("Panel.png");
    Sprite sprite(textureKvadr, IntRect(0, 0, Width, Height));
    Sprite GameOver1(GameOver, IntRect(0, 0, 200, 200));
    Sprite Panel1(Panel, IntRect(0, 0, 320, 50));
    Texture Background;
    Background.loadFromFile("WBVk.png");
    Sprite Back(Background, IntRect(0, 0, 320, 480));
    sprite.setScale(barsize.x / sprite.getLocalBounds().width, barsize.x / sprite.getLocalBounds().height);
    sprite.setOrigin(Width / 2, Width / 2);
    Texture SoundOff;
    SoundOff.loadFromFile("SoundOn.png");
    Sprite SoundOff1(SoundOff, IntRect(0, 0, 15, 13));
    SoundOff1.setPosition(5, 5);
    int i;
    float delay,Speed;
    float timer,timer2;
    Clock clock;
    int Position;
    Event event;
    window.setFramerateLimit(350);
    Panel1.setPosition(0, 480);
    SoundGO.setVolume(30);
    bool SoundFlag = true;
    bool MousePressed;
Restart: /// Точка повернення після натискання клавіші Q на фінальному екрані!!!
    Score = 0;
    window.clear();
    Speed = 0;
    MousePressed = true;
    pressed = true;
    ActiveF = 0;
    Position = BOK / 2-Width/2;
    Kord **mas = new  Kord*[10000];// Динамічний масив для зберігання блоків (кожен блок займає 4 квадрати)
    for (int j = 0; j < 10000; j++)
        mas[j] = new Kord[4];
    for (int j = 0; j < 10000; j++)
    {
        Generator(mas, j, rand() % 5);// Генеруємо зразу всі блоки в масиві
        mas[j][0].active = 0;
        mas[j][1].active = 0;
        mas[j][2].active = 0;
        mas[j][3].active = 0;
        
    }
    mas[0][0].active = 1;
    mas[0][1].active = 1;
    mas[0][2].active = 1;
    mas[0][3].active = 1;
    i = Height/2;
    delay = 0.1;
    timer = 0;
    timer2 = 0;
    
    while (window.isOpen())
    {   
        float time;
        time = clock.getElapsedTime().asSeconds();//Генератор затримки падіння блоків
        clock.restart();
        timer += time;
     
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        
        if(event.type == Event::KeyPressed && event.key.code == Keyboard::Up)
        timer2 += time/2;
        else  timer2 += time;
            if (event.type == Event::KeyReleased || timer2 > 0.1)
            {
                timer2 = 0;
                pressed = true;// Булевий флажок для однократного натискання клавіші. Без нього клавіша зажимається
            }
            if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left && MousePressed)//Вмикання та вимикання звука
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (position.x <= 20 && position.x >= 5 && position.y <= 18 && position.y >= 5)
                    if (SoundFlag) {
                        SoundBox.setVolume(0);
                        SoundGO.setVolume(0);
                        SoundDelete.setVolume(0);
                        SoundFlag = false;
                        SoundOff.loadFromFile("SoundOff.png");
                        MousePressed = false;
                    }
                    else {
                        SoundBox.setVolume(100);
                        SoundGO.setVolume(30);
                        SoundDelete.setVolume(100);
                        SoundFlag = true;
                        SoundOff.loadFromFile("SoundOn.png");
                        MousePressed = false;
                    }

            }
            if (event.type == Event::MouseButtonReleased)
                MousePressed = true;
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left) //Рух вліво
        if(CheckLeft(mas,Position,i)!=1 && pressed && CheckY(mas,Position, i))
        { 
            Position -= Width; pressed = false; continue;
        }
       
       if (event.type == Event::KeyPressed && event.key.code == Keyboard::Up) //Переворот
            if ( pressed && CheckY(mas, Position, i))
            {
                Rotate(mas,Position, i);
                 pressed = false;
                
            }
            
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right) //Рух вправо
            
            if (CheckRight(mas, Position,i)!=1 && pressed && CheckY(mas, Position, i))
                { Position += Width; pressed = false; continue;
            }
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::LControl)//Пауза
        {
            text.setFillColor(Color::Red);
            text.setCharacterSize(50);
            text.setString(L"P A U S E");
              text.setPosition(40, 220);
             window.draw(text);
             text.setCharacterSize(20);
             text.setFillColor(Color::Yellow);
             text.setString(L"press any key to continue");
             text.setPosition(40, 280);
             window.draw(text);
             window.display();
             while (!(event.type == Event::KeyPressed && event.key.code != Keyboard::LControl))
             {
                 if (event.type == Event::Closed)
                 {
                     window.close();
                     break;
                 }
             window.waitEvent(event);
             }
        }
        window.clear();
        window.draw(Back);
        window.draw(SoundOff1);
        window.draw(Panel1);
        text.setFillColor(Color::Black);
        text.setCharacterSize(13);
        text.setString(L"Your current score is:");
        text.setPosition(30, 490);
        window.draw(text);
        text.setString(std::to_string(Score));
        text.setPosition(220, 490);
        window.draw(text);
        DrawKvadrat(Position, i,&sprite, mas);      ////ВИВОДИМО БЛОКИ З МАСИВУ НА ЕКРАН
        window.display();
        if (timer > delay)
        {

            
            if(CheckY(mas, Position, i))
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Down)
                delay=0.001-Speed/100;
            else 
                delay=0.1-Speed;
            timer = 0;
            i += 2;//Рух падаючого блоку!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            sprite.setPosition(Position, i);
        }
       
        if (CheckY(mas, Position, i) == 0)
        {
           
            for (int j = 0; j < 4; j++)
            {
                mas[ActiveF][j].x += Position; ///Фіксуємо позицію блока та переходимо до слідуючго
                mas[ActiveF][j].y += i;
                mas[ActiveF][j].active = 0;
                
            }
            if (CheckLine(mas))
                SoundDelete.play();
            else SoundBox.play();
            for (int j = 0; j < 4; j++)
            {

                if (mas[ActiveF][j].y <= Height / 2 && ActiveF != 0 || ActiveF==9999)
                {
                    window.clear();
                    SoundGO.play();
                    GameOver.loadFromFile("GO.png");
                    GameOver1.setPosition(70, 100);
                    text.setFillColor(Color::White);
                    text.setCharacterSize(13);
                    text.setString(L"Your score is:");
                    text.setPosition(70, 250);
                    window.draw(GameOver1);//Виводимо фінальний екран
                    window.draw(text);
                    text.setString(std::to_string(Score));
                    text.setCharacterSize(25);
                    text.setPosition(70, 300);
                    window.draw(text);
                    text.setCharacterSize(15);
                    text.setPosition(70, 450);
                    text.setString("Press Q to Quit");
                    window.draw(text);
                    text.setPosition(70, 480);
                    text.setString("Press Space to Restart");
                    window.draw(text);
                    window.display();
                    for (;;) {
                        window.waitEvent(event);

                        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
                        {
                            SoundGO.stop();
                            for (int j = 0; j < 10000; j++)
                               delete[]mas[j];
                            goto Restart;
                        }
                        else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q)
                        {
                      
                            for (int j = 0; j < 10000; j++)
                                delete[]mas[j];
                            return 0;
                        }
                    }
                }       
            }
            i = 0;
            Position = BOK / 2 - Width / 2;
            if (ActiveF % 5 == 0 && Speed < 0.08)
                Speed = Speed + 0.005;
            else if (ActiveF % 5 == 0 && Speed > 0.08)
                Speed = Speed + 0.001;
            ActiveF++;
            mas[ActiveF][0].active = 1;
            mas[ActiveF][1].active = 1;
            mas[ActiveF][2].active = 1;
            mas[ActiveF][3].active = 1;
        }
      
    }
    for (int j = 0; j <10000; j++)
        delete[]mas[j];
    return 0;
}