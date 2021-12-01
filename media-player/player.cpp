#include "player.h"
#include "playercontrols.h"
#include <QFileDialog>
#include <QBoxLayout>
#include <QStandardPaths>
#include <QString>
#include <QTime>

Player::Player(QWidget *parent) : QWidget(parent)
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    m_titleLabel = new QLabel(tr("No file loaded."), this);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    QPushButton *openButton = new QPushButton(tr("Open"), this);

    connect(openButton, &QPushButton::clicked, this, &Player::open);

    PlayerControls *controls = new PlayerControls(this);
    controls->setState(m_player->playbackState());
    controls->setVolume(m_audioOutput->volume());
    controls->setMuted(controls->isMuted());

    connect(controls, &PlayerControls::play, m_player, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, m_player, &QMediaPlayer::stop);
    connect(controls, &PlayerControls::changeVolume, m_audioOutput, &QAudioOutput::setVolume);
    connect(controls, &PlayerControls::changeMuting, m_audioOutput, &QAudioOutput::setMuted);

    connect(m_player, &QMediaPlayer::playbackStateChanged, controls, &PlayerControls::setState);
    connect(m_audioOutput, &QAudioOutput::volumeChanged, controls, &PlayerControls::setVolume);
    connect(m_audioOutput, &QAudioOutput::mutedChanged, controls, &PlayerControls::setMuted);

    connect(m_player, &QMediaPlayer::metaDataChanged, this, &Player::setMetadata);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(openButton);
    controlLayout->addStretch(1);
    controlLayout->addWidget(controls);
    controlLayout->addStretch(1);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_titleLabel, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addLayout(controlLayout);
    mainLayout->addStretch(1);

    setWindowTitle(tr("Jukebox"));
    setLayout(mainLayout);
}

Player::~Player()
{
    delete m_player;
    delete m_audioOutput;
    delete m_titleLabel;
}

void Player::open()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open File"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));
    QUrl file = fileDialog.getOpenFileUrl();
    m_player->setSource(file);
    m_player->play();
}

void Player::setMetadata()
{
    // Set title
    m_metaData = m_player->metaData();
    m_titleLabel->setText(m_metaData.stringValue(QMediaMetaData::Title));
}
