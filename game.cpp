/*
 * game.cpp
 * CS11 Splendor
 * Edited by: Cam Kooken (@kkooke01)
 */

#include "game.h"

//constructor
Game::Game(string filename, string play_nobles){
   //store gem colors for easy reference in drawing functions 
   colors[0] = "white";
   colors[1] = "red";
   colors[2] = "pink";
   colors[3] = "blue";
   colors[4] = "green";
   colors[5] = "gold";

   //include nobles or not
   if(play_nobles == "true"){
        this->play_nobles = true;
   }else{
        this->play_nobles = false;
   }

   //initialize the board
   for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
   }     

    initDecks(filename);
    initGridBank();
}

//destructor
Game::~Game() {
    delete vendors;
    delete transport;
    delete mines;

    for (int i = 0; i < 3; i++) {
        player_1.setReserve(nullptr, i);
        player_2.setReserve(nullptr, i);
    }
}

/* initDecks
 * Input: Name of input file
 * Description: Helper function for Game, reads from input file & initializes 
 *      decks of cards
 * Output: Initializes & fills 3 vector Deck classes
 */
void Game::initDecks(string filename) {
   vendors = new Vector();
   mines = new Vector();
   transport = new Vector();

   ifstream infile;
   infile.open(filename);
   if(infile.fail()) {
        cerr << "ERROR: failed to open file\n";
        exit(EXIT_FAILURE);
   }
   
   char type;
   int prestige;
   string discount;
   int cost_0, cost_1, cost_2, cost_3, cost_4;
   string skip;

   infile >> skip >> skip >> skip >> skip >> skip >> skip >> skip >> skip;

   while (!infile.eof()) {
        infile >> type >> prestige >> discount >> cost_0 >> cost_1 >> cost_2 >>
            cost_3 >> cost_4;
        
        new_card = new Card(type, prestige, discount, cost_0, cost_1, 
            cost_2, cost_3, cost_4);
        
        if (new_card->getType() == 'm') {
            //add to mines deck
            mines->push_back(new_card);
        } else if (new_card->getType() == 't') {
            //add to transport deck
            transport->push_back(new_card);
        } else if (new_card->getType() == 'v') {
            //add to vendors deck
            vendors->push_back(new_card);
        }
   }
   infile.close();
   // I know this is where my mem leak is coming from, don't know how to fix it
   delete new_card;
}

/* initGridBank
 * Input: none
 * Description: Helper function for Game, initializes card grid & gem bank
 * Output: Modifies Game's grid, gem_bank fields
 */
void Game::initGridBank() {
    //draw cards from decks & make grid
    for (int i = 0; i < 4; i++) {
        grid[0][i] = vendors->back();
        vendors->pop_back();

        grid[1][i] = transport->back();
        transport->pop_back();

        grid[2][i] = mines->back();
        mines->pop_back();
   }

   //initialize gem bank
   for (int j = 0; j < 5; j++) {
        gem_bank[j] = 4;
   }
    gem_bank[5] = 5;
}

//main game loop
void Game::playGame(){
    bool game_over = false;
    bool player1_turn = true;

    while(!game_over){
        //reset the screen
        clearBoard();
        screen_clear();
        screen_home();
        //draw the board
        drawBoard();

        game_over = takeTurn(&player_1, &player_2, player1_turn);

        if ((player_1.totalPrestige() >= 15 || 
            player_2.totalPrestige() >= 15) && player1_turn == false) {
            game_over = true;
        }

        //change turns
        player1_turn = !player1_turn;
    } 

    if (player_1.totalPrestige() >= 15) {
        if (player_2.totalPrestige() >= 15) {
            cout << "You losers tied - BORING!!!\n";
        } else {
            cout << "Congratulations Player 1: YOU WIN!!!\n";
        }
    } else if (player_2.totalPrestige() >= 15) {
        cout << "Congratulations Player 2: YOU WIN!!!\n";
    }
}

/* takeTurn
 * Input: Pointers to two Player classes, Boolean indicating whose turn it is
 * Description: Helper function for playGame, handles an indvidual turn
 * Output: Bool variable determining whether the game is over after each turn
 */
bool Game::takeTurn(Player *p1, Player *p2, bool player1_turn) {
    bool game_over = false;
    
    if (player1_turn) {
        cout << "PLAYER 1, enter your next move: ";
    } else {
        cout << "PLAYER 2, enter your next move: ";
    }

    string query;
    cin >> query;
        
    if (player1_turn) {
        game_over = queryResponse(p1, query);
        returnGems(p1, true);
    } else {
        game_over = queryResponse(p2, query);
        returnGems(p2, false);
    }

    return game_over;
}

//Get the gem color associated with the given index 
string Game::indexToColor(int index){
    return colors[index];
}

//Get index number associated with given gem color
int Game::colorIndex(string color) {
    for (int i = 0; i < 6; i++) {
        if (color == colors[i]) {
            return i;
        }
    }
}

/* queryResponse
 * Input: Pointer to a Player class, query to respond to
 * Description: Handles each possible query provided by user
 * Output: A Boolean variable determining whether the game is over
 */
bool Game::queryResponse(Player *p, string query) {
    if (query == "q") {
        return true;
    } else if (query == "p2") {
        string color;
        cin >> color;

        takeTwo(p, color);
        return false;
    } else if (query == "p3") {
        string color1, color2, color3;
        cin >> color1 >> color2 >> color3;

        takeThree(p, color1, color2, color3);
        return false;
    } else if (query == "b") {
        buyBoardQuery(p);
        return false;
    } else if (query == "r") {
        reserveQuery(p);
        return false;
    } else if (query == "br") {
        int index;
        cin >> index;
        index -= 1;

        buyReserve(p, index);
        for (int i = 2; i > index; i--) {
            p->setReserve(p->getReserve(i), i - 1);
        }
        return false;
    }   
}

/* buyBoardQuery
 * Input: Pointer to a Player class
 * Description: Helper function for queryResponse, handles buying cards from 
 *      different decks on the board
 * Output: Potentially modifies one of the Deck classes
 */
void Game::buyBoardQuery(Player *p) {
    char type;
    int index;
    cin >> type >> index;
    index -= 1;

    if (type == 'm') {
        buyBoard(p, grid[2][index]);

        grid[2][index] = mines->back();
        mines->pop_back();
    } else if (type == 't') {
        buyBoard(p, grid[1][index]);

        grid[1][index] = transport->back();
        transport->pop_back();
    } else if (type == 'v') {
        buyBoard(p, grid[0][index]);

        grid[0][index] = vendors->back();
        vendors->pop_back();
    }
}

/* reserveQuery
 * Input: Pointer to a Player class
 * Description: Helper function for queryResponse, handles reserving cards from 
 *      different decks on the board
 * Output: Potentially modifies one of the Deck classes, Player's reserve field
 */
void Game::reserveQuery(Player *p) {
    char type;
    int index;
    cin >> type >> index;
    index -= 1;

    if (type == 'm') {
        reserveCard(p, grid[2][index]);

        grid[2][index] = mines->back();
        mines->pop_back();
    } else if (type == 't') {
        reserveCard(p, grid[1][index]);

        grid[1][index] = transport->back();
        transport->pop_back();
    } else if (type == 'v') {
        reserveCard(p, grid[0][index]);

        grid[0][index] = vendors->back();
        vendors->pop_back();
    }
}

/* takeTwo
 * Input: Pointer to a Player class, color of gems being taken
 * Description: Handles the 'p2' query, gives the player 2 gems of same color 
 *      from gem bank
 * Output: Modifies gem_bank array, Player's gem_count field
 */
void Game::takeTwo(Player *p, string color) {
    int color_index = colorIndex(color);

    if (gem_bank[color_index] <= 2) {
        cout << "Invalid move (unavailable gems) - enter a valid move: ";
        string query;
        cin >> query;
        queryResponse(p, query);
    } else {
        int new_count = p->getGemCount(color_index) + 2;

        p->setGemCount(new_count, color_index);
        gem_bank[color_index] -= 2;
    }
}

/* takeThree
 * Input: Pointer to a Player class, colors of gems being taken
 * Description: Handles the 'p3' query, gives the player 2 gems of different
 *      colors from gem bank
 * Output: Modifies gem_bank array, Player's gem_count field
 */
void Game::takeThree(Player *p, string color1, string color2, string color3) {
    int color1_index = colorIndex(color1);
    int color2_index = colorIndex(color2);
    int color3_index = colorIndex(color3);

    if (gem_bank[color1_index] < 1 || gem_bank[color2_index] < 1 || 
        gem_bank[color3_index] < 1) {
        cout << "Invalid move (unavailable gems) - enter a valid move: ";
        string query;
        cin >> query;
        queryResponse(p, query);
    } else {
        int new_count1 = p->getGemCount(color1_index) + 1;
        int new_count2 = p->getGemCount(color2_index) + 1;
        int new_count3 = p->getGemCount(color3_index) + 1;

        p->setGemCount(new_count1, color1_index);
        gem_bank[color1_index] -= 1;

        p->setGemCount(new_count2, color2_index);
        gem_bank[color2_index] -= 1;

        p->setGemCount(new_count3, color3_index);
        gem_bank[color3_index] -= 1;
    }
}

/* buyBoard
 * Input: Pointer to a Player class, pointer to Card class being bought
 * Description: Handles the 'b' query, buys card from one of the decks on the 
 *      board & adjusts player's stats accordingly
 * Output: Potentially modifies Card's price field, Player's discounts, 
 *      gem_count, and prestige fields
 */
void Game::buyBoard(Player *p, Card *c) {
    for (int k = 0; k < 5; k++) {
        if (c->getPrice(k) > 0) {
            c->setPrice((c->getPrice(k) - p->getDiscount(k)), k);
        }
    }

    bool can_buy = canBuy(p, c);

    if (can_buy == true) {
        int remainder = 0;

        for (int i = 0; i < 5; i++) {
            if (p->getGemCount(i) < c->getPrice(i)) {
                remainder += (c->getPrice(i) - p->getGemCount(i));
                gem_bank[i] += p->getGemCount(i);
                p->setGemCount(0, i);
            } else {
                p->setGemCount((p->getGemCount(i) - c->getPrice(i)), i);
                gem_bank[i] += c->getPrice(i);
            }
        }

        p->setGemCount((p->getGemCount(5) - remainder), 5);
        gem_bank[5] += remainder;

        p->setPrestige(p->totalPrestige() + c->getPrestige());
        int disc_index = colorIndex(c->getGemColor());
        p->setDiscount(p->getDiscount(disc_index) + 1, disc_index);

        delete c;
    } else {
        for (int j = 0; j < 5; j++) {
            c->setPrice((c->getPrice(j) + p->getDiscount(j)), j);
        }

        cout << "Invalid move (cannot afford card) - enter a valid move: ";
        string query;
        cin >> query;
        queryResponse(p, query);
    }
}

/* reserveCard
 * Input: Pointer to a Player class, pointer to Card class being reserved
 * Description: Handles the 'r' query, reserves card from one of the decks on 
 *      the board & gives a "wildcard" gem to player
 * Output: Modifies Player's reserve & gem_count fields
 */
void Game::reserveCard(Player *p, Card *c) {
    if (p->getReserve(0) == nullptr) {
        p->setReserve(c, 0);
    } else if (p->getReserve(1) == nullptr) {
        p->setReserve(c, 1);
    } else if (p->getReserve(2) == nullptr) {
        p->setReserve(c, 2);
    } else {
        cout << "Invalid move (reserve is already full) - enter a valid move: ";
        string query;
        cin >> query;
        queryResponse(p, query);
    }

    if (gem_bank[5] > 0) {
        p->setGemCount((p->getGemCount(5) + 1), 5);
        gem_bank[5] -= 1;
    }
}

/* buyReserve
 * Input: Pointer to a Player class, index variable
 * Description: Handles the 'br' query, buys card from player's reserve slot & 
 *      adjusts player's stats accordingly
 * Output: Potentially modifies Card's price field, Player's discounts, 
 *      gem_count, reserve, and prestige fields
 */
void Game::buyReserve(Player *p, int index) {
    Card *c = p->getReserve(index);
    for (int k = 0; k < 5; k++) {
        if (c->getPrice(k) > 0) {
            c->setPrice((c->getPrice(k) - p->getDiscount(k)), k);
        }
    }

    bool can_buy = canBuy(p, c);
    if (can_buy == true) {
        int remainder = 0;

        for (int i = 0; i < 5; i++) {
            if (p->getGemCount(i) < c->getPrice(i)) {
                remainder += (c->getPrice(i) - p->getGemCount(i));
                gem_bank[i] += p->getGemCount(i);
                p->setGemCount(0, i);
            } else {
                p->setGemCount((p->getGemCount(i) - c->getPrice(i)), i);
                gem_bank[i] += c->getPrice(i);
            }
        }

        p->setGemCount((p->getGemCount(5) - remainder), 5);
        gem_bank[5] += remainder;

        p->setPrestige(p->totalPrestige() + c->getPrestige());
        int disc_index = colorIndex(c->getGemColor());
        p->setDiscount(p->getDiscount(disc_index) + 1, disc_index);

        delete c;
    } else {
        for (int j = 0; j < 5; j++) {
            c->setPrice((c->getPrice(j) + p->getDiscount(j)), j);
        }
        
        cout << "Invalid move (cannot afford card) - enter a valid move: ";
        string query;
        cin >> query;
        queryResponse(p, query);
    }
}

/* canBuy
 * Input: Pointer to a Player class, pointer to Card class being bought
 * Description: Helper function for buyBoard & buyReserve, determines whether 
 *      player can actually afford card they're buying
 * Output: Boolean variable that determines whether player can afford card
 */
bool Game::canBuy(Player *p, Card *c) {
    int total_cost = 0;
    for (int i = 0; i < 5; i++) {
        total_cost += c->getPrice(i);
    }

    /* cout << total_cost << endl; */

    for (int j = 0; j < 5; j++) {
        if (p->getGemCount(j) < c->getPrice(j)) {
            total_cost -= p->getGemCount(j);
        } else {
            total_cost -= c->getPrice(j);
        }
    }

    /* cout << total_cost << endl; */

    if (p->getGemCount(5) < total_cost) {
        return false;
    } else {
        return true;
    }
}

/* returnGems
 * Input: Pointer to a Player class, Boolean determining whose turn it is
 * Description: Helper function for takeTurn, prompts player to return gems to 
 *      bank if they have more than 10
 * Output: Potentially modifies gem_bank, Player's gem_count field
 */
void Game::returnGems(Player *p, bool player1_turn) {
    int total_gems = 0;
    for (int i = 0; i < 6; i++) {
        total_gems += p->getGemCount(i);
    }

    if (total_gems > 10) {
        clearBoard();
        screen_clear();
        screen_home();
        //draw the board
        drawBoard();

        if (player1_turn) {
            cout << "PLAYER 1, you have more than 10 gems! Name one gem you ";
            cout << "would like to return: ";
        } else {
            cout << "PLAYER 2, you have more than 10 gems! Name one gem you ";
            cout << "would like to return: ";
        }
        string color;
        cin >> color;
        int index = colorIndex(color);

        p->setGemCount((p->getGemCount(index) - 1), index);
        gem_bank[index] += 1;

        return returnGems(p, player1_turn);
    } else {
        return;
    }
}

//helper functions for board ascii display:

//draw all of the elements onto the game board
void Game::drawBoard(){
    int vpos = 0;
    int hpos = 1;

    //draw the card decks
    if(!vendors->empty()){
        drawDeck(vpos, hpos, "Vendors");
    }
    vpos += CARD_HEIGHT + 1;
    if(!transport->empty()){
        drawDeck(vpos, hpos, "Transport");
    }
    vpos += CARD_HEIGHT + 1;
    if(!mines->empty()){
        drawDeck(vpos, hpos, "Mines");
    }

    //draw the in play cards
    vpos = 0;
    hpos = CARD_WIDTH + 2;
    for(int i = 0; i < CARD_ROWS; i++){
        for(int j = 0; j < CARD_COLS; j++){
            if(grid[i][j] != nullptr){
                drawCard(vpos, hpos, grid[i][j]);
            }
            hpos += CARD_WIDTH + 1;
        }
        vpos += CARD_HEIGHT + 1;
        hpos = CARD_WIDTH + 2;
    }

    //draw the gems
    int gemOffset = ((CARD_WIDTH + 1) * 5) + 3;
    vpos = 1;
    hpos = gemOffset;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            int index = (i * 2) + j;
            if(gem_bank[index] > 0){
                board[vpos][hpos + 5] = gem_bank[index] + 48;
                board_color[vpos][hpos + 5] = indexToColor(index);
                drawGem(vpos + 1, hpos, indexToColor(index));
            }
            hpos += CARD_WIDTH;
        }
        vpos += CARD_HEIGHT + 1;
        hpos = gemOffset;
    }
    
    //draw players
    drawPlayer(vpos + 1, 1, &player_1);
    drawPlayer(vpos + 1, ((CARD_WIDTH + 1) * 4), &player_2);

    //draw nobles
    if(play_nobles){
        hpos = ((CARD_WIDTH + 1) * 7) + 2;
        vpos = 2;
        for(int i = 0; i < NUM_NOBLES; i++){
            if(nobles->at(i) != nullptr){
                drawNoble(vpos, hpos, nobles->at(i));
            }
            vpos += CARD_HEIGHT + 1;
        }
    }

    //print out the finished board
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(board_color[i][j] != "white"){
                screen_fg(board_color[i][j]);
            }
            cout << board[i][j];
            screen_fg("white");
        }
        cout << endl;
   } 
   cout << endl; 
}

//draws a noble at the specified position of the board
void Game::drawNoble(int vpos, int hpos, Card *c){
    string top    = "|\\  /*\\  /|";
    string second = "| \\/   \\/ |";
    string blank  = "N         N";
    string bottom = "~~~~~~~~~~~";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];

        for(int j = 2; j < 5; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + 5][hpos + i] = bottom[i];
    }

    //draw the prices
    vpos += 2;
    for(int i = 0; i < GEMS-1; i++){
        int price = c->getPrice(i);
        if(price > 0){
            board_color[vpos][hpos + 4] = indexToColor(i);
            board_color[vpos][hpos + 5] = indexToColor(i);
            board_color[vpos][hpos + 6] = indexToColor(i);
            board[vpos][hpos + 4] = '|';
            board[vpos][hpos + 5] = price + 48;
            board[vpos][hpos + 6] = '|';
            vpos++;
        }
    }
}

//draws a player at the specified position of the board
void Game::drawPlayer(int vpos, int hpos, Player *p){
    //player string
    string prestige = to_string(p->totalPrestige());
    string player_s;
    if(hpos < 10){
        player_s = " PLAYER 1: " + prestige + " prestige point(s)";
    }else{
        player_s = " PLAYER 2: " + prestige + " prestige point(s)";
    }
    int length = player_s.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = player_s[i];
    }

    //bonus string
    vpos++;
    string bonus_s = "Discounts: ";
    length = bonus_s.length();
    for(int i = 0; i < GEMS-1; i++){
        int bonus = p->getDiscount(i);
        bonus_s += "|" + to_string(bonus) + "| ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
        if(bonus >= 10){
            length += 1;
        }
    }
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = bonus_s[i];
    }

    //gem string
    vpos++;
    string gems_s = "     Gems: ";
    length = gems_s.length();
    int different_gems = GEMS;
    for(int i = 0; i < different_gems; i++){
        int gems = p->getGemCount(i);
        gems_s += "(" + to_string(gems) + ") ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
    }
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = gems_s[i];
    }

    //reserve string
    vpos++;
    string reserve_s = " Reserved:";
    length = reserve_s.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = reserve_s[i];
    }

    hpos += reserve_s.length();
    for(int i = 0; i < 3; i++){
        Card *c = p->getReserve(i);
        if(c == nullptr){
            drawDeck(vpos, hpos, "");
        }else{
            drawCard(vpos, hpos, c);
        }
        hpos += CARD_WIDTH + 1;
    }
}

//draws a card deck at the specified position of the board
void Game::drawDeck(int vpos, int hpos, string type){
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];
        for(int j = 2; j < CARD_HEIGHT; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
    }

    int start_pos = hpos + (blank.length() / 2) - (type.length() / 2);
    length = type.length();
    for(int i = 0; i < length; i++){
        board[vpos + (CARD_HEIGHT/2)][start_pos + i] = type[i];
    }
}

//draws a gem at the specified position of the board
void Game::drawGem(int vpos, int hpos, string color){
    string gem_image[6];
    gem_image[0] = "  _______  ";
    gem_image[1] = ".'_/_|_\\_'.";
    gem_image[2] = "\\`\\  |  /`/";
    gem_image[3] = " `\\  | //' ";
    gem_image[4] = "   `\\|/`   ";
    gem_image[5] = "     `     ";

    for(int i = 0; i < GEMS; i++){
        int length = gem_image[i].length();
        for(int j = 0; j < length; j++){
            board[vpos + i][hpos + j] = gem_image[i][j];
            board_color[vpos + i][hpos + j] = color;
        }
    }
}

//draws a card at the specified position of the board
void Game::drawCard(int vpos, int hpos, Card *c){
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];

        if(i == 8 || i == 9){
            board_color[vpos + 1][hpos + i] = c->getGemColor();
        }
        board[vpos + 1][hpos + i] = second[i];

        for(int j = 2; j < CARD_HEIGHT; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
    }

    //draw the prestige number and gem
    board[vpos + 2][hpos + 3] = c->getPrestige() + 48;
    board_color[vpos + 2][hpos + 8] = c->getGemColor();
    board_color[vpos + 2][hpos + 9] = c->getGemColor();
    board[vpos + 2][hpos + 8] = '\\';
    board[vpos + 2][hpos + 9] = '/';
    board_color[vpos + 1][hpos + 8] = c->getGemColor();
    board_color[vpos + 1][hpos + 9] = c->getGemColor();
    board[vpos + 1][hpos + 8] = '_';
    board[vpos + 1][hpos + 9] = '_';

    //draw the prices
    vpos += 4;
    for(int i = 0; i < GEMS-1; i++){
        int price = c->getPrice(i);
        if(price > 0){
            board_color[vpos][hpos + 2] = indexToColor(i);
            board_color[vpos][hpos + 3] = indexToColor(i);
            board_color[vpos][hpos + 4] = indexToColor(i);
            board[vpos][hpos + 2] = '(';
            board[vpos][hpos + 3] = price + 48;
            board[vpos][hpos + 4] = ')';
            vpos++;
        }
    }

}

//clear the board display
void Game::clearBoard(){
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
   }  
}
