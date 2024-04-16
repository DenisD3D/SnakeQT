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

ScoreManager::ScoreManager(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this); // Initialize the manager in the constructor

    connect(manager, &QNetworkAccessManager::finished, this, &ScoreManager::onRequestFinished);
}

void ScoreManager::upload_score(const QString &map_name, const QString &name, int score) const {
    QNetworkRequest request(
        QUrl("http://snakeqt.denisd3d.fr/highscores/" + map_name + "/" + name + "/" + QString::number(score)));
    request.setRawHeader("X-App-Name", "SnakeQt");

    manager->get(request);
}

void ScoreManager::get_highscores(const QString &map_name) const {
    QNetworkRequest request(QUrl("http://snakeqt.denisd3d.fr/highscores/" + map_name));

    manager->get(request);

}

void ScoreManager::onRequestFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error: " << reply->errorString();
        return;
    }

    if (!reply->request().hasRawHeader("X-App-Name")) {
        const auto responseData = reply->readAll();
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        auto l = QList<QPair<QString, QVariant> >();
        const QJsonObject obj = jsonDoc.object();
        for (const auto &key: obj.keys()) {
            l.append(QPair(key, obj[key].toInt()));
        }

        std::sort(l.begin(), l.end(), [](const QPair<QString, QVariant>& a, const QPair<QString, QVariant>& b) {
            return a.second.toInt() > b.second.toInt();
        });

        emit highscoresReceived(l);
    }
}