/*
 * game.h
 * CS11 Splendor
 * Edited by: Cam Kooken (@cbkooken)
 */

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <string>
#include "card.h"
#include "player.h"
#include "deck.h"
#include "termfuncs.h"
using namespace std;

class Game {
  public:
    //constructor
    Game(string filename, string play_nobles);
      void initDecks(string filename);
      void initGridBank();

    //destructor
    ~Game();

    //main gameplay
    void playGame();
    bool takeTurn(Player *p1, Player *p2, bool player1_turn);

    //utility functions
    string indexToColor(int index);
    int colorIndex(string color);

    Card *new_card;

  private:
    //drawing functions
    void drawBoard();
    void drawNoble(int vpos, int hpos, Card *c);
    void drawPlayer(int vpos, int hpos, Player *p);
    void drawDeck(int vpos, int hpos, string type);
    void drawCard(int vpos, int hpos, Card *c);
    void drawGem(int vpos, int hpos, string color);
    void clearBoard();

    static const int BOARD_HEIGHT = 41;
    static const int BOARD_WIDTH = 120;
    static const int CARD_WIDTH = 13;
    static const int CARD_HEIGHT = 8;
    static const int CARD_ROWS = 3;
    static const int CARD_COLS = 4;
    static const int NUM_NOBLES = 3;
    static const int GEMS = 6;

    int gem_bank[GEMS];
    Player player_1;
    Player player_2;
    Vector *vendors;
    Vector *transport;
    Vector *mines;
    Vector *nobles;

    bool queryResponse(Player *p, string query);
    void reserveQuery(Player *p);
    void buyBoardQuery(Player *p);
    void returnGems(Player *p, bool player1_turn);

    void takeTwo(Player *p, string color);
    void takeThree(Player *p, string color1, string color2, string color3);
    void reserveCard(Player *p, Card *c);
    void buyReserve(Player *p, int index);
    void buyBoard(Player *p, Card *c);
    bool canBuy(Player *p, Card *c);

    //nobles activated true/false
    bool play_nobles;

    //cards in play
    Card *grid[CARD_ROWS][CARD_COLS];

    //display
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    string board_color[BOARD_HEIGHT][BOARD_WIDTH];

    //gem colors in the game
    string colors[GEMS];
};

#endif
