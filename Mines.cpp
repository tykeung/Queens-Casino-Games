#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <limits>

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
    Player() : betAmount(0), numMines(0), totalProfit(0), playing(true) {}

    void setBetAmount(int amount) { betAmount = amount; }
    void setNumMines(int mines) { numMines = mines; }
    void setTotalProfit(int profit) { totalProfit = profit; }
    void setPlaying(bool status) { playing = status; }

    int getBetAmount() const { return betAmount; }
    int getNumMines() const { return numMines; }
    int getTotalProfit() const { return totalProfit; }

    bool isPlaying() const { return playing; }
    void endRound() { playing = false; }

private:
    int betAmount;
    int numMines;
    int totalProfit;
    bool playing;
};

class MinesweeperGame {
public:
    MinesweeperGame() : multiplier(1.0), rtp(0.99), houseEdge(0.01), boardSize(4) {}

    void startGame(Player& player, MinesweeperGame& game) {
    // Set the number of mines based on the player's choice
    int chosenMines;

    while (true) {
        cout << "Enter the number of mines (1-15): ";
        cout.flush();
        cin >> chosenMines;

        if (cin.fail() || chosenMines < 1 || chosenMines > 15) {
            cout << "Invalid input. Please enter a number between 1 and 15." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }

    player.setNumMines(chosenMines);

    // Clear the input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Initialize game state for a new game
    player.setTotalProfit(0);
    player.setPlaying(true);

    // Set the multiplier based on the number of mines
    multiplier = calculateMultiplier(player.getNumMines());

    // Reset the board
    initializeBoard(player);

}


    void revealTile(Tile& tile, Player& player) {
        tile.reveal();

        if (tile.containsMine()) {
            player.endRound();
        } else {
            // Calculate and update total profit
            player.setTotalProfit(player.getTotalProfit() + player.getBetAmount() * multiplier);
        }
    }

    double calculateMultiplier(int numMines) const {
        // Calculate multiplier based on the number of mines
        // (Add your own logic for determining the multiplier)
        return 1.0 + 0.1 * numMines;
    }

    bool playAgain() {
        char choice;
        cout << "Do you want to keep playing? (Y/N): ";
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
    vector<Tile> tiles;
};


int main() {
    // Initialize random seed
    srand(static_cast<unsigned>(time(0)));

    // Create Minesweeper game instance
    MinesweeperGame game;



    do {
        // Create player
        Player player;
        game.startGame(player, game);
        // Continue the game until the player chooses to start a new game or the game ends
        while (player.isPlaying()) {
            int row, col;
            cout << "Enter row (0-" << game.getBoardSize() - 1 << "): ";
            row = getIntegerInput();
            cout << "Enter column (0-" << game.getBoardSize() - 1 << "): ";
            col = getIntegerInput();

            if (row < 0 || row >= game.getBoardSize() || col < 0 || col >= game.getBoardSize()) {
                cout << "Invalid coordinates. Try again." << endl;
                continue;
            }

            game.revealTile(game.getTile(row, col), player);

            // Check if the round has ended
            if (!player.isPlaying()) {
                cout << "Total Profit: $" << player.getTotalProfit() << endl;
                break;
            }
        }

    } while (game.playAgain());

    return 0;
}
