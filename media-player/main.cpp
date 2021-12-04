#include "player.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Jukebox");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    Player player;
    player.show();
    return app.exec();
}
