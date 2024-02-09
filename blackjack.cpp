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

int getCardValue(Card card)
{
    int value = 0;
    switch (card.value)
    {
    case ACE:
        value = 11; // Initially consider Ace as 11
        break;
    case JACK:
    case QUEEN:
    case KING:
        value = 10;
        break;
    default:
        value = card.value;
    }
    return value;
}

int main()
{
    int balance = 1000; // starting balance
    int bet;
    char choice;
    Deck deck;
    deck.shuffle();
    bool playerBusted = false;
    bool dealerButsed = false;

    while (balance > 0)
    {
        cout << "Your balance: $" << balance << endl;
        cout << "Place your bet: ";
        cin >> bet;

        if (bet > balance)
        {
            cout << "You don't have enough balance to place this bet!" << endl;
            continue;
        }

        //Initiliaze hands
        vector<Card> playerHand;
        vector<Card> dealerHand;

        //Deal cards from deck into hands
        playerHand.push_back(deck.dealCard());
        dealerHand.push_back(deck.dealCard());
        playerHand.push_back(deck.dealCard());
        dealerHand.push_back(deck.dealCard());

        // loop for Player's turn
        while (true) {

        }

        
        // loop for Dealer's turn
        while (true) {

        }



        cout << "Your balance: $" << balance << endl;

        if (balance > 0)
        {
            cout << "Do you want to play again? (y/n): ";
            cin >> choice;

            if (choice != 'y')
            {
                break;
            }
        }else{
            break;
        }
    }

    if (balance <= 0){
        cout << "You are out of money! ";
    }
    cout << "Your final balance: $" << balance << endl;
    return 0;
}