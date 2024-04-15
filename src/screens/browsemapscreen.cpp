#include "browsemapscreen.hpp"


/**
 * Construct map browser screen.
 */
BrowseMapScreen::BrowseMapScreen(QWidget *parent): QWidget(parent) {
    // Load custom font
    const int id = QFontDatabase::addApplicationFont(":/images/game_played.otf");
    const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont snakefont(family);
    snakefont.setPointSize(20);

    const auto layoutV = new QVBoxLayout(this);
    const auto layoutH = new QHBoxLayout;

    const auto helpText = new QLabel("Select a map to play, maps with a cloud icon are stored online and will be downloaded:", this);

    const auto backButton = new QPushButton("Main Menu", this);
    backButton->setFont(snakefont);
    backButton->setStyleSheet("padding-left: 20px; padding-right: 20px;");
    backButton->setFixedSize(200, 50);

    const auto openMapFolderButton = new QPushButton("Open map folder", this);
    openMapFolderButton->setFont(snakefont);
    openMapFolderButton->setStyleSheet("font-size: 17px;");
    openMapFolderButton->setFixedSize(200, 50);

    loadButton = new QPushButton("Play", this);
    loadButton->setFont(snakefont);
    loadButton->setStyleSheet("padding-left: 20px; padding-right: 20px;");
    loadButton->setFixedSize(200, 50);

    mapList = new QListWidget(this);

    layoutH->addWidget(backButton);
    layoutH->addStretch();
    layoutH->addWidget(openMapFolderButton);
    layoutH->addStretch(2);
    layoutH->addWidget(loadButton);

    layoutV->addWidget(helpText);
    layoutV->addWidget(mapList);
    layoutV->addLayout(layoutH);


    connect(backButton, &QPushButton::clicked, this, &BrowseMapScreen::backClicked);
    connect(loadButton, &QPushButton::clicked, this, &BrowseMapScreen::loadClicked);
    connect(mapList, &QListWidget::itemSelectionChanged, this, &BrowseMapScreen::mapSelectionChanged);
    connect(openMapFolderButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo("maps").absoluteFilePath()));
    });

    // Load local maps list
    QDirIterator it("maps", QDir::Files);
    const QIcon driveIcon(":/images/drive.png");
    while (it.hasNext()) {
        it.next();
        const auto item = new QListWidgetItem(it.fileName());
        item->setIcon(driveIcon);
        item->setData(Qt::UserRole, true);
        mapList->addItem(item);
    }

    // Load online maps list
    loadOnlineMaps();
}

void BrowseMapScreen::loadOnlineMaps() {
    downloader = new FileDownloader(QUrl("http://snakeqt.denisd3d.fr/maps"), this);
    connect(downloader, &FileDownloader::downloaded, this, &BrowseMapScreen::onlineMapListDownloaded);
}

void BrowseMapScreen::backClicked() {
    emit back();
}

void BrowseMapScreen::loadClicked() {
    if (mapList->currentItem() != nullptr) {
        if (!mapList->currentItem()->data(Qt::UserRole).toBool()) {
            auto mapName = mapList->currentItem()->text().replace(".skm", "");
            downloader = new FileDownloader(QUrl("http://snakeqt.denisd3d.fr/maps/" + mapName),
                                            this);
            connect(downloader, &FileDownloader::downloaded, [this, mapName]() {
                const QDir dir("maps");
                if (!dir.exists()) {
                    dir.mkpath(".");
                }
                QFile file("maps/" + mapName + ".skm");
                file.open(QIODevice::WriteOnly);
                file.write(downloader->downloadedData());
                file.close();
                emit load("maps/" + mapName + ".skm");
            });
        } else {
            emit load("maps/" + mapList->currentItem()->text());
        }
    }
}

void BrowseMapScreen::mapSelectionChanged() {
    if (mapList->currentItem() != nullptr) {
        loadButton->setText(mapList->currentItem()->data(Qt::UserRole).toBool() ? "Play" : "Download and Play");
        loadButton->setStyleSheet(mapList->currentItem()->data(Qt::UserRole).toBool() ? "font-size: 20px;" : "font-size: 17px;");
    }
}

void BrowseMapScreen::onlineMapListDownloaded() {
    const QByteArray data = downloader->downloadedData();
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    const QIcon cloudIcon(":/images/cloud.png");
    QJsonArray array = doc.array();
    for (const auto &value: array) {
        QString mapName = value.toObject().value("id").toString();
        bool found = false;
        for (int i = 0; i < mapList->count(); i++) {
            if (mapList->item(i)->text() == mapName) {
                found = true;
                break;
            }
        }

        if (!found) {
            const auto item = new QListWidgetItem(mapName);
            item->setData(Qt::UserRole, false);
            item->setIcon(cloudIcon);
            mapList->addItem(item);
        }
    }
}
