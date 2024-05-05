#pragma once
#include <iostream>
#include <map>
#include <chrono>
#include "pcsq.h"
#include "dataStructures.h"
#include "logic.h"
#include "main_menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;
bool firstenter = 1;
float window_w = 850, window_h = 800;
RenderWindow window(VideoMode(window_w, window_h), "Chess"); // Create SFML window
const int SQUARE_SIZE = 100;
const int BOARD_SIZE = 8;

bool reset = 0;

Vector2f calculateSquarePosition(int& row, int& col)
{
    return Vector2f(col * SQUARE_SIZE, row * SQUARE_SIZE);
}
struct buttons {
    Texture sound_txt, main_txt, restart_txt;
    Sprite  sound_button, main_button, restart_button;
    void initialize() {

        sound_txt.loadFromFile("button_UI.png");
        main_txt.loadFromFile("button_UI.png");
        restart_txt.loadFromFile("button_UI.png");

        sound_button.setTexture(sound_txt);
        main_button.setTexture(main_txt);
        restart_button.setTexture(restart_txt);

        sound_button.setScale(2.9, 2.9);
        main_button.setScale(2.9, 2.9);
        restart_button.setScale(2.9, 2.9);

        main_button.setTextureRect(IntRect(10 * 16, 9 * 16, 16, 16));
        restart_button.setTextureRect(IntRect(9 * 16, 9 * 16, 16, 16));
        sound_button.setTextureRect(IntRect(6 * 16, 10 * 16, 16, 16));


        sound_button.setPosition(window_w - 50, 100);
        main_button.setPosition(window_w - 50, 300);
        restart_button.setPosition(window_w - 50, 200);


    }
    void draw() {
        window.draw(main_button);
        window.draw(restart_button);
        window.draw(sound_button);
    }
    void MouseClickButtons(MainMenu& mainMenu) {
        auto mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
        if (mainMenu.settingsPage.getSoundState())
            sound_button.setTextureRect(IntRect(6 * 16, 10 * 16, 16, 16));
        else
            sound_button.setTextureRect(IntRect(7 * 16, 10 * 16, 16, 16));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (main_button.getGlobalBounds().contains(mousePosition)) {
                mainMenu.loadedMenu = 0;
                reset = 1;
            }
            if (restart_button.getGlobalBounds().contains(mousePosition)) {
                reset = 1;
            }
            if (sound_button.getGlobalBounds().contains(mousePosition)) {
                //toogle on the sound
                mainMenu.settingsPage.setSoundState(!mainMenu.settingsPage.getSoundState());
                if (mainMenu.settingsPage.getSoundState())
                    sound_button.setTextureRect(IntRect(6 * 16, 10 * 16, 16, 16));
                else
                    sound_button.setTextureRect(IntRect(7 * 16, 10 * 16, 16, 16));
            }
        }
        else {
            if (main_button.getGlobalBounds().contains(mousePosition)) {
                main_button.setScale(3, 3);
            }
            else {
                main_button.setScale(2.9, 2.9);
            }
            if (restart_button.getGlobalBounds().contains(mousePosition)) {
                restart_button.setScale(3, 3);
            }
            else {
                restart_button.setScale(2.9, 2.9);
            }
            if (sound_button.getGlobalBounds().contains(mousePosition)) {
                sound_button.setScale(3, 3);
            }
            else {
                sound_button.setScale(2.9, 2.9);
            }
        }
    }
};
struct theme {

};
////////////////////////////////////////////////////////////////////////////////////////////
struct soundss {
    SoundBuffer moveS, moveCh, prom, gameSt, gameEnd, captur, castl, tenS, illeg;
    Sound move, check, promotion, gamestart, gameend, capture, castle, tenseconds, illegal;
    void load_sounds() {
        if (!moveS.loadFromFile("wood/move-self.mp3") ||
            !moveCh.loadFromFile("wood/move-check.mp3") ||
            !prom.loadFromFile("wood/promote.mp3") ||
            !tenS.loadFromFile("wood/tenseconds.wav") ||
            !illeg.loadFromFile("wood/illegal.wav") ||
            !gameSt.loadFromFile("wood/game-start.wav") ||
            !gameEnd.loadFromFile("wood/game-end.wav") ||
            !captur.loadFromFile("wood/capture.mp3") ||
            !castl.loadFromFile("wood/castle.mp3"))
        {
            cout << "ERROR";
        }
        move.setBuffer(moveS);
        check.setBuffer(moveCh);
        promotion.setBuffer(prom);
        gamestart.setBuffer(gameSt);
        gameend.setBuffer(gameEnd);
        capture.setBuffer(captur);
        castle.setBuffer(castl);
        tenseconds.setBuffer(tenS);
        illegal.setBuffer(illeg);
    }
};
struct End {
    Texture background_txt, main_txt, restart_txt;
    Sprite  background, main_button, restart_button;
    Text sentence;
    string win, win1, win2, lose, draw;
    Font font;
    void initialize() {

        background_txt.loadFromFile("assets/background.png");
        main_txt.loadFromFile("button_UI.png");
        restart_txt.loadFromFile("button_UI.png");

        background.setOrigin(background_txt.getSize().x / 2, background_txt.getSize().y / 2);
        main_button.setOrigin(main_txt.getSize().x / 2, main_txt.getSize().y / 2);
        restart_button.setOrigin(restart_txt.getSize().x / 2, restart_txt.getSize().y / 2);

        background.setTexture(background_txt);
        main_button.setTexture(main_txt);
        restart_button.setTexture(restart_txt);

        background.setScale(0.3, 0.3);
        main_button.setScale(2.9, 2.9);
        restart_button.setScale(2.9, 2.9);

        main_button.setTextureRect(IntRect(10 * 16, 9 * 16, 16, 16));
        restart_button.setTextureRect(IntRect(9 * 16, 9 * 16, 16, 16));
        background.setTextureRect(IntRect(0, 500, 2000, 1500));


        background.setPosition(window_w / 2, window_h / 2);
        main_button.setPosition(window_w * 1.04, window_h / 2);
        restart_button.setPosition(window_w, window_h / 2);
        sentence.setPosition(window_w * 0.47, window_h * 0.4);

        font.loadFromFile("font.ttf");
        sentence.setFont(font);
        sentence.setCharacterSize(25);

        win1 = "White is victorious !";
        win2 = "Black is victorious !";
        win = "YOU WON !";
        lose = "you lost";
        draw = "Stalemate";
    }
    void change(int num, int mode, soundss sounds, bool first_end) {
        if (first_end) {
            sounds.gameend.setVolume(50);
            sounds.gameend.play();
        }
        if (mode == 0) {
            sentence.setString(draw);
        }
        else if (mode == 1) {
            if (num == 1) {
                sentence.setString(win);
            }
            else if (num == 2) {
                sentence.setString(lose);
            }
        }
        else {
            if (num == 1) {
                sentence.setString(win1);
            }
            else if (num == 2) {
                sentence.setString(win2);
            }
        }
        window.draw(background);
        window.draw(main_button);
        window.draw(restart_button);
        window.draw(sentence);
    }
    void MouseClickButtons(MainMenu& mainMenu) {
        auto mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (main_button.getGlobalBounds().contains(mousePosition)) {
                mainMenu.loadedMenu = 0;
                reset = 1;
            }
            if (restart_button.getGlobalBounds().contains(mousePosition)) {
                reset = 1;
            }
            if (sentence.getGlobalBounds().contains(mousePosition)) {
                //play sound
            }
        }
        else {
            if (main_button.getGlobalBounds().contains(mousePosition)) {
                main_button.setScale(3, 3);
            }
            else {
                main_button.setScale(2.9, 2.9);
            }
            if (restart_button.getGlobalBounds().contains(mousePosition)) {
                restart_button.setScale(3, 3);
            }
            else {
                restart_button.setScale(2.9, 2.9);
            }
            if (sentence.getGlobalBounds().contains(mousePosition)) {
                sentence.setScale(1.2, 1.2);
            }
            else {
                sentence.setScale(1, 1);
            }
        }
    }
};

struct piecess {
    Texture pawnBtxt, knightBtxt, rookBtxt, kingBtxt, queenBtxt, bishopBtxt;
    Texture pawnWtxt, knightWtxt, rookWtxt, kingWtxt, queenWtxt, bishopWtxt;
    Texture validDottxt;
    Texture bgk;
    Sprite bgkk;
    Sprite board;
    Texture boardTexture; // Texture for the chessboard
    void load_pieces(MainMenu& mainMenu) {
        validDottxt.loadFromFile("square_light.png");

        if (mainMenu.settingsPage.getColor() == 3) {
            mainMenu.settingsPage.setColor(1);
            if (!pawnBtxt.loadFromFile("newpieces/blackpawn.png") ||
                !knightBtxt.loadFromFile("newpieces/blackknight.png") ||
                !rookBtxt.loadFromFile("newpieces/blackrook.png") ||
                !kingBtxt.loadFromFile("newpieces/blackking.png") ||
                !queenBtxt.loadFromFile("newpieces/blackqueen.png") ||
                !bishopBtxt.loadFromFile("newpieces/blackbishop.png") ||
                !pawnWtxt.loadFromFile("newpieces/whitepawn.png") ||
                !knightWtxt.loadFromFile("newpieces/whiteknight.png") ||
                !rookWtxt.loadFromFile("newpieces/whiterook.png") ||
                !kingWtxt.loadFromFile("newpieces/whiteking.png") ||
                !queenWtxt.loadFromFile("newpieces/whitequeen.png") ||
                !bishopWtxt.loadFromFile("newpieces/whitebishop.png"))
            {
                cout << "Failed to load piece textures\n";
            }
        }
        else if (mainMenu.settingsPage.getColor() == 4) {
            mainMenu.settingsPage.setColor(2);
            if (!pawnBtxt.loadFromFile("newpieces/whitepawn.png") ||
                !knightBtxt.loadFromFile("newpieces/whiteknight.png") ||
                !rookBtxt.loadFromFile("newpieces/whiterook.png") ||
                !kingBtxt.loadFromFile("newpieces/whiteking.png") ||
                !queenBtxt.loadFromFile("newpieces/whitequeen.png") ||
                !bishopBtxt.loadFromFile("newpieces/whitebishop.png") ||
                !pawnWtxt.loadFromFile("newpieces/blackpawn.png") ||
                !knightWtxt.loadFromFile("newpieces/blackknight.png") ||
                !rookWtxt.loadFromFile("newpieces/blackrook.png") ||
                !kingWtxt.loadFromFile("newpieces/blackking.png") ||
                !queenWtxt.loadFromFile("newpieces/blackqueen.png") ||
                !bishopWtxt.loadFromFile("newpieces/blackbishop.png"))
            {
                cout << "Failed to load piece textures\n";
            }
        }
        bgk.loadFromFile("brown.png");
        bgkk.setTexture(bgk);
        bgkk.setScale(window.getSize().x / bgkk.getLocalBounds().width, window.getSize().y / bgkk.getLocalBounds().height);

        boardTexture.loadFromFile("newboard.jpg"); // Load chessboard texture
        board.setTexture(boardTexture); // Sprite for the chessboard
        board.setScale(0.74f, 0.74f); // Scale the chessboard to fit the window

    }
    void draw_pieces(GameState& current_state, bool validPoints[][8]) {
        window.draw(bgkk);
        window.draw(board);

        // Draw pieces
        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            for (int col = 0; col < BOARD_SIZE; ++col)
            {
                int pieceType = current_state.board[row][col];

                Sprite piece;
                switch (pieceType)
                {
                case 6: piece.setTexture(pawnWtxt); break;
                case 4: piece.setTexture(knightWtxt); break;
                case 5: piece.setTexture(bishopWtxt); break;
                case 3: piece.setTexture(rookWtxt); break;
                case 2: piece.setTexture(queenWtxt); break;
                case 1: piece.setTexture(kingWtxt); break;
                case -6: piece.setTexture(pawnBtxt); break;
                case -4: piece.setTexture(knightBtxt); break;
                case -5: piece.setTexture(bishopBtxt); break;
                case -3: piece.setTexture(rookBtxt); break;
                case -2: piece.setTexture(queenBtxt); break;
                case -1: piece.setTexture(kingBtxt); break;
                default: break;
                }
                float scale = SQUARE_SIZE / max(piece.getLocalBounds().width, piece.getLocalBounds().height);
                piece.setScale(scale, scale);
                piece.setPosition(calculateSquarePosition(row, col));
                window.draw(piece);
            }
        }

        // Draw valid points
        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            for (int col = 0; col < BOARD_SIZE; ++col)
            {
                if (validPoints[row][col])
                {
                    Sprite validDot;
                    validDot.setTexture(validDottxt);
                    float scale = SQUARE_SIZE / max(validDot.getLocalBounds().width, validDot.getLocalBounds().height);
                    validDot.setScale(scale, scale);
                    validDot.setPosition(calculateSquarePosition(row, col));
                    window.draw(validDot);
                }
            }
        }
    }

};
void reset_validpoints(bool validPoints[][8]) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            validPoints[i][j] = false;
}
void valid_moves(GameState& current_state, bool validPoints[][8], int& lastPieceX, int& lastPieceY) {
    reset_validpoints(validPoints);

    if (current_state.player == 1)
    {
        myPair<int, int>currentmouse = { lastPieceX,lastPieceY };
        for (int i = 0; i < current_state.white_possible_moves.size(); i++)
        {
            char a = current_state.white_possible_moves[i][0];
            char b = current_state.white_possible_moves[i][1];
            char c = current_state.white_possible_moves[i][2];
            char d = current_state.white_possible_moves[i][3];
            myPair<int, int>validsquare = to_index(c, d);
            if (currentmouse == to_index(a, b) &&
                current_state.check_legal(lastPieceX, lastPieceY, validsquare.first, validsquare.second, current_state.player))
                validPoints[validsquare.first][validsquare.second] = 1;
        }
    }
    else
    {

        myPair<int, int>currentmouse = { lastPieceX,lastPieceY };
        for (int i = 0; i < current_state.black_possible_moves.size(); i++)
        {
            char a = current_state.black_possible_moves[i][0];
            char b = current_state.black_possible_moves[i][1];
            char c = current_state.black_possible_moves[i][2];
            char d = current_state.black_possible_moves[i][3];
            myPair<int, int>validsquare = to_index(c, d);
            if (currentmouse == to_index(a, b) &&
                current_state.check_legal(lastPieceX, lastPieceY, validsquare.first, validsquare.second, current_state.player))
                validPoints[validsquare.first][validsquare.second] = 1;
        }
    }
}
void applying_sounds(GameState& current_state, soundss& sounds, int& mouseX, int& mouseY, int& lastPieceX, int& lastPieceY, bool sound_state) {
    GameState new_state = current_state.simulate_move(lastPieceX, lastPieceY, mouseX, mouseY);
    if (sound_state) {
        if (new_state.checked(new_state.white_king.first, new_state.white_king.second, 1) ||
            new_state.checked(new_state.black_king.first, new_state.black_king.second, -1))
            sounds.check.play();
        else if (current_state.board[lastPieceX][lastPieceY] == 6 && mouseX == 0 ||
            current_state.board[lastPieceX][lastPieceY] == -6 && mouseX == 7)
            sounds.promotion.play();
        else if (current_state.board[mouseX][mouseY])
            sounds.capture.play();
        else if (current_state.board[mouseX][mouseY] == 0)
            sounds.move.play();
    }
}
void move_generation(GameState& current_state) {
    if (current_state.white_possible_moves.empty())
        current_state.generate_all_possible_moves(1);
    if (current_state.black_possible_moves.empty())
        current_state.generate_all_possible_moves(-1);
}

int main()
{
    ///////////////////////////////////////////******************************
    bool first_end = 1;
    int end_game = -10, end_game_ai = -10;
    GameState current_state;
    Minimax AI;
    //MainMenu mainMenu(window.getSize());
    MainMenu mainMenu(window.getSize());
    //pieces
    piecess pieces;

    //sounds
    soundss sounds;
    sounds.load_sounds();
    int current_color = 3;

    // used for timing functions for debugging.
    chrono::steady_clock::time_point start_time, current_time;
    chrono::milliseconds duration;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //END GAME
    End end;
    end.initialize();
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //buttons
    buttons button;
    button.initialize();

    // Load textures for chess pieces

    // Array to hold piece sprites
    bool validPoints[8][8] = {}, mousePressed = false; // Array to hold valid points
    int lastPieceX, lastPieceY; // Variables to store last selected piece position
    int mouseY = 0, mouseX = 0; // Variables to store last position the mouse was clicked
    current_state.initialize_board();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ////////////////////////////////////////////////////////////////////////////////
            if (event.type == sf::Event::Closed)
                window.close();
            //////////////////////////////////////////////////////////////////////////////////////////////********************
            if (end_game != -10 || end_game_ai != -10)
                end.MouseClickButtons(mainMenu);

            if (Keyboard::isKeyPressed(Keyboard::Z))end_game = 1;
            if (Keyboard::isKeyPressed(Keyboard::R) || reset) {
                reset_validpoints(validPoints);
                current_state.initialize_board();
                current_state.player = 1;
                reset = 0;
                end_game = -10;
                end_game_ai = -10;
            }
            //////////////////////////////////////////////////////////////////////////////////////////*************************************
            if (Mouse::isButtonPressed(Mouse::Left)) {
                mouseY = Mouse::getPosition(window).x / 100;
                mouseX = Mouse::getPosition(window).y / 100;
                if (in_board(mouseX, mouseY))
                    mousePressed = true;

            }
            else
                mousePressed = false;
            auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (mainMenu.loadedMenu == 3) {
                mainMenu.settingsPage.SettingsMouseClicks(mouse, window.getSize(), mainMenu.loadedMenu);

            }
            if (mainMenu.loadedMenu == 1 || mainMenu.loadedMenu == 2)
                button.MouseClickButtons(mainMenu);
        }
        if (mainMenu.settingsPage.getColor() == 3 || mainMenu.settingsPage.getColor() == 4)
            pieces.load_pieces(mainMenu);

        /////////////////////////////////////////////////////////////////////////////
        if (mainMenu.loadedMenu == 0) {
            auto mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
            mainMenu.MenuButtonsAnimation(window);
            mainMenu.MainMenuMouseClick(mousePosition, window);
            ////////////////////////////////////////////////////////////////////////////////////////////////////////*********
            reset = 0;
            //////////////////////////////////////////////////////////////////////////////////////////////////////
        }

        //menu 2 2players
        if (mainMenu.loadedMenu == 2) {
            move_generation(current_state);
            if (mousePressed) {
                if (current_state.board[mouseX][mouseY] * current_state.player > 0)
                {
                    lastPieceX = mouseX;
                    lastPieceY = mouseY;
                    valid_moves(current_state, validPoints, lastPieceX, lastPieceY);
                }
                else if (validPoints[mouseX][mouseY])
                {
                    applying_sounds(current_state, sounds, mouseX, mouseY, lastPieceX, lastPieceY, mainMenu.settingsPage.getSoundState());
                    current_state = current_state.simulate_move(lastPieceX, lastPieceY, mouseX, mouseY);
                    reset_validpoints(validPoints);
                    move_generation(current_state);
                    if (current_state.checkmate(1))
                        end_game = -1; //black won
                    else if (current_state.checkmate(-1))
                        end_game = 1; //white won
                    else if (current_state.stalemate(1) || current_state.stalemate(-1)) {
                        end_game = 2;//draw
                    }

                }
            }

        }
        //menu 1 ai
        else if (mainMenu.loadedMenu == 1)
        {
            move_generation(current_state);
            if (current_state.player == 1)
            {
                if (current_state.board[mouseX][mouseY] > 0 && current_state.player == 1)
                {
                    lastPieceX = mouseX;
                    lastPieceY = mouseY;

                    valid_moves(current_state, validPoints, lastPieceX, lastPieceY);
                }
                else if (validPoints[mouseX][mouseY])
                {
                    applying_sounds(current_state, sounds, mouseX, mouseY, lastPieceX, lastPieceY, mainMenu.settingsPage.getSoundState());
                    current_state = current_state.simulate_move(lastPieceX, lastPieceY, mouseX, mouseY);
                    reset_validpoints(validPoints);

                }
            }
            else
            {

                AI.iterative_deepening(current_state);
                string best = AI.best_move;
                myPair<int, int>from = to_index(best[0], best[1]), to = to_index(best[2], best[3]);
                applying_sounds(current_state, sounds, to.first, to.second, from.first, from.second, mainMenu.settingsPage.getSoundState());
                current_state = current_state.simulate_move(from.first, from.second, to.first, to.second);

            }
            move_generation(current_state);
            if (current_state.checkmate(1))
                end_game_ai = -1; //you lost
            else if (current_state.checkmate(-1))
                end_game_ai = 1; //you won
            else if (current_state.stalemate(1) || current_state.stalemate(-1))
                end_game_ai = 2;//draw

        }
        //start_time = chrono::steady_clock::now();

        window.clear();
        if (mainMenu.loadedMenu == 0) {
            mainMenu.DisplayMainMenu(window);
        }
        else if (mainMenu.loadedMenu == 3) {
            mainMenu.settingsPage.DisplaySettings(window);

        }
        else {
            pieces.draw_pieces(current_state, validPoints);
            button.draw();
        }
        ///////////////////////////////////////////////////////////////////************************
        if (end_game == 1)
        {
            end.change(1, 2, sounds, first_end);
            first_end = 0;
        }
        else if (end_game == -1)
        {
            end.change(2, 2, sounds, first_end);
            first_end = 0;
        }
        else if (end_game == 2 || end_game_ai == 2)
        {
            end.change(0, 0, sounds, first_end);
            first_end = 0;
        }
        if (end_game_ai == 1)
        {
            end.change(1, 1, sounds, first_end);
            first_end = 0;
        }
        else if (end_game_ai == -1)
        {
            end.change(2, 1, sounds, first_end);
            first_end = 0;
        }
        ///////////////////////////////////////////////////////////////////************************

        window.display(); // Display the window
        current_state.white_possible_moves.clear();
        current_state.black_possible_moves.clear();
        if (mainMenu.loadedMenu == 1)
            mouseX = 0, mouseY = 0;
    }
    return 0;
}
