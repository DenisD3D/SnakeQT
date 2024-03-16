#include "editorscreen.hpp"


/**
 * Construct main game window
 */
EditorScreen::EditorScreen(const QString &file_info, const bool create_map, QWidget *parent): QWidget(parent) {
    map = Map(file_info, create_map);

    gameArea = new GameArea(map, nullptr, nullptr, true, this);

    auto *gameLayout = new QVBoxLayout;
    gameLayout->addWidget(gameArea);

    connect(gameArea, &GameArea::positionClicked, this, &EditorScreen::placeTileAtPosition);

    auto *leftLayout = new QVBoxLayout;
    auto *mapName = new QLineEdit(map.getName());
    auto *mapAuthor = new QLineEdit(map.getAuthor());
    auto *mapDescription = new QTextEdit(map.getDescription());
    auto *mapWidth = new QLineEdit(QString::number(map.getWidth()));
    mapWidth->setValidator(new QIntValidator(1, MAX_MAP_SIZE, this));
    auto *mapHeight = new QLineEdit(QString::number(map.getHeight()));
    mapHeight->setValidator(new QIntValidator(1, MAX_MAP_SIZE, this));
    auto *snakeInitX = new QLineEdit(QString::number(map.getInitX()));
    snakeInitX->setValidator(new QIntValidator(0, map.getWidth() - 1, this));
    auto *snakeInitY = new QLineEdit(QString::number(map.getInitY()));
    snakeInitY->setValidator(new QIntValidator(0, map.getHeight() - 1, this));
    auto *snakeInitLength = new QLineEdit(QString::number(map.getInitSnakeLength()));
    snakeInitLength->setValidator(new QIntValidator(1, MAX_MAP_SIZE, this));
    auto *snakeInitDirection = new QComboBox();
    snakeInitDirection->addItem("Left", GAUCHE);
    snakeInitDirection->addItem("Right", DROITE);
    snakeInitDirection->addItem("Up", HAUT);
    snakeInitDirection->addItem("Down", BAS);
    snakeInitDirection->setCurrentIndex(map.getInitDirection());
    auto *saveButton = new QPushButton("Save map");
    auto *saveAsButton = new QPushButton("Save map as");

    auto *mapNameLabel = new QLabel("Map Name");
    auto *mapAuthorLabel = new QLabel("Map Author");
    auto *mapDescriptionLabel = new QLabel("Map Description");
    auto *mapWidthLabel = new QLabel("Map Width");
    auto *mapHeightLabel = new QLabel("Map Height");
    auto *snakeInitXLabel = new QLabel("Snake Initial X");
    auto *snakeInitYLabel = new QLabel("Snake Initial Y");
    auto *snakeInitLengthLabel = new QLabel("Snake Initial Length");
    auto *snakeInitDirectionLabel = new QLabel("Snake Initial Direction");

    leftLayout->addWidget(mapNameLabel);
    leftLayout->addWidget(mapName);
    leftLayout->addWidget(mapAuthorLabel);
    leftLayout->addWidget(mapAuthor);
    leftLayout->addWidget(mapDescriptionLabel);
    leftLayout->addWidget(mapDescription);
    leftLayout->addWidget(mapWidthLabel);
    leftLayout->addWidget(mapWidth);
    leftLayout->addWidget(mapHeightLabel);
    leftLayout->addWidget(mapHeight);
    leftLayout->addWidget(snakeInitXLabel);
    leftLayout->addWidget(snakeInitX);
    leftLayout->addWidget(snakeInitYLabel);
    leftLayout->addWidget(snakeInitY);
    leftLayout->addWidget(snakeInitLengthLabel);
    leftLayout->addWidget(snakeInitLength);
    leftLayout->addWidget(snakeInitDirectionLabel);
    leftLayout->addWidget(snakeInitDirection);
    leftLayout->addWidget(saveButton);
    leftLayout->addWidget(saveAsButton);

    connect(mapName, &QLineEdit::editingFinished, [this, mapName] {
        map.setName(mapName->text());
    });
    connect(mapAuthor, &QLineEdit::editingFinished, [this, mapAuthor] {
        map.setAuthor(mapAuthor->text());
    });
    connect(mapDescription, &QTextEdit::textChanged, [this, mapDescription] {
        map.setDescription(mapDescription->toPlainText());
    });
    connect(mapWidth, &QLineEdit::editingFinished, [this, snakeInitX, mapWidth] {
        map.setWidth(mapWidth->text().toInt());
        snakeInitX->setValidator(new QIntValidator(0, map.getWidth() - 1, this));
        gameArea->resizeWidget();
        update();
    });
    connect(mapHeight, &QLineEdit::editingFinished, [this, snakeInitY, mapHeight] {
        map.setHeight(mapHeight->text().toInt());
        snakeInitY->setValidator(new QIntValidator(0, map.getHeight() - 1, this));
        gameArea->resizeWidget();
        update();
    });
    connect(snakeInitX, &QLineEdit::editingFinished, [this, snakeInitX] {
        map.setInitX(snakeInitX->text().toInt());
    });
    connect(snakeInitY, &QLineEdit::editingFinished, [this, snakeInitY] {
        map.setInitY(snakeInitY->text().toInt());
    });
    connect(snakeInitLength, &QLineEdit::editingFinished, [this, snakeInitLength] {
        map.setInitSnakeLength(snakeInitLength->text().toInt());
    });
    connect(snakeInitDirection, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, snakeInitDirection](const int index) {
                map.setInitDirection(static_cast<Direction>(snakeInitDirection->itemData(index).toInt()));
            });
    connect(saveButton, &QPushButton::clicked, [this] {
        map.save();
    });
    connect(saveAsButton, &QPushButton::clicked, [this] {
        auto fileName = QFileDialog::getSaveFileName(this, tr("New Map"), QString(), tr("Map Files (*.skm)"));
        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".skm"))
                fileName += ".skm";

            map.setFile(fileName);
            map.save();
        }
    });

    auto *rightLayout = new QVBoxLayout;
    // Add tile type input fields to the right layout
    auto *tileTypeLabel = new QLabel("Tile Types");

    tileTypeTable->setColumnCount(1);
    tileTypeTable->setRowCount(map.getTypes().size());
    tileTypeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);;
    tileTypeTable->horizontalHeader()->hide();
    tileTypeTable->verticalHeader()->hide();
    tileTypeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    tileTypeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tileTypeTable->sortItems(0, Qt::AscendingOrder);

    const int rowHeight = tileTypeTable->rowHeight(0);
    tileTypeTable->setIconSize(QSize(rowHeight, rowHeight));

    QMap<QString, TileType> types = map.getTypes();
    int i = 0;
    for (auto it = types.end() - 1; it != types.begin() - 1; --it) {
        auto *item = new QTableWidgetItem(it.key());
        item->setIcon(QIcon(it.value().texture));

        if (it.value().is_default) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }

        tileTypeTable->setItem(i, 0, item);
        i++;
    }
    tileTypeTable->selectRow(0);

    connect(tileTypeTable, &CustomTableWidget::iconDoubleClicked, this, &EditorScreen::onTileSelectorIconDoubleClicked);
    connect(tileTypeTable, &QTableWidget::itemDoubleClicked, this, &EditorScreen::onTileSelectorItemDoubleClicked);
    connect(tileTypeTable, &QTableWidget::itemChanged, this, &EditorScreen::onTileSelectorItemChanged);


    rightLayout->addWidget(tileTypeLabel);
    rightLayout->addWidget(tileTypeTable);


    auto *hLayout = new QHBoxLayout(this);
    hLayout->addLayout(leftLayout);
    hLayout->addLayout(gameLayout);
    hLayout->addLayout(rightLayout);
}

void EditorScreen::onTileSelectorIconDoubleClicked(QTableWidgetItem *item) {
    if (item->flags() & Qt::ItemIsEditable) {
        // Get a new texture for the tile type
        const QString texturePath = QFileDialog::getOpenFileName(this, "Open Image", "",
                                                                 "Image Files (*.png *.jpg *.bmp)");
        if (texturePath.isEmpty())
            return;

        const QPixmap newTexture(texturePath);
        if (newTexture.isNull())
            return;

        // Update the texture in the QMap
        map.setTypeTexture(item->text(), newTexture);
        // Update the icon in the table
        item->setIcon(QIcon(newTexture));
        // Redraw the game area
        gameArea->update();
    }
}

void EditorScreen::placeTileAtPosition(const Position pos, const bool is_right_click) {
    QString selectedItemValue = nullptr;
    if (is_right_click) {
        selectedItemValue = map.getDefaultTile();
    } else {
        if (tileTypeTable->currentItem() == nullptr)
            return;

        selectedItemValue = tileTypeTable->currentItem()->text();
        if (!map.getTypes().contains(selectedItemValue))
            return;
    }
    map.setTileAt(pos, selectedItemValue);
    gameArea->update();
}

void EditorScreen::onTileSelectorItemDoubleClicked(QTableWidgetItem *item) {
    lastEditedItemText = item->text();
}

void EditorScreen::onTileSelectorItemChanged(QTableWidgetItem *item) {
    if (lastEditedItemText.isEmpty())
        return;

    const QString newText = item->text();

    map.setNewTypeName(lastEditedItemText, newText);

    lastEditedItemText.clear();
}
