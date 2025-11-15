#include "Cards.hpp"

//HEARTS = 1 PT
// QUEEN OF SPADES = 13 POINTS
int main()
{
    //create the deck
    Deck Deck;


    std::cout << Deck.size() << std::endl;
    //Two of clubs
    Deck[40].printRankAndSuite();
    //Queen of Spades
    Deck[33].printRankAndSuite();

    //create the hand for each player
}