#ifndef EDITORSCREEN_HPP
#define EDITORSCREEN_HPP
#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

#include "../map.hpp"
#include "gamearea.hpp"


class CustomTableWidget final : public QTableWidget {
    Q_OBJECT

public:
    explicit CustomTableWidget(QWidget *parent = nullptr)
        : QTableWidget(parent) {
    }

signals:
    void iconDoubleClicked(QTableWidgetItem *item);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override {
        QTableWidgetItem *item = itemAt(event->pos());
        if (item) {
            QRect iconRect = visualItemRect(item);
            iconRect.setWidth(iconSize().width()); // consider only the icon area
            if (iconRect.contains(event->pos())) {
                emit iconDoubleClicked(item);
                return;
            }
        }

        QTableWidget::mouseDoubleClickEvent(event);
    }

    bool edit(const QModelIndex &index, const EditTrigger trigger, QEvent *event) override {
        const QTableWidgetItem *item = itemFromIndex(index);
        if (item && item->flags() & Qt::ItemIsEditable) {
            return QTableWidget::edit(index, trigger, event);
        }
        return false;
    }
};

class EditorScreen final : public QWidget {
    Q_OBJECT

    GameArea *gameArea = {};
    Map map;

    CustomTableWidget *tileTypeTable = new CustomTableWidget;
    QString lastEditedItemText;

public:
    explicit EditorScreen(const QString &file_info, bool create_map = false, QWidget *parent = nullptr);

public slots:
    void onTileSelectorIconDoubleClicked(QTableWidgetItem *item);

    void onTileSelectorItemDoubleClicked(QTableWidgetItem *item);

    void onTileSelectorItemChanged(QTableWidgetItem *item);

    void placeTileAtPosition(Position pos, bool is_right_click);
};
#endif //EDITORSCREEN_HPP
