#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

#define DEFAULT_VOLUME 5

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void play_music();
    void pause_music();
    void set_volume();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;

    QPushButton *play;
    QPushButton *previousMedia;
    QPushButton *nextMedia;
    QPushButton *pause;

    QSlider *volumeSlider;
    int volume;
};
#endif // MAINWINDOW_H
