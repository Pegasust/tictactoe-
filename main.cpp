#include <QApplication>
#include <QWidget>
#include <iostream>
// #include "main_window.hpp"
#include "TTTGUI.hpp"

int main(int argc, char *argv[]) {
     QApplication a(argc, argv);
     TTTGUI window;
     // window.show();
     return QApplication::exec();
}