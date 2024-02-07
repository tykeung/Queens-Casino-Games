#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <random>
#include <algorithm>
using namespace std;

enum Value
{
    ACE = 1,
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

enum Suit
{
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};

struct Card
{
    Value value;
    Suit suit;
};

class Deck
{
private:
    vector<Card> cards;

public:
    Deck()
    {
        for (int suit = CLUBS; suit <= SPADES; ++suit)
        {
            for (int value = ACE; value <= KING; ++value)
            {
                cards.push_back({static_cast<Value>(value), static_cast<Suit>(suit)});
            }
        }
    }

    void shuffle()
    {
        srand(static_cast<unsigned int>(time(0)));
        random_shuffle(cards.begin(), cards.end()); // Wjy can't I use this
    }

    Card dealCard()
    {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};
