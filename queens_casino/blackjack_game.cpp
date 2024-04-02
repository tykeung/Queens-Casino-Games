// To add
// 1. Make hit function
// 2. Make hand class
// 3. Insurance
// 4. Split



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
        newDeck();
        shuffle();
    }

    void newDeck() {
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
        random_shuffle(cards.begin(), cards.end()); 
    }

    Card dealCard()
    {
        if (cards.empty())
        {
            cout << "Deck is empty, get a new deck" << endl;
            newDeck();
            shuffle();
        }
        Card card = cards.back(); // combine these lines?
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
        rank = "ace";
        break;
    case JACK:
        rank = "jack";
        break;
    case QUEEN:
        rank = "queen";
        break;
    case KING:
        rank = "king";
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

string getCardSuit(Card card)
{
    string suit = "";
    switch (card.suit)
    {
    case CLUBS:
        suit = "clubs";
        break;
    case DIAMONDS:
        suit = "diamonds";
        break;
    case HEARTS:
        suit = "hearts";
        break;
    case SPADES:
        suit = "spades";
        break;
    }
    return suit;
}

string getCardSuitIcon(Card card)
{
    string suit = "";
    switch (card.suit)
    {
    case CLUBS:
        suit = "♣️";
        break;
    case DIAMONDS:
        suit = "♦️";
        break;
    case HEARTS:
        suit = "♥️";
        break;
    case SPADES:
        suit = "♠️";
        break;
    }
    return suit;
}

string getCardFile(Card card) {
    return getCardRank(card) + "_of_" + getCardSuit(card) + ".png";
}



// int getCardName(Card card)
// {
//     return card.rank + " of "+card.suit;
// }

// Card hitPlayer(Deck deck, Card playerHand, int playerTotal, int bet, int balance, bool playerBusted)
// {
//     // Add a card to the hand
//     // return the new hand

//     Card newCard = deck.dealCard();
//     playerHand.push_back(newCard);
//     playerTotal = calculatePoints(playerHand);
//     cout << "You drew a " << getCardRank(newCard) << endl;
//     if (playerTotal > 21)
//     {
//         cout << "Busted! You lose." << endl;
//         balance -= bet;
//         playerBusted = true;
//         break;
//     }
// }



int calculatePoints(vector<Card> *hand)
{
    int aces = 0;
    int total = 0;
    for (Card card : *hand)
    {
        if (card.rank == ACE)
        {
            aces++;
        }
        total += getCardValue(card);
    }

    while (aces > 0 and total > 21)
    {
        aces--;
        total -= 10;
    }

    return total;
}

// int main()
// {
//     /*
//     int balance = 1000; // starting balance
//     int bet;
//     char choice;
//     Deck deck;
//     deck.shuffle();
//     bool playerBusted = false;
//     bool dealerBusted = false;
//     int playerTotal = 0;
//     int dealerTotal = 0;

//     while (balance > 0)
//     {
//         cout << "Your balance: $" << balance << endl;
//         cout << "Place your bet: ";
//         cin >> bet;

//         if (bet > balance)
//         {
//             cout << "You don't have enough balance to place this bet!" << endl;
//             continue;
//         }

//         // Initiliaze hands
//         vector<Card> playerHand;
//         vector<Card> dealerHand;

//         //Replace with deal functions

//         // Deal cards from deck into hands
//         playerHand.push_back(deck.dealCard());

//         Card dealerFaceUp = deck.dealCard();
//         dealerHand.push_back(dealerFaceUp);

//         playerHand.push_back(deck.dealCard());
//         Card dealerFaceDown = deck.dealCard();
//         dealerHand.push_back(dealerFaceDown);

//         playerTotal = calculatePoints(playerHand);
//         dealerTotal = calculatePoints(dealerHand);

//         cout << "Dealer's face up card: " << getCardRank(dealerFaceUp) << endl;

//         // loop for Player's turn
//         while (true)
//         {
//             cout << "Your hand: ";
//             for (Card card : playerHand)
//             {
//                 //cout << getCardRank(card) << "(" << getCardValue(card) << ") ";
//                 cout << getCardRank(card)  << " ";
//             }

//             cout << endl
//                  << "Your hands' value: " << playerTotal << endl;
//             cout << "Hit, stand, or double? (h/s/d): ";
//             cin >> choice;

//             if (choice == 'h')
//             {

//                 //REPLACE WITH HIT FUNCTION
//                 Card newCard = deck.dealCard();
//                 playerHand.push_back(newCard);
//                 playerTotal = calculatePoints(playerHand);
//                 cout << "You drew a " << getCardRank(newCard) << endl;
//                 if (playerTotal > 21)
//                 {
//                     cout << "Busted! You lose." << endl;
//                     balance -= bet;
//                     playerBusted = true;
//                     break;
//                 }
//             }
//             else if (choice == 's')
//             {
//                 break;
//             }
//             else if(choice == 'd')
//             {
//                 // double the bet
//                 bet *= 2;

//                 //Hit once
//                 Card newCard = deck.dealCard();
//                 playerHand.push_back(newCard);
//                 playerTotal = calculatePoints(playerHand);
//                 cout << "You drew a " << getCardRank(newCard) << endl;
//                 if (playerTotal > 21)
//                 {
//                     cout << "Busted! You lose." << endl;
//                     balance -= bet;
//                     playerBusted = true;
//                     break;
//                 }

//                 //Stop from hitting again
//                 break;
//             }
//             else
//             {
//                 cout << "Invalid choice! Please enter 'h' for hit or 's' for stand." << endl;
//             }
//         }

//         // Dealer's turn
//         if (!playerBusted) {
//             while (true) {

//                 cout << "Dealer's hand: ";
//                 for (const auto& card : dealerHand) {
//                     cout << getCardRank(card) << " ";
//                 }
//                 cout << endl;

//                 dealerTotal = calculatePoints(dealerHand);
//                 if (dealerTotal >= 17) {
//                     break;
//                 }
//                 Card newCard = deck.dealCard();
//                 dealerHand.push_back(newCard);
//                 cout << "Dealer hit and drew a " << getCardRank(newCard) << endl;
//             }

//             if (dealerTotal < 21 ) {
//                 cout << "Dealer stands" << endl;
//             }

            
//             cout << "Dealer's final hand: ";
//             for (const auto& card : dealerHand) {
//                 cout << getCardRank(card) << "" << getCardSuitIcon(card) << " ";
//             }
//             cout << endl;
//             cout << "Dealer's hands' value: " << dealerTotal << endl;

//             if (dealerTotal > 21 ) {
//                 cout << "The dealer busted! You win!" << endl;
//                 balance += bet;
//             }else if(playerTotal > dealerTotal) {
//                 cout << "You win!" << endl;
//                 balance += bet;
//             }
//              else if (dealerTotal > playerTotal) {
//                 cout << "Dealer wins!" << endl;
//                 balance -= bet;
//             } else {
//                 cout << "It's a tie! The dealer wins..." << endl;
//                 balance -= bet;
//             }
//         }

//         cout << "Your balance: $" << balance << endl;

//         if (balance > 0)
//         {
//             cout << "Do you want to play again? (y/n): ";
//             cin >> choice;

//             if (choice != 'y')
//             {
//                 break;
//             }
//         }
//         else
//         {
//             break;
//         }
//     }

//     if (balance <= 0)
//     {
//         cout << "You are out of money! ";
//     }
//     cout << "Your final balance: $" << balance << endl;
//     return 0;
//        */
// }
