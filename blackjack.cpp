#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <random>
#include <algorithm>
using namespace std;

enum Rank
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
    Rank rank;
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
            for (int rank = ACE; rank <= KING; ++rank)
            {
                cards.push_back({static_cast<Rank>(rank), static_cast<Suit>(suit)});
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

string getCardRank(Card card)
{
    string rank = "";
    switch (card.rank)
    {
    case ACE:
        rank = "Ace";
        break;
    case JACK:
        rank = "Jack";
        break;
    case QUEEN:
        rank = "Queen";
        break;
    case KING:
        rank = "King";
        break;
    default:
        rank = to_string(card.rank);
    }
    return rank;
}

int getCardValue(Card card)
{
    int value = 0;
    switch (card.rank)
    {
    case ACE:
        value = 11; // Initially consider Ace as 11
        //Do I need to pass in the hand or the points?
        break;
    case JACK:
    case QUEEN:
    case KING:
        value = 10;
        break;
    default:
        value = card.rank;
    }
    return value;
}

// int getCardName(Card card)
// {
//     return card.value + " of "+card.suit;
// }

int calculatePoints(vector<Card> hand)
{
    int total = 0;
    for (Card card : hand)
    {
        total += getCardValue(card);
    }

    return total;
}

int main()
{
    int balance = 1000; // starting balance
    int bet;
    char choice;
    Deck deck;
    deck.shuffle();
    bool playerBusted = false;
    bool dealerBusted = false;
    int playerTotal = 0;
    int dealerTotal = 0;

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

        // Initiliaze hands
        vector<Card> playerHand;
        vector<Card> dealerHand;

        // Deal cards from deck into hands
        playerHand.push_back(deck.dealCard());
        dealerHand.push_back(deck.dealCard());
        playerHand.push_back(deck.dealCard());
        dealerHand.push_back(deck.dealCard());

        playerTotal = calculatePoints(playerHand);
        dealerTotal = calculatePoints(dealerHand);

        // loop for Player's turn
        while (true)
        {
            cout << "Your hand: ";
            for (Card card : playerHand)
            {
                cout << getCardRank(card) << "(" << getCardValue(card) << ") ";
            }

            cout << endl
                 << "Your hands' value: " << playerTotal << endl;
            cout << "Hit or stand? (h/s): ";
            cin >> choice;

            if (choice == 'h')
            {
                playerHand.push_back(deck.dealCard());
                playerTotal = calculatePoints(playerHand);
                if (playerTotal > 21)
                {
                    cout << "Busted! You lose." << endl;
                    balance -= bet;
                    playerBusted = true;
                    break;
                }
            }
            else if (choice == 's')
            {
                break;
            }
            else
            {
                cout << "Invalid choice! Please enter 'h' for hit or 's' for stand." << endl;
            }
        }

        // loop for Dealer's turn
        // while (true)
        // {
        // }

        cout << "Your balance: $" << balance << endl;

        if (balance > 0)
        {
            cout << "Do you want to play again? (y/n): ";
            cin >> choice;

            if (choice != 'y')
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    if (balance <= 0)
    {
        cout << "You are out of money! ";
    }
    cout << "Your final balance: $" << balance << endl;
    return 0;
}