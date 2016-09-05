#include <iostream>
#include<stdio.h>
#include <vector>
#include <string>

// Arrays of chars for printing out the board all fancy-like
const char GLYPHS[3][3][3] =
{
    {
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' }
    },
    {
        { ' ', ' ', ' ' },
        { ' ', 'X', ' ' },
        { ' ', ' ', ' ' }
    },
    {
        { ' ', ' ', ' ' },
        { ' ', 'O', ' ' },
        { ' ', ' ', ' ' }
    }
};
const int TIE_VAL = -1;
// Indexes for board values
const int NO_VAL = 0;
const int X_VAL = 1;
const int O_VAL = 2;



// Game board class
struct AiMove
{
    AiMove() {};
    AiMove(int Score) : score(Score) {}
    int x;
    int y;
    int score;
};
class Board
{
public:

    // Initializes the board
    void init(int size);
    // Clears the board
    void clear();
    // Prints the board to standard output
    void print() const;

    // If no player won, returns NO_VAL. Otherwise returns X_VAL, O_VAL or TIE_VAL
    int checkVictory() const;

    // Sets value at x,y spot
    void setVal(int x, int y, int val)
    {
        _board[y * _size + x] = val;
    }

    // Gets value at x,y spot
    int getVal(int x, int y) const
    {
        return _board[y * _size + x];
    }

    // Getters
    int getSize() const { return _size; }

private:
    // Adds a horizontal line to string for printing
    void addHorizontalLine(std::string& s) const;
    // Adds a horizontal axis guide line to string for printing
    void addGuide(std::string& s) const;

    std::vector<int> _board;
    int _size;
};
class AI
{
public:
    // Initializes the AI player
    void init(int aiPlayer);
    // Performs the AI move
    void performMove(Board& board);
private:
    AiMove getBestMove(Board& board, int player);

    int _aiPlayer; ///< Index of the AI
    int _humanPlayer; ///< Index of the player
};


enum class GameState { PLAYING, EXIT };

class MainGame
{
public:
    // Runs the main loop
    void run();

private:
    // Initializes the game
    void init();
    // Performs a human controlled move
    void playerMove();
    // Performs an AI move
    void aiMove();
    // Changes players
    void changePlayer();
    // Ends a game and prompts for quit or re-try
    void endGame(bool wasTie);

    // Member Variables
    Board _board; ///< The tic-tac-toe boarde
    int _currentPlayer; ///< The index of the current player
    int _aiPlayer; ///< Index of the AI player
    GameState _gameState; ///< The state of the game
    AI _ai; ///< The AI player
    bool _isMultiplayer;
};


void Board::init(int size)
{
    // Just cap it at 9 since we have limited console space
    if (size > 9)
     size = 9;
    _size = size;
    // Set the board size
    _board.resize(size * size);
    // Clear it
    clear();
}

void Board::clear()
{
    for (int i = 0; i < _board.size(); i++)
    {
        _board[i] = NO_VAL;
    }
}

void Board::print() const
{
    // Instead of calling printf multiple times, we build a single
    // string and call printf once, since its faster.

    std::string text = "";
    // Just reserve a bunch of memory, this should be enough
    text.reserve(_size * _size * 4);

    // Make top guide
    addGuide(text);

    // Loop through columns
    for (int y = 0; y < _size; y++) {
        // Add line
        addHorizontalLine(text);
        // Loop through glyph y
        for (int gy = 0; gy < 3; gy++)
        {
            // Add number or space
            if (gy == 1)
            {
                std::string text =text+std::to_string(_size - y);
            } else {
                text += " ";
            }
            // Loop through row cells
            for (int x = 0; x < _size; x++)
            {
                // Loop through glyph x
                text += "|";
                for (int gx = 0; gx < 3; gx++)
                {
                    text += GLYPHS[getVal(x, _size - y - 1)][gy][gx];
                }
            }
            text += "|";
            // Add number or newline
            if (gy == 1)
            {
                std::string text =text+std::to_string(_size - y) + "\n";
            } else {
                text += "\n";
            }
        }
    }
    // Add bottom line
    addHorizontalLine(text);\
    // Make bottom guide
    addGuide(text);

    // printf is faster than cout

    // Print the string
    printf("%s\n", text.c_str());
}

int Board::checkVictory() const
{
    bool victory;
    int c;
    // Check the rows
    for (int y = 0; y < _size; y++)
    {
        c = getVal(0, y);
        if (c != NO_VAL)
        {
            victory = true;
            for (int x = 0; x < _size; x++)
            {
                if (getVal(x, y) != c)
                {
                    victory = false;
                    break;
                }
            }
            if (victory) return c;
        }
    }

    // Check the columns
    for (int x = 0; x < _size; x++)
    {
        c = getVal(x, 0);
        if (c != NO_VAL) {
            victory = true;
            for (int y = 0; y < _size; y++)
            {
                if (getVal(x, y) != c)
                {
                    victory = false;
                    break;
                }
            }
            if (victory) return c;
        }
    }

    // Check top left diagonal
    c = getVal(0, 0);
    if (c != NO_VAL) {
        victory = true;
        for (int xy = 0; xy < _size; xy++)
        {
            if (getVal(xy, xy) != c)
            {
                victory = false;
                break;
            }
        }
        if (victory) return c;
    }

    // Check top right diagonal
    c = getVal(_size - 1, 0);
    if (c != NO_VAL)
    {
        victory = true;
        for (int xy = 0; xy < _size; xy++)
        {
            if (getVal(_size - xy - 1, xy) != c)
            {
                victory = false;
                break;
            }
        }
        if (victory) return c;
    }

    // Check for tie game
    for (int i = 0; i < _board.size(); i++)
    {
        if (_board[i] == NO_VAL) return NO_VAL;
    }
    // If we get here, every spot was filled, so return tie
    return TIE_VAL;
}

void Board::addHorizontalLine(std::string& s) const
{
    s += "-";
    for (int x = 0; x < _size; x++)
    {
        s += "----";
    }
    s += "--\n";
}

void Board::addGuide(std::string& s) const
{
    s += " ";
    for (int i = 1; i <= _size; i++)
    {
        std::string s =s+ "| " + std::to_string(i) + " ";
    }
    s += "|\n";
}

void AI::init(int aiPlayer)
{
    _aiPlayer = aiPlayer;
    if (_aiPlayer == X_VAL)
    {
        _humanPlayer = O_VAL;
    } else {
        _humanPlayer = X_VAL;
    }
}

void AI::performMove(Board& board)
{
    AiMove bestMove = getBestMove(board, _aiPlayer);
    board.setVal(bestMove.x, bestMove.y, _aiPlayer);
}

AiMove AI::getBestMove(Board& board, int player)
{

    // Base case, check for end state
    int rv = board.checkVictory();
    if (rv == _aiPlayer)
    {
        return AiMove(10);
    } else if (rv == _humanPlayer)
    {
        return AiMove(-10);
    } else if (rv == TIE_VAL)
    {
        return AiMove(0);
    }

    std::vector<AiMove> moves;

    // Do the recursive function calls and construct the moves vector
    for (int y = 0; y < board.getSize(); y++)
    {
        for (int x = 0; x < board.getSize(); x++)
        {
            if (board.getVal(x, y) == NO_VAL)
            {
                AiMove move;
                move.x = x;
                move.y = y;
                board.setVal(x, y, player);
                if (player == _aiPlayer)
                {
                    move.score = getBestMove(board, _humanPlayer).score;
                } else
                {
                    move.score = getBestMove(board, _aiPlayer).score;
                }
                moves.push_back(move);
                board.setVal(x, y, NO_VAL);
            }
        }
    }

    // Pick the best move for the current player
    int bestMove = 0;
    if (player == _aiPlayer)
    {
        int bestScore = -1000000;
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i].score > bestScore)
            {
                bestMove = i;
                bestScore = moves[i].score;
            }
        }
    } else
    {
        int bestScore = 1000000;
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i].score < bestScore)
            {
                bestMove = i;
                bestScore = moves[i].score;
            }
        }
    }
    // Return the best move
    return moves[bestMove];
}
void MainGame::run()
{
    init();
    // Game loop
    while (_gameState != GameState::EXIT)
    {
        // Print out the board
        _board.print();
        // Say which player is playing
        if (_currentPlayer == X_VAL)
        {
            printf("  Player X's turn!\n\n");
        } else
        {
            printf("  Player O's turn!\n\n");
        }
        // Perform a move
        if (!_isMultiplayer && _currentPlayer == _aiPlayer)
        {
            aiMove();
        } else {
            playerMove();
        }
        // Check victory condition
        int rv = _board.checkVictory();
        if (rv != NO_VAL)
        {
            endGame(rv == TIE_VAL);
        } else {
            changePlayer();
            // printf is faster than cout
            // Add a bunch of blank space to "clear" the command prompt
            for (int i = 0; i < 3; i++)
            {
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            }
        }
    }
}

void MainGame::init()
{
    _gameState = GameState::PLAYING;
    _board.init(3);
    _currentPlayer = X_VAL;

    printf("\n\n****** Welcome to Tic-Tac-Toe ******\n\n");

    // Check for multiplayer
    printf("\nMultiplayer? (y/n):");
    char input = ' ';
    bool isValid;
    do {
        isValid = true;
        if (!(std::cin >> input))
        {
            std::cout << "Invalid input!";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            isValid = false;
        } else if (input == 'y' || input == 'Y')
        {
            _isMultiplayer = true;
        } else
        {
            _isMultiplayer = false;
        }
    } while (isValid == false);

    if (!_isMultiplayer)
    {
        // Get player
        printf("\nWould you like to be X or O:");
        do {
            isValid = true;
            if (!(std::cin >> input))
            {
                std::cout << "Invalid input!";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                isValid = false;
            } else if (input == 'x' || input == 'X')
            {
                _aiPlayer = O_VAL;
            } else if (input == 'o' || input == 'O')
            {
                _aiPlayer = X_VAL;
            } else {
                std::cout << "Must enter X or O!";
                isValid = false;
            }
        } while (isValid == false);
        // Init the AI player
        _ai.init(_aiPlayer);
    }
    printf("\n\n");
}

void MainGame::playerMove()
{
    // Get input
    bool wasValid = false;

    int x, y;
    do {
        printf("Enter the X: ");
        while (!(std::cin >> x))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printf("ERROR: Invalid input!");
        }
        printf("Enter the Y: ");
        while (!(std::cin >> y))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printf("ERROR: Invalid input!");
        }

        if (x < 1 || y < 1 || x > _board.getSize() || y > _board.getSize())
        {
            printf("ERROR: Invalid X or Y!\n");
        } else if (_board.getVal(x - 1, y - 1) != 0)
        {
            printf("ERROR: That spot is taken!\n");
        } else
        {
            wasValid = true;
        }

    } while (!wasValid);

    // Now place the token
    _board.setVal(x - 1, y - 1, _currentPlayer);
}

void MainGame::aiMove()
{
    _ai.performMove(_board);
}

void MainGame::changePlayer()
{
    if (_currentPlayer == X_VAL)
    {
        _currentPlayer = O_VAL;
    } else
    {
        _currentPlayer = X_VAL;
    }
}

void MainGame::endGame(bool wasTie)
{

    _board.print();

    if (wasTie)
    {
        printf("\n\n Tie game! Enter any key to play again, or Z to exit: ");
    } else
    {
        if (_currentPlayer == X_VAL)
        {
            printf("\n\n  Player X wins! Enter any key to play again, or Z to exit: ");
        } else
        {
            printf("\n\n  Player O wins! Enter any key to play again, or Z to exit: ");
        }
    }

    char input;
    std::cin >> input;
    if (input == 'Z' || input == 'z')
    {
        _gameState = GameState::EXIT;
    } else
    {
        // Re init the game
        init();
    }
}
int main()
{
     try
    {
       // Start the game
    MainGame mainGame;
    mainGame.run();


    }
    catch (std::exception const &exc)
    {
        std::cerr << "Exception caught " << exc.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught\n";
    }

    return 0;


}
// This marks the End
