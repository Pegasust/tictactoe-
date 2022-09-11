#include <string>
#include <vector>
#include "Game.h"
using namespace std;

/**
 * Construct a Tic Toe Game where one human user can play against the Computer's AI.
 *
 */
Game::Game () {
  size = 3;
  initializeBoard();
}

/**
 * Called from constructor and startNewGame().
 */
void Game::initializeBoard () {
  board.clear();
  for (int r = 0; r < size; r++) {
    vector<char> row{'_', '_', '_'};
    board.push_back(row);
  }
}

/**
* Select the given move and toggle between X and O player.
* Precondition row and col are both 0, 1 or  2
*
* @param row The row the player moves to
* @param col The column the player moves to column
* @param testing Passed the value of true during testing
*/
void Game::humanMove (int row, int col) {
    board[row][col] = 'O';
}


/**
 * Let the computer player chose the last available square. This is a very weak strategy.
 * Consider having the computer look for its own win and if not found, and look for a block.
 * Currently a loop an available square, one that has not been selected using random row and column.
 * A board[row][col] that equals '_' insteadof 'X' or 'O'
 */
Game::square Game::computerMove () {
  Game::square result;
  // initialize random seed:
  srand (time(NULL));
  int times = 0;
  while(true) {
    // generate a random integer 0, 1, or 2
    int r = rand() % 3;
    int c = rand() % 3;
    times++;
    if(board[r][c] == '_' ) {
      result.row = r;
      result.col = c;
      board[r][c] = 'X';
      break;
    }
  }
  return result;
}


/**
 * Provide a textual version of this tic tac toe board to help debug.
 *
 * @return The current state of this tic tac board showing 'X', 'O', or '_' in each square
 */
string Game::toString() {
  string result;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      result = result + board[r][c] + " ";
    }
    result += "\n";
  }
  return result;
}


/**
 * Since we have access to the selections '_' or 'O' or 'X' in the 2D array,
 * we need to know if the human player has clicked an already selected square
 * @return The state of this one square.
 */
bool Game::squareAvailable(int row, int column) {
  return board[row][column] == '_';
}


/**
 * Let users know if a game ended in a tie
 *
 * @return True if the current state of this game is tied
 */
bool Game::tied () {
  return maxMovesRemaining() == 0 && !didWin('X') &&
         !didWin('O');
}


/**
 * Let a strategy know how many moves can still be.
 * I found I needed this when implementing a strategy and for testing.
 * It's not really needed to build the game
 *
 * @return How many locations have not been taken.
 */
int Game::maxMovesRemaining () {
  int result = 0;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (board[r][c] == '_')
        result++;
    }
  }
  return result;
}


/**
  * Let others know the game is not yet over
  */
bool Game::stillRunning () {
  return !tied() and !didWin('X') and !didWin('O');
}


/**
 * Decide if either of the two players have won.
 *
 * return True if the playerChar ('O' or 'X' for computer) wins
 */
bool Game::didWin (char playerChar) {
  return wonByRow(playerChar) ||
         wonByCol(playerChar) ||
         wonByDiagonal(playerChar);
}

// Helper functions

bool Game::wonByRow (char playerChar) {
  for (int r = 0; r < size; r++) {
    int rowSum = 0;
    for (int c = 0; c < size; c++)
      if (board[r][c] == playerChar)
        rowSum++;
    if (rowSum == size)
      return true;
  }
  return false;
}

bool Game::wonByCol (char playerChar) {
  for (int c = 0; c < size; c++) {
    int colSum = 0;
    for (int r = 0; r < size; r++)
      if (board[r][c] == playerChar)
        colSum++;
    if (colSum == size)
      return true;
  }
  return false;
}

bool Game::wonByDiagonal (char playerChar) {
  // Check Diagonal from upper left to lower right
  int sum = 0;
  for (int r = 0; r < size; r++)
    if (board[r][r] == playerChar)
      sum++;
  if (sum == size)
    return true;

  // Check Diagonal from upper right to lower left
  sum = 0;
  for (int r = size - 1; r >= 0; r--)
    if (board[size - r - 1][r] == playerChar)
      sum++;
  if (sum == size)
    return true;

  // No win on either diagonal
  return false;
}