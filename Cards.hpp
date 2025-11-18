#ifndef FINALPROJECTSPRING2025_CARDS_HPP
#define FINALPROJECTSPRING2025_CARDS_HPP
#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

//two enums for both the rank and suite of the card
enum RANK
{   ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING

};

enum SUITE
{
    HEARTS,
    SPADES,
    DIAMONDS,
    CLUBS,
};

// two vectors that contain the names of the rank and the suite
inline std::vector rankNames{ACE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING};
inline std::vector suiteNames{HEARTS,SPADES,DIAMONDS,CLUBS};

class Cards
{
    protected:
        RANK Rank;
        SUITE Suite;
    public:
        //constructors one default, and one that initializes the card with a rank and a suite
        Cards()=default;

        Cards(RANK r, SUITE s)
        {
            Rank = r;
            Suite = s;
        };
        //gives the rank of the card
        RANK getRank()
        {
            return Rank;
        };
        //gives the suite of the card
        SUITE getSuite()
        {
            return Suite;
        }
    //prints out a card's rank and suite
    void printRankAndSuite()
        {
            std::cout << "Rank: " << getRank() << std::endl;
            std::cout << "Suite: " << getSuite() << std::endl;

        }
    //operator overload to compare two cards
    bool operator==(const Cards& card)
    {
            return Rank == card.Rank && Suite == card.Suite;
    }
    //destructor when the program is finished
    ~Cards()=default;
};

class Deck: public Cards
{
    //create a vector of cards
    std::vector<Cards> Cards;

    public:
    //sets up the deck of cards
    Deck() : Cards()
    {

        for (auto & suiteName : suiteNames)
    {
        for (auto & rankName : rankNames)
        {
            Cards.emplace_back(rankName, suiteName);
        }
    }
    };
    //shuffles the cards in a random order
    void shuffleCards()
    {
        //create a random seed and place that seed inside a Mersenne Twister algorithm
        std::random_device randCards;
        std::mt19937 cardGenerator(randCards());
        std::shuffle(std::begin(Cards), std::end(Cards), cardGenerator);
    }
    ~Deck()=default;
};

class Hand
{
    std::vector<Cards> cardHand{0};
    int cardLimit{13};

    public:
    //reserves space for integer n
    Hand()
    {
        cardHand.reserve(cardLimit);
    };
    //sets up the maximum amount of cards that the hand can have
    void setHand(int n)
    {
        cardLimit = n;
        if (cardLimit < 13)
        {
            cardHand.shrink_to_fit();
        }
        cardHand.resize(n);
    }
    //allows a set amount of cards to be added to the hand until it reaches the limit
     void addCard(const Cards c)
    {
         if (cardHand.size() <= cardLimit)
         {
                cardHand.emplace_back(c);
         }
    }
    //removes a card as long as it's inside the vector
    void removeCard(int rC)
    {
         if (rC > cardLimit || rC < 0)
             return;
        auto removeCard = std::remove(std::begin(cardHand), std::end(cardHand), cardHand[rC]);
         cardHand.erase(removeCard, std::end(cardHand));
    }
    //sorts the hand by rank and suite
    void sortHand()
    {
        for (int pass = 0 ; pass < (cardLimit - 1) ; pass++)
        {
            for (int i = 0; i < (cardLimit-1); i++)
                if (cardHand[i].getSuite() > cardHand[i+1].getSuite() || cardHand[i].getSuite() > cardHand[i+1].getSuite() && cardHand[i].getRank() > cardHand[i+1].getRank())
                {
                    std::swap(cardHand[i], cardHand[i+1]);
                }
        }

     }
    ~Hand()=default;
};

#endif //FINALPROJECTSPRING2025_CARDS_HPP
//
// std::vector<Cards> Cards{0};
// std::random_device randCards;
// static std::mt19937 cardRNG;
// std::uniform_int_distribution<> distribution;
// static void shuffleCards()
// {
//     std::shuffle(Cards.begin(),Cards.end(),cardRNG);
// }