#include <QApplication>
#include <QPushButton>
#include "Cards.hpp"

int main(int argc, char *argv[])
{
    Deck Deck[52];
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
