/*
 *  player.h
 *  CS11 Splendor
 *  Created by: Kate Kooken (@kkooke01)
 */

#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include "card.h"
using namespace std;

class Player {
   public:
      Player();
      ~Player();
      
      //getters
      int totalPrestige();
      int getDiscount(int index);
      int getGemCount(int index);
      Card *getReserve(int index);

      //setters
      void setPrestige(int new_prestige);
      void setDiscount(int new_discount, int index);
      void setGemCount(int new_count, int index);
      void setReserve(Card *new_card, int index);

   private:
      int prestige;
      int discounts[5];
      int gem_count[6];
      Card *reserve[3];
};

#endif