/*
 *  card.cpp
 *  CS11 Splendor
 *  Created by: Kate Kooken (@kkooke01)
 */

#include "card.h"

//constructor
Card::Card(char type, int prestige, string discount, int cost_0, int cost_1, 
   int cost_2, int cost_3, int cost_4) {

   this->type = type;
   this->prestige = prestige;
   this->discount = discount;

   this->price[0] = cost_0;
   this->price[1] = cost_1;
   this->price[2] = cost_2;
   this->price[3] = cost_3;
   this->price[4] = cost_4;
}

/* getType
 * Input: none
 * Description: Getter for type private field
 * Output: The type of this card
 */
char Card::getType() {
   return type;
}

/* getPrice
 * Input: Index variable
 * Description: Getter for price private array
 * Output: The price of this card for given color index
 */
int Card::getPrice(int index) {
   return price[index];
}

/* getGemColor
 * Input: none
 * Description: Getter for discount private field
 * Output: The color discount given by this card
 */
string Card::getGemColor() {
   return discount;
}

/* getPrestige
 * Input: none
 * Description: Getter for prestige private field
 * Output: The prestige value of this card
 */
int Card::getPrestige() {
   return prestige;
}

//setter

/* setPrice
 * Input: New price, index variable
 * Description: Setter for price private array
 * Output: Adjusts price of this card at given color index
 */
void Card::setPrice(int new_price, int index) {
   this->price[index] = new_price;
}