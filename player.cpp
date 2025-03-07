/*
 *  player.cpp
 *  CS11 Splendor
 *  Created by: Cam Kooken (@cbkooken)
 */

#include "player.h"

//constructor
Player::Player() {
   prestige = 0;

   for (int i = 0; i < 5; i++) {
      discounts[i] = 0;
   }

   for (int j = 0; j < 3; j++) {
      this->reserve[j] = nullptr;
   }

   for (int k = 0; k < 6; k++) {
      gem_count[k] = 0;
   }
}

//destructor
Player::~Player() {
   for (int i = 0; i < 3; i++) {
      delete reserve[i];
   }
}
      
//getters

/* totalPrestige
 * Input: none
 * Description: Getter for prestige private field
 * Output: A player's current number of prestige points
 */
int Player::totalPrestige() {
   return prestige;
}

/* getDiscount
 * Input: none
 * Description: Getter for discounts private array
 * Output: A player's active discounts for a given gem color
 */
int Player::getDiscount(int index) {
   return discounts[index];
}

/* getGemCount
 * Input: none
 * Description: Getter for gem_count private array
 * Output: The number of a given color gem in a player's bank
 */
int Player::getGemCount(int index) {
   return gem_count[index];
}

/* getReserve
 * Input: none
 * Description: Getter for reserve private array
 * Output: A pointer to a Card class reserved by this player
 */
Card* Player::getReserve(int index) {
   return reserve[index];
}

//setters

/* setPrestige
 * Input: New prestige value
 * Description: Setter for prestige private field
 * Output: Changes the value of this player's prestige variable
 */
void Player::setPrestige(int new_prestige) {
   prestige = new_prestige;
}

/* setDiscount
 * Input: New discount value, index variable
 * Description: Setter for discounts private array
 * Output: Changes the value of this player's discounts variable at given index
 */
void Player::setDiscount(int new_discount, int index) {
   discounts[index] = new_discount;
}

/* setGemCount
 * Input: New gem count value, index variable
 * Description: Setter for gem_count private array
 * Output: Changes the value of this player's gem_count variable at given index
 */
void Player::setGemCount(int new_count, int index) {
   gem_count[index] = new_count;
}

/* setReserve
 * Input: Pointer to new reserve card, index variable
 * Description: Setter for reserve private array
 * Output: Changes the Card pointed to by this player's reserve variable at 
 *       given index
 */
void Player::setReserve(Card *new_card, int index) {
   reserve[index] = new_card;
}
