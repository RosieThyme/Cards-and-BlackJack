#ifndef FINALPROJECTSPRING2025_CARDS_HPP
#define FINALPROJECTSPRING2025_CARDS_HPP
#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

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
    DIAMONDS,
    HEARTS,
    SPADES,
    CLUBS,
};

inline std::vector rankNames{ACE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING};
inline std::vector suiteNames{DIAMONDS,HEARTS,SPADES,CLUBS};

class Cards
{
    protected:
        RANK Rank;
        SUITE Suite;
    public:
        Cards()=default;

        Cards(RANK r, SUITE s)
        {
            Rank = r;
            Suite = s;
        };
        void setRankandSuite(RANK r, SUITE s)
        {
            Rank = r;
            Suite = s;
        }
        RANK getRank()
        {
            return Rank;
        };

        SUITE getSuite()
        {
            return Suite;
        }
    void printRankAndSuite()
        {
            std::cout << "Rank: " << getRank() << std::endl;
            std::cout << "Suite: " << getSuite() << std::endl;

        }
        ~Cards()=default;
};

class Deck: public Cards
{
    std::mt19937 cardRNG;

    public:
    std::vector<Cards> Cards{0};
    Deck()
    {

        for (auto & suiteName : suiteNames)
    {
        for (auto & rankName : rankNames)
        {
            Cards.emplace_back(rankName, suiteName);
        }
    }
    };

    void shuffleCards()
    {
        std::shuffle(std::begin(Cards), std::end(Cards), cardRNG);
    }
    ~Deck()=default;
};

class Hand: public std::vector<Cards>
{
    public:
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