#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    volume = DEFAULT_VOLUME;

    // Buttons
    play = MainWindow::findChild<QPushButton *>(QStringLiteral("play_btn"));
    nextMedia = MainWindow::findChild<QPushButton *>(QStringLiteral("nextMedia_btn"));
    previousMedia = MainWindow::findChild<QPushButton *>(QStringLiteral("previousMedia_btn"));
    pause = MainWindow::findChild<QPushButton *>(QStringLiteral("pause_btn"));

    // Sliders
    volumeSlider = MainWindow::findChild<QSlider *>(QStringLiteral("volume_slider"));

    volumeSlider->setValue(volume);

    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;

    player->setAudioOutput(audioOutput);

    player->setSource(QUrl::fromLocalFile("/home/ignacio/Music/I feel good.mp3"));
    audioOutput->setVolume(volume / 100.0);

    connect(play, SIGNAL(clicked(bool)), this, SLOT(play_music()));
    connect(pause, SIGNAL(clicked(bool)), this, SLOT(pause_music()));
    connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(set_volume()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    delete audioOutput;
}

void MainWindow::play_music()
{
    player->play();
}

void MainWindow::pause_music()
{
    player->pause();
}

void MainWindow::set_volume()
{
    float volume = volumeSlider->value() / 100.0;
    audioOutput->setVolume(volume);
}

