#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class FileDownloader final : public QObject {
    Q_OBJECT

public:
    explicit FileDownloader(const QUrl &url, QObject *parent = nullptr);

    ~FileDownloader() override;

    QByteArray downloadedData() const;

signals:
    void downloaded();

private slots:
    void fileDownloaded(QNetworkReply *pReply);

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
};

class ScoreManager final : public QObject {
    Q_OBJECT

    QNetworkAccessManager *manager;

public:
    explicit ScoreManager(QObject *parent = nullptr);

    void upload_score(const QString &map_name, const QString &name, int score) const;

    void get_highscores(const QString &map_name) const;

signals:
    void highscoresReceived(const QList<QPair<QString, QVariant> > &highscores);

private slots:
    void onRequestFinished(QNetworkReply *reply);
};
#endif // NETWORK_MANAGER_H
