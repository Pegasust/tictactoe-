/**
 * A definition for a game of Tic Tac Toe where a human plays against the computer
 *
* Created by Rick Mercer on 2019-03-07.
* Refactored 25-April-2022
*/
#ifndef TTT_GAME_H
#define TTT_GAME_H

#include <vector>
#include <string>
using namespace std;

class Game {
private:
  vector<vector<char>> board;
  int size;

public:
  Game();

  // Use this to store a row and column
  struct square {
    int row;
    int col;
  };

  string toString();

  square computerMove();

  void initializeBoard();

  void humanMove(int row, int col);

  bool squareAvailable(int r, int c);

  bool stillRunning();

  bool didWin(char playerChar);

  bool tied();

  int maxMovesRemaining();

private:

  bool wonByRow(char playerChar);

  bool wonByCol(char playerChar);

  bool wonByDiagonal(char playerChar);
};

#endif //TTT_GAME_H
