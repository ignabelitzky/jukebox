#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QMediaMetaData>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include "playercontrols.h"
#include "config.h"

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
    QLineEdit *m_titleLineEdit = nullptr;

    QLabel *m_artistLabel = nullptr;
    QLineEdit *m_artistLineEdit = nullptr;

    QPushButton *m_openButton = nullptr;

    PlayerControls *m_controls = nullptr;

    QMediaMetaData m_metaData;

    void set_controls();
    void set_connections();
    void set_metadata_gui();
    void set_layouts_gui();
    void set_gui();
};

#endif // PLAYER_H
