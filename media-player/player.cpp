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

    m_artistLabel = new QLabel(tr("Artist:"), this);
    m_artistLabel->setAlignment(Qt::AlignCenter);
    m_artistLabel->adjustSize();

    m_artistLineEdit = new QLineEdit(this);
    m_artistLineEdit->setAlignment(Qt::AlignCenter);
    m_artistLineEdit->setFixedSize(250, 25);
    m_artistLineEdit->setReadOnly(true);

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

    QBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(2, 2, 2, 2);
    titleLayout->addWidget(m_titleLabel, Qt::AlignCenter);
    titleLayout->addStretch(1);
    titleLayout->addWidget(m_titleLineEdit, Qt::AlignCenter);
    titleLayout->addStretch(1);

    QBoxLayout *artistLayout = new QHBoxLayout;
    artistLayout->setContentsMargins(2, 2, 2, 2);
    artistLayout->addWidget(m_artistLabel, Qt::AlignCenter);
    artistLayout->addStretch(1);
    artistLayout->addWidget(m_artistLineEdit, Qt::AlignCenter);
    artistLayout->addStretch(1);

    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->addLayout(titleLayout, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addLayout(artistLayout, Qt::AlignCenter);
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

    // Set artist
    m_artistLineEdit->setText(m_metaData.stringValue(QMediaMetaData::AlbumArtist));
    m_artistLineEdit->adjustSize();
}
