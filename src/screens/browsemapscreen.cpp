#include "browsemapscreen.hpp"


/**
 * Construct map browser screen.
 */
BrowseMapScreen::BrowseMapScreen(QWidget *parent): QWidget(parent) {
    const auto layout = new QVBoxLayout(this);

    const auto backButton = new QPushButton("Back", this);
    mapList = new QListWidget(this);
    loadButton = new QPushButton("Play", this);

    layout->addWidget(backButton);
    layout->addWidget(mapList);
    layout->addWidget(loadButton);

    connect(backButton, &QPushButton::clicked, this, &BrowseMapScreen::backClicked);
    connect(loadButton, &QPushButton::clicked, this, &BrowseMapScreen::loadClicked);
    connect(mapList, &QListWidget::itemSelectionChanged, this, &BrowseMapScreen::mapSelectionChanged);

    // Load maps
    QDirIterator it("maps", QDir::Files);
    QPixmap transparentPixmap(24, 24);
    transparentPixmap.fill(Qt::transparent);
    const QIcon spacerIcon(transparentPixmap);
    while (it.hasNext()) {
        it.next();
        const auto item = new QListWidgetItem(it.fileName());
        item->setIcon(spacerIcon);
        item->setData(Qt::UserRole, true);
        mapList->addItem(item);
    }

    loadOnlineMaps();
}

void BrowseMapScreen::loadOnlineMaps() {
    downloader = new FileDownloader(QUrl("https://snakeqt.denisd3d.fr/maps"), this);
    connect(downloader, &FileDownloader::downloaded, this, &BrowseMapScreen::onlineMapListDownloaded);
}

void BrowseMapScreen::backClicked() {
    emit back();
}

void BrowseMapScreen::loadClicked() {
    if (mapList->currentItem() != nullptr) {
        if (!mapList->currentItem()->data(Qt::UserRole).toBool()) {
            auto mapName = mapList->currentItem()->text().replace(".skm", "");
            downloader = new FileDownloader(QUrl("https://snakeqt.denisd3d.fr/maps/" + mapName),
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
