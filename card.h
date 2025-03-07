/*
 *  card.h
 *  CS11 Splendor
 *  Created by: Cam Kooken (@cbkooken)
 */

#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Card {
   public:
      //constructor
      Card(char type, int prestige, string discount, int cost_0, int cost_1, 
         int cost_2, int cost_3, int cost_4);
   
      //getters
      char getType();
      int getPrice(int index);
      string getGemColor();
      int getPrestige();

      //setter
      void setPrice(int new_price, int index);

      //initial price values (I know this is ugly but I wasn't sure how else to
      //access them)
      int cost_0;
      int cost_1;
      int cost_2;
      int cost_3;
      int cost_4;

   private:
      char type;
      int prestige;
      string discount;
      int price[5];
};

#endif
