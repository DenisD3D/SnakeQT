#include "network_manager.hpp"

FileDownloader::FileDownloader(const QUrl &url, QObject *parent) : QObject(parent), m_WebCtrl(this) {
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));

    const QNetworkRequest request(url);
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() {
}

void FileDownloader::fileDownloaded(QNetworkReply *pReply) {
    if (pReply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error: " << pReply->errorString();
    } else {
        m_DownloadedData = pReply->readAll();
        emit downloaded();
    }
    pReply->deleteLater();
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}
