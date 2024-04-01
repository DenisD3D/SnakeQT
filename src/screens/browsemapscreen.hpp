#ifndef BROWSEMAPSCREEN_HPP
#define BROWSEMAPSCREEN_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

#include "../network_manager.hpp"

class BrowseMapScreen final : public QWidget {
    Q_OBJECT
    QListWidget *mapList;
    QPushButton *loadButton;
    FileDownloader *downloader;

    void loadOnlineMaps();

    void updateMapList();

public:
    explicit BrowseMapScreen(QWidget *parent = nullptr);

public slots:
    void backClicked();

    void loadClicked();

    void onlineMapListDownloaded();

    void mapSelectionChanged();

signals:
    void back();

    void load(const QString &mapName);
};

#endif //BROWSEMAPSCREEN_HPP
