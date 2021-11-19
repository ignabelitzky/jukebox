#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>
#include <QAbstractButton>
#include <QAbstractSlider>
#include <QComboBox>

class PlayerControls : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerControls(QWidget *parent = nullptr);

    QMediaPlayer::PlaybackState state() const;
    float volume() const;
    bool isMuted() const;

public slots:
    void setState(QMediaPlayer::PlaybackState state);
    void setVolume(float volume);
    void setMuted(bool muted);

signals:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(float volume);
    void changeMuting(bool muting);

private slots:
    void playClicked();
    void muteClicked();
    void onVolumeSliderValueChanged();

private:
    QMediaPlayer::PlaybackState m_playerState = QMediaPlayer::StoppedState;
    bool m_playerMuted = false;
    QAbstractButton *m_playButton = nullptr;
    QAbstractButton *m_stopButton = nullptr;
    QAbstractButton *m_nextButton = nullptr;
    QAbstractButton *m_previousButton = nullptr;
    QAbstractButton *m_muteButton = nullptr;
    QAbstractSlider *m_volumeSlider = nullptr;

};

#endif // PLAYERCONTROLS_H
