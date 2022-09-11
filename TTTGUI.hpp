#pragma once
#include <array>

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QIcon>
#include <QMessageBox>
#include <QPointer>
#include <QString>

#include <QtGlobal>
#include <QtDebug>
#include <iostream>

#include "Game.h"


/**
 * @brief Equivalent to `ret = QPointer<T>(new T(...));`
 * This less typing by forcing class template type deduction
 * 
 * @tparam Ret_T 
 * @tparam Forward_T 
 * @param ret 
 * @param forwarded_args 
 */
template<typename Ret_T, typename... Forward_T>
QPointer<Ret_T>& assign(QPointer<Ret_T>& ret, Forward_T&&... forwarded_args) {
    ret = new Ret_T(std::forward<Forward_T>(forwarded_args)...);
    return ret;
}
template<typename Iter_T, typename... Forward_T>
void assign(Iter_T begin, Iter_T end, Forward_T&&... forwarded_args) {
    for(auto itr = begin; itr != end; ++itr) {
        assign(*itr, std::forward<Forward_T>(forwarded_args)...);
    }
}

// I use a resource folder with custom path definition in CMakeLists.txt
// ${CMAKE_BINARY_DIR}/resources/ // the trailing '/' is necessary.
#ifndef RES_FOLDER
#define RES_FOLDER ""
#endif
#define RESOURCE_DIR(file_str) RES_FOLDER file_str


enum class TileGameState: int {EMPTY=0, HUMAN=1, COMPUTER=2};
enum class GameState: int{TIE=0, PLAYER_WIN=1, COMPUTER_WIN=2};
template<typename... Config_T>
constexpr const char* tile_path(TileGameState state, Config_T&&... conf) {
    constexpr const char* memo[] = {
        nullptr,
        RESOURCE_DIR("O.png"),
        RESOURCE_DIR("X.png")
    };
    return memo[static_cast<int>(state)];
}
// can override for different configurations like this
// template<>
// constexpr std::string tile_path(TileGameState state, int conf);

class TTTGameModel: public Game{};

class TTT_Tile: public QPushButton {
    Q_OBJECT;
private:
    QIcon icon;
    Game::square my_position;
public:
    TTT_Tile(QWidget* parent=0): QPushButton(parent)
    {
        on_state_transition(TileGameState::EMPTY);
        setMinimumSize(50,50);
    }
    template<typename... square_Construct>
    void set_position(square_Construct&&... fwd) {
        my_position = Game::square{std::forward<square_Construct>(fwd)...};
    }
    void on_state_transition(TileGameState next) {
        auto path = tile_path(next);
        qDebug().nospace() << "path: " << path <<"\n";
        icon = QIcon(path);
        this->setIcon(icon);
    }
    Game::square& position() {return my_position;}


    // try to maintain aspect ratio
    bool hasHeightForWidth() const {return true;}
    int heightForWidth(int w) const {return w;}
};

class TTTGUI: public QWidget {
    Q_OBJECT;
private:
    TTTGameModel model;
    std::array<QPointer<TTT_Tile>, 9> game_buttons;
    QPointer<QGridLayout> layout;
public:
    TTTGUI(): model(), game_buttons() {
        init();
        layoutGUI();
        add_handlers();
    }
    void init() {
        assign(layout);
        assign(game_buttons.begin(), game_buttons.end());
    }
    void layoutGUI() {
        for(int i = 0; i < game_buttons.size(); ++i) {
            int row = i / 3;
            int col = i % 3;
            layout->addWidget(game_buttons[i], row, col, Qt::AlignCenter);
            game_buttons[i]->set_position(row, col);
        }
        layout->setHorizontalSpacing(0);
        layout->setVerticalSpacing(0);
        this->setLayout(layout);
        this->setWindowTitle("TicTacToe");
        this->show();
    }
    void on_game_end(GameState state) {
        QMessageBox msg;
        switch(state) {
            case GameState::TIE:
                msg.setText("Game tie!");
                break;
            case GameState::PLAYER_WIN:
                msg.setText("Human won!");
                break;
            case GameState::COMPUTER_WIN:
                msg.setText("Computer won!");
                break;
            default:
                qDebug().nospace() << "Should not end up at default case\n";
                exit(1);
        }
        msg.exec();
    }
    void add_handlers() {
        for(int i = 0; i < game_buttons.size(); ++i) {
            int row = i / 3;
            int col = i % 3;
            QObject::connect(game_buttons[i], &TTT_Tile::clicked, this, [=]() {
                if(!model.stillRunning()) {
                    qDebug().nospace() << "User click but game already done\n";
                    return;
                }
                // state change?
                auto& btn = *game_buttons[i];
                auto& pos = btn.position();
                if(!model.squareAvailable(pos.row, pos.col)) {
                    QMessageBox msg;
                    msg.setText("Space not available!");
                    msg.exec();
                    return;
                }
                // ok, state change.
                btn.on_state_transition(TileGameState::HUMAN);
                model.humanMove(pos.row, pos.col);
                
                // game end handling
                if(!model.stillRunning()) {
                    on_game_end(model.tied()? GameState::TIE: model.didWin('O')? GameState::PLAYER_WIN: GameState::COMPUTER_WIN);
                    return;
                }
                
                // computer move
                auto comp_sqr = model.computerMove();
                auto& comp_btn = *game_buttons[comp_sqr.row * 3 + comp_sqr.col];
                comp_btn.on_state_transition(TileGameState::COMPUTER);

                // game end handling
                if(!model.stillRunning()) {
                    on_game_end(model.tied()? GameState::TIE: model.didWin('O')? GameState::PLAYER_WIN: GameState::COMPUTER_WIN);
                    return;
                }
            });
        }
    }
};