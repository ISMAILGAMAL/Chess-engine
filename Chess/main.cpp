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
struct buttons 
{
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
    void MouseClickButtons(MainMenu& mainMenu, GameState& current_state) {
        auto mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
        if (mainMenu.settingsPage.getSoundState())
            sound_button.setTextureRect(IntRect(6 * 16, 10 * 16, 16, 16));
        else
            sound_button.setTextureRect(IntRect(7 * 16, 10 * 16, 16, 16));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (main_button.getGlobalBounds().contains(mousePosition)) {
                mainMenu.loadedMenu = 0;
                current_state.initialize_board();
                reset = 1;
            }
            if (restart_button.getGlobalBounds().contains(mousePosition)) {
                current_state.initialize_board();
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
string createPathPieces(int& theme, string team, string piece)
{
    return "pieces/" + team + piece + to_string(theme) + ".png";
}
string createPathSounds(int& theme, string sound)
{
    return "sounds/" + sound + (theme != 4 ? to_string(theme) : to_string(1)) + ".wav";
}
string createPathBoard(int& theme)
{
    return "boards/board" + to_string(theme) + ".jpg";
}
struct Theme
{
    Vector2f board_scale;
    Texture pawnBtxt, knightBtxt, rookBtxt, kingBtxt, queenBtxt, bishopBtxt;
    Texture pawnWtxt, knightWtxt, rookWtxt, kingWtxt, queenWtxt, bishopWtxt;
    Texture validDottxt, not_validtxt;
    Texture bgk;
    Sprite bgkk;
    Sprite board;
    Texture boardTexture; // Texture for the chessboard
    void change_theme(MainMenu& mainmenu, int themenum = 1)
    {
        string team1 = (mainmenu.settingsPage.getColor() == 1 ? "w" : "b");
        string team2 = (mainmenu.settingsPage.getColor() != 1 ? "w" : "b");
        validDottxt.loadFromFile("green2.png");
        not_validtxt.loadFromFile("red.png");
        pawnBtxt.loadFromFile(createPathPieces(themenum, team1, "p"));
        knightBtxt.loadFromFile(createPathPieces(themenum, team1, "n"));
        rookBtxt.loadFromFile(createPathPieces(themenum, team1, "r"));
        kingBtxt.loadFromFile(createPathPieces(themenum, team1, "k"));
        queenBtxt.loadFromFile(createPathPieces(themenum, team1, "q"));
        bishopBtxt.loadFromFile(createPathPieces(themenum, team1, "b"));
        pawnWtxt.loadFromFile(createPathPieces(themenum, team2, "p"));
        knightWtxt.loadFromFile(createPathPieces(themenum, team2, "n"));
        rookWtxt.loadFromFile(createPathPieces(themenum, team2, "r"));
        kingWtxt.loadFromFile(createPathPieces(themenum, team2, "k"));
        queenWtxt.loadFromFile(createPathPieces(themenum, team2, "q"));
        bishopWtxt.loadFromFile(createPathPieces(themenum, team2, "b"));

        boardTexture.loadFromFile(createPathBoard(themenum)); // Load chessboard texture
        board.setTexture(boardTexture); // Sprite for the chessboard
        board.setScale(0.74f, 0.74f); // Scale the chessboard to fit the window
        if (themenum == 1)
        {
            bgk.loadFromFile("backgrounds/brown.png");
            bgkk.setTexture(bgk);
            //bgkk.setScale(window.getSize().x / bgkk.getLocalBounds().width, window.getSize().y / bgkk.getLocalBounds().height);
        }
        else if (themenum == 2)
        {
            bgk.loadFromFile("backgrounds/8_bit.jpg");
            bgkk.setTexture(bgk);
        }
        else if (themenum == 3)
        {
            bgk.loadFromFile("backgrounds/newspaper.png");
            bgkk.setTexture(bgk);
            bgkk.setScale(0.5, 0.5);
            bgkk.setPosition(window_w - 525, 0);
        }
        else if (themenum == 4)
        {
            bgk.loadFromFile("backgrounds/staunton.png");
            bgkk.setTexture(bgk);
            bgkk.setScale(1, 1);
            bgkk.setPosition(window_w - 500, 0);
        }
        else if (themenum == 5)
        {
            bgk.loadFromFile("backgrounds/marble.jpg");
            bgkk.setTexture(bgk);
            bgkk.setScale(1, 1);
            bgkk.setPosition(window_w - 525, 0);
        }
    }
};
struct soundss {
    SoundBuffer moveS, moveCh, prom, gameSt, gameEnd, captur, castl, tenS, illeg;
    Sound move, check, promotion, gamestart, gameend, capture, castle, tenseconds, illegal;
    void load_sounds(int themenum = 1)
    {
        moveS.loadFromFile(createPathSounds(themenum, "move-self"));
        moveCh.loadFromFile(createPathSounds(themenum, "move-check"));
        prom.loadFromFile(createPathSounds(themenum, "promote"));
        tenS.loadFromFile(createPathSounds(themenum, "tenseconds"));
        illeg.loadFromFile(createPathSounds(themenum, "illegal"));
        gameSt.loadFromFile(createPathSounds(themenum, "game-start"));
        gameEnd.loadFromFile(createPathSounds(themenum, "game-end"));
        captur.loadFromFile(createPathSounds(themenum, "capture"));
        castl.loadFromFile(createPathSounds(themenum, "castle"));
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
    void MouseClickButtons(MainMenu& mainMenu, GameState& currentState) {
        auto mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (main_button.getGlobalBounds().contains(mousePosition)) {
                mainMenu.loadedMenu = 0;
                currentState.initialize_board();
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

struct piecess
{
    void draw_pieces(GameState& current_state, bool validPoints[][8], Theme& theme)
    {
        window.draw(theme.bgkk);
        window.draw(theme.board);

        // Draw pieces
        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            for (int col = 0; col < BOARD_SIZE; ++col)
            {
                int pieceType = current_state.board[row][col];

                Sprite piece;
                switch (pieceType)
                {
                case 6: piece.setTexture(theme.pawnWtxt); break;
                case 4: piece.setTexture(theme.knightWtxt); break;
                case 5: piece.setTexture(theme.bishopWtxt); break;
                case 3: piece.setTexture(theme.rookWtxt); break;
                case 2: piece.setTexture(theme.queenWtxt); break;
                case 1: piece.setTexture(theme.kingWtxt); break;
                case -6: piece.setTexture(theme.pawnBtxt); break;
                case -4: piece.setTexture(theme.knightBtxt); break;
                case -5: piece.setTexture(theme.bishopBtxt); break;
                case -3: piece.setTexture(theme.rookBtxt); break;
                case -2: piece.setTexture(theme.queenBtxt); break;
                case -1: piece.setTexture(theme.kingBtxt); break;
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
                    Sprite validDot(theme.validDottxt), notvalid(theme.not_validtxt);
                    float scale = SQUARE_SIZE / max(validDot.getLocalBounds().width, validDot.getLocalBounds().height);
                    validDot.setScale(scale, scale);
                    notvalid.setScale(scale, scale);
                    notvalid.setPosition(calculateSquarePosition(row, col));
                    validDot.setPosition(calculateSquarePosition(row, col));
                    if (current_state.board[row][col] == 0) {
                        window.draw(validDot);
                    }
                    else {
                        window.draw(notvalid);
                    }
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
    bool first_end = 1;
    int end_game = -10, end_game_ai = -10;
    GameState current_state;
    Minimax AI;
    //MainMenu mainMenu(window.getSize());
    MainMenu mainMenu(window.getSize());
    //pieces
    piecess pieces;
    //theme
    Theme theme;
    theme.change_theme(mainMenu);
    //sounds
    soundss sounds;
    sounds.load_sounds();
    int current_color = 3;

    //END GAME
    End end;
    end.initialize();
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
            if (event.type == sf::Event::Closed)
                window.close();

            if (end_game != -10 || end_game_ai != -10)
                end.MouseClickButtons(mainMenu, current_state);

            if (Keyboard::isKeyPressed(Keyboard::Num1)) {
                theme.change_theme(mainMenu, 1);
                sounds.load_sounds(1);
            }
            if (Keyboard::isKeyPressed(Keyboard::Num2)) {
                theme.change_theme(mainMenu, 2);
                sounds.load_sounds(2);
            }
            if (Keyboard::isKeyPressed(Keyboard::Num3)) {
                theme.change_theme(mainMenu, 3);
                sounds.load_sounds(3);
            }
            if (Keyboard::isKeyPressed(Keyboard::Num4)) {
                theme.change_theme(mainMenu, 4);
                sounds.load_sounds(4);
            }
            if (Keyboard::isKeyPressed(Keyboard::Num5)) {
                theme.change_theme(mainMenu, 5);
                sounds.load_sounds(5);
            }
            if (Keyboard::isKeyPressed(Keyboard::R) || reset) {
                reset_validpoints(validPoints);
                current_state.initialize_board();
                current_state.player = 1;
                reset = 0;
                end_game = -10;
                end_game_ai = -10;
            }

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
                button.MouseClickButtons(mainMenu, current_state);
        }

        if (mainMenu.loadedMenu == 0) {
            auto mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
            mainMenu.MenuButtonsAnimation(window);
            mainMenu.MainMenuMouseClick(mousePosition, window);
            reset = 0;
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

                    }
                }
            }
            else
            {
                AI.iterative_deepening(current_state);
                string best = AI.best_move;
                cout << "Best Move: " << best << " " << AI.best_score << endl;
                cout << "Nodes Evaluated: " << AI.node_counter << endl;
                cout << "Depth Reached: " << AI.reached_depth << endl;
                cout << "Time Taken: " << AI.time_in_seconds << endl;
                myPair<int, int>from = to_index(best[0], best[1]), to = to_index(best[2], best[3]);
                applying_sounds(current_state, sounds, to.first, to.second, from.first, from.second, mainMenu.settingsPage.getSoundState());
                current_state = current_state.simulate_move(from.first, from.second, to.first, to.second);
                current_state.show();
            }
            move_generation(current_state);
            if (current_state.checkmate(1))
                end_game_ai = -1; //you lost
            else if (current_state.checkmate(-1))
                end_game_ai = 1; //you won
            else if (current_state.stalemate(1) || current_state.stalemate(-1))
                end_game_ai = 2;//draw
        }

        window.clear();
        if (mainMenu.loadedMenu == 0) {
            mainMenu.DisplayMainMenu(window);
        }
        else if (mainMenu.loadedMenu == 3) {
            mainMenu.settingsPage.DisplaySettings(window);
        }
        else {
            pieces.draw_pieces(current_state, validPoints, theme);
            button.draw();
        }

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

        window.display(); // Display the window
        current_state.white_possible_moves.clear();
        current_state.black_possible_moves.clear();
    }

    return 0;
}
