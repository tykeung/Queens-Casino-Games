#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <limits>
#include <cmath>

using namespace std;

int getIntegerInput() {
    int input;
    while (true) {
        cin >> input;

        if (cin.fail()) {
            cout << "Invalid input. Please enter an integer." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        } else {
            // Clear the input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    return input;
}


bool getYesNoInput() {
    char choice;
    while (true) {
        cin >> choice;
        choice = tolower(choice); // Convert to lowercase for case-insensitive comparison

        if (choice == 'y' || choice == 'n') {
            break;
        } else {
            cout << "Invalid input. Please enter 'Y' or 'N'." << endl;
        }
    }
    return (choice == 'y');
}

class Tile {
public:
    enum State { HIDDEN, REVEALED, GEM, MINE };

    Tile() : state(HIDDEN), isMine(false) {}

    State getState() const { return state; }
    bool containsMine() const { return isMine; }

    void reveal() {
        state = REVEALED;
        if (isMine) {
            cout << "Mine hit! Game Over." << endl;
        } else {
            cout << "Gem collected!" << endl;
        }
    }

    void placeMine() { isMine = true; }

private:
    State state;
    bool isMine;
};

class Player {
public:
    Player() : betAmount(0), numMines(0), totalProfit(0), chipBalance(0), playing(true), totalWon(0) {}

    void setBetAmount(int amount) { betAmount = amount; }
    void setNumMines(int mines) { numMines = mines; }
    void setTotalProfit(int profit) { totalProfit = profit; }
    void setPlaying(bool status) { playing = status; }
    void setChipBalance(int balance) { chipBalance = balance; }
    void setWinnings(int winnings) {totalWon = winnings;}

    int getBetAmount() const { return betAmount; }
    int getNumMines() const { return numMines; }
    int getTotalProfit() const { return totalProfit; }
    int getChipBalance() const { return chipBalance; }
    int getWinnings() const { return totalWon; }

    bool isPlaying() const { return playing; }
    void endRound() { playing = false; }

private:
    int betAmount;
    int numMines;
    int totalProfit;
    bool playing;
    int chipBalance;
    int id;
    int totalWon;
};

class MinesweeperGame {
public:
    MinesweeperGame() : rtp(0.99), houseEdge(0.01), boardSize(5), consecutiveWins(0), 
    allTilesRevealed_(false) {}

    void resetBoard(Player& player) {
        // Clear the existing board tiles
        tiles.clear();
        
        // Initialize the board with hidden tiles
        for (int i = 0; i < boardSize * boardSize; ++i) {
            tiles.push_back(Tile());
        }
        
        // Place mines randomly
        for (int i = 0; i < player.getNumMines(); ++i) {
            int randomPosition = rand() % (boardSize * boardSize);
            tiles[randomPosition].placeMine();
        }
    }


    void startGame(Player& player, MinesweeperGame& game) {
        // Reset consecutive wins and multiplier
        game.consecutiveWins = 0;
        multiplier = 1.0;
        resetBoard(player);
        int chosenMines;

    while (true) {
        cout << "Enter the number of mines (1-24): ";
        cout.flush();
        cin >> chosenMines;

        if (cin.fail() || chosenMines < 1 || chosenMines > 24) {
            cout << "Invalid input. Please enter a number between 1 and 24." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }

    player.setNumMines(chosenMines);

    // Clear the input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int betAmount;
    cout << "Enter your bet amount (1-" << player.getChipBalance() << "): ";
    while (true) {
        betAmount = getIntegerInput();
        if (betAmount < 1 || betAmount > player.getChipBalance()) {
            cout << "Invalid bet amount. Please enter a number between 1 and " << player.getChipBalance() << ": ";
        } else {
            break;
        }
    }
        player.setBetAmount(betAmount);

    // Initialize game state for a new game
    player.setTotalProfit(0);
    player.setPlaying(true);

    // Set the multiplier based on the number of mines
    multiplier = calculateMultiplier(player.getNumMines());

    // Reset the board
    initializeBoard(player);

}


void revealTile(Tile& tile, Player& player) {
    if (tile.getState() == Tile::REVEALED) {
        cout << "This tile has already been revealed. Please choose another one." << endl;
        return;
    }

    tile.reveal();
    int newBalance;

    if (tile.containsMine()) {
        player.endRound();
        newBalance = player.getChipBalance() - player.getBetAmount();
        player.setChipBalance(newBalance);
        consecutiveWins = 0; // Reset consecutiveWins when a mine is revealed
        cout << "Current Chip Balance: $" << player.getChipBalance() << endl;
    } else {
        // Calculate and update winnings
        consecutiveWins++;
        double multiplier = calculateMultiplier(player.getNumMines());
        int winnings = player.getBetAmount() * multiplier;
        player.setTotalProfit(winnings);

        // Check if all tiles are revealed or the player chooses to end the round
        bool allTilesRevealed = checkAllTilesRevealed(player);
        if (allTilesRevealed || !player.isPlaying()) {
            // Add winnings to chip balance if the round ends
            newBalance = player.getChipBalance() + player.getTotalProfit();
            player.setChipBalance(newBalance);
            cout << "Total Winnings: $" << player.getTotalProfit() << endl;
            cout << "Current Chip Balance: $" << player.getChipBalance() << endl;
        }
    }
}

    bool checkAllTilesRevealed(Player& player) const {
        // Check if all non-mine tiles are revealed
        for (const auto& tile : tiles) {
            if (!tile.containsMine() && tile.getState() != Tile::REVEALED) {
                return false;
            }
        }
        return true;
    }

    bool allTilesRevealed() const {
        return allTilesRevealed_;
    }

    double calculateMultiplier(int numMines) const {
        return std::pow(1.05, numMines+consecutiveWins);
    }

    bool playAgain() {
        char choice;
        cout << "Do you want to start a new game? (Y/N): ";
        cin >> choice;

        while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
            cout << "Invalid input. Please enter 'Y' or 'N': ";
            cin >> choice;
        }

        if(choice == 'Y' || choice == 'y'){
            return true;
        }
        else return false;
}


    void initializeBoard(Player& player) {
        // Clear the existing board
        tiles.clear();

        // Initialize the board with mines
        for (int i = 0; i < player.getNumMines(); ++i) {
            int randomPosition = rand() % (boardSize * boardSize);
            tiles.push_back(Tile()); // Ensure there's a Tile object at the random position
            tiles[randomPosition].placeMine();
        }
    }

    int getBoardSize() const {
        return boardSize;
    }

    Tile& getTile(int row, int col) {
        int index = row * boardSize + col;
        return tiles[index];
    }

private:
    double multiplier;
    const double rtp;
    const double houseEdge;
    const int boardSize;
    int consecutiveWins;
    vector<Tile> tiles;
    bool allTilesRevealed_;
};


int main() {
    // Initialize random seed
    srand(static_cast<unsigned>(time(0)));
        
    // Create Minesweeper game instance
    MinesweeperGame game;

    // Create player
    Player player;
    player.setChipBalance(100);

    do {
        MinesweeperGame game;
        // Start a new game
        game.startGame(player, game);
        
        // Continue the game until the player chooses to start a new game or the game ends
        while (player.isPlaying()) {
            int row, col;
            cout << "Enter row coordinate (0-" << game.getBoardSize() - 1 << "): ";
            row = getIntegerInput();
            cout << "Enter column coordinate (0-" << game.getBoardSize() - 1 << "): ";
            col = getIntegerInput();

            if (row < 0 || row >= game.getBoardSize() || col < 0 || col >= game.getBoardSize()) {
                cout << "Invalid coordinates. Try again." << endl;
                continue;
            }

            game.revealTile(game.getTile(row, col), player);
            
            // Check if all non-mine tiles are revealed
            if (game.allTilesRevealed()) {
                player.endRound();
                break;
            }

            if (player.getChipBalance() <= 0) {
                cout << "Current Chip Balance: $" << player.getChipBalance() << endl;
                break; // Game over, no need to ask to continue
            }

            if (player.isPlaying()) {
                cout << "Total Winnings: $" << player.getTotalProfit()-player.getBetAmount() << endl;
                char endRoundChoice;
                cout << "Do you want to continue this round? (Y/N): ";
                cin >> endRoundChoice;
                while (endRoundChoice != 'Y' && endRoundChoice != 'y' && endRoundChoice != 'N' && endRoundChoice != 'n') {
                cout << "Invalid input. Please enter 'Y' or 'N': ";
                cin >> endRoundChoice;
        }
                if (tolower(endRoundChoice) == 'n') {
                    // Add total winnings to chip balance if the round ends
                    player.setChipBalance(player.getChipBalance() + player.getTotalProfit() - player.getBetAmount());
                    cout << "Current Chip Balance: $" << player.getChipBalance() << endl;
                    player.endRound();
                    break;
                }
            }
        }

        
    } while (game.playAgain());

    return 0;
}