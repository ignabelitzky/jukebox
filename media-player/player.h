#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QMediaMetaData>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
    ~Player();

signals:

private slots:
    void open();
    void setMetadata();

private:
    QMediaPlayer *m_player = nullptr;
    QAudioOutput *m_audioOutput = nullptr;
    QLabel *m_titleLabel = nullptr;
    QMediaMetaData m_metaData;
    QString title;
};

#endif // PLAYER_H
