#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include"classes.h"
#include<string>

using namespace std;
int main()
{
    // create the window
   /* sf::ContextSettings settings;
    settings.antialiasingLevel = 8;*/

    sf::RenderWindow window(sf::VideoMode(1300, 700), "Tetris");


    // run the program as long as the window is open
    game tetris;

    //Initilization
    tetramino* types[7];
    I_block a;
    types[0] = &a;

    J_block b;
    types[1] = &b;

    L_block c;
    types[2] = &c;

    O_block d;
    types[3] = &d;

    S_block e;
    types[4] = &e;

    T_block f;
    types[5] = &f;

    Z_block g;
    types[6] = &g;


    bool start = false;
    bool new_highscore = false;
    int block;
    float fall_time = 1000;
    // Creating a clock
    sf::Clock clock;



    string playerInput;
    sf::Text playerText;
    sf::Font font;
    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
    {
        // error...
        cout << "Unable to load the required font!\n";
    }
    bool backspace_pressed = false;


    while (window.isOpen())
    {
        window.clear(sf::Color::Black);


        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            // draw everything here...
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && tetris.well_filled() == false && start == false) {
                if (playerInput.size() > 0) {

                    backspace_pressed = true;
                   // cout << "called\n";

                }



            }
            else if (event.type == sf::Event::TextEntered && tetris.well_filled() == false && start == false)
            {
                if (playerInput.size() < 10) {// MAX 10 character length
                    tetris.input_name(window, playerInput, playerText, event, font);
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && tetris.well_filled() == false && start == false) {
                srand(time(NULL));
                block = rand() % 7;
                start = true;
                cout << "Enter\n";



            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && start == true) {
                tetris.right(window, types, block);

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && start == true) {
                tetris.left(window, types, block);

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && start == true) {// rotate while sped up
                tetris.rotate(window, types, block);
                fall_time = 50;

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && start == true) {
                fall_time = 50;

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && start == true) {
                tetris.rotate(window, types, block);

            }

        }

        if (backspace_pressed) {
            playerInput.pop_back();
            playerText.setString(playerInput);
            backspace_pressed = false;
        }

        if (tetris.well_filled()) {
            start = false;
        }


        if (start == true) {

            // Draw well
            tetris.set_up(window, types);

            tetris.generate_tetramino(window, types, block);
            tetris.display_info(window, playerInput);
            //tetris.hide(window);

            sf::Time elapsed1 = clock.getElapsedTime();
            float milliseconds_passed = elapsed1.asMilliseconds();


            if ((milliseconds_passed) > fall_time) {
                bool collision = tetris.collision(window, types, block);
                if (collision == true) {
                    tetris.check_lines();
                    tetris.check_level();
                    srand(time(NULL));
                    block = rand() % 7;

                }
                else {

                }
                clock.restart();
                // reset fall_time
                fall_time = 1000;
                for (int i = 1; i < tetris.get_level(); i++) {
                    fall_time = fall_time * 0.9;
                }
               // cout << fall_time << endl;


            }


        }
        //after game ends
        else if (start == false && tetris.well_filled() == true) {
            tetris.manage_leaderboard(playerInput, new_highscore);
            tetris.gameover(window);

        }

        //before game begins
        else if (start == false) {
            window.draw(playerText);
            tetris.starting_text(window);
        }


        window.display();
    }

    return 0;
}