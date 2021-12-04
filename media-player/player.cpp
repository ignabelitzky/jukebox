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

    m_titleLabel = new QLabel(tr("Title:"), this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->adjustSize();

    m_titleLineEdit = new QLineEdit(this);
    m_titleLineEdit->setAlignment(Qt::AlignCenter);
    m_titleLineEdit->setFixedSize(250,25);
    m_titleLineEdit->setReadOnly(true);

    QPushButton *openButton = new QPushButton(tr("Open File"), this);

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
    controlLayout->setContentsMargins(5, 5, 5, 5);
    controlLayout->addWidget(openButton);
    controlLayout->addStretch(1);
    controlLayout->addWidget(controls, Qt::AlignCenter);
    controlLayout->addStretch(1);

    QBoxLayout *metadataLayout = new QHBoxLayout;
    metadataLayout->setContentsMargins(5, 5, 5, 5);
    metadataLayout->addWidget(m_titleLabel, Qt::AlignCenter);
    metadataLayout->addStretch(1);
    metadataLayout->addWidget(m_titleLineEdit, Qt::AlignCenter);
    metadataLayout->addStretch(1);

    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->addLayout(metadataLayout, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addLayout(controlLayout);
    mainLayout->addStretch(1);

    setWindowTitle(tr("Jukebox"));
    setLayout(mainLayout);

    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

Player::~Player()
{
    delete m_player;
    delete m_audioOutput;
    delete m_titleLabel;
    delete m_titleLineEdit;
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
    m_titleLineEdit->setText(m_metaData.stringValue(QMediaMetaData::Title));
    m_titleLineEdit->adjustSize();
}
