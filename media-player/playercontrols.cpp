#include "playercontrols.h"
#include <QToolButton>
#include <QBoxLayout>

PlayerControls::PlayerControls(QWidget *parent) : QWidget(parent)
{
    m_playButton = new QToolButton(this);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked, this, &PlayerControls::playClicked);

    m_stopButton = new QToolButton(this);
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopButton->setEnabled(false);

    connect(m_stopButton, &QAbstractButton::clicked, this, &PlayerControls::stop);

    m_nextButton = new QToolButton(this);
    m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(m_nextButton, &QAbstractButton::clicked, this, &PlayerControls::next);

    m_previousButton = new QToolButton(this);
    m_previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(m_previousButton, &QAbstractButton::clicked, this, &PlayerControls::previous);

    m_muteButton = new QToolButton(this);
    m_muteButton->setIcon(style()->standardIcon((QStyle::SP_MediaVolume)));

    connect(m_muteButton, &QAbstractButton::clicked, this, &PlayerControls::muteClicked);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);

    connect(m_volumeSlider, &QSlider::valueChanged, this, &PlayerControls::onVolumeSliderValueChanged);

    QBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_playButton);
    layout->addWidget(m_stopButton);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_previousButton);
    layout->addWidget(m_muteButton);
    layout->addWidget(m_volumeSlider);
    setLayout(layout);
}

QMediaPlayer::PlaybackState PlayerControls::state() const
{
    return m_playerState;
}

float PlayerControls::volume() const
{
    float linearVolume = QAudio::convertVolume(m_volumeSlider->value() / float(100),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);
    return linearVolume;
}

bool PlayerControls::isMuted() const
{
    return m_playerMuted;
}

void PlayerControls::setState(QMediaPlayer::PlaybackState state)
{
    if(state != m_playerState) {
        m_playerState = state;

        switch(state) {
        case QMediaPlayer::StoppedState:
            m_stopButton->setEnabled(false);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

void PlayerControls::setVolume(float volume)
{
    float logarithmicVolume = QAudio::convertVolume(volume,
                                                    QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);
    m_volumeSlider->setValue(logarithmicVolume * 100);
}

void PlayerControls::setMuted(bool muted)
{
    if(muted != m_playerMuted) {
        m_playerMuted = muted;
        m_muteButton->setIcon(style()->standardIcon(muted?
                                  QStyle::SP_MediaVolumeMuted:
                                  QStyle::SP_MediaVolume));
    }
}

void PlayerControls::playClicked()
{
   switch(m_playerState) {
   case QMediaPlayer::StoppedState:
   case QMediaPlayer::PausedState:
       emit play();
       break;
   case QMediaPlayer::PlayingState:
       emit pause();
       break;
   }
}

void PlayerControls::muteClicked()
{
    emit changeMuting(!m_playerMuted);
}

void PlayerControls::onVolumeSliderValueChanged()
{
    emit changeVolume(volume());
}
