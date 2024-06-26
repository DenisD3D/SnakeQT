#include "editorscreen.hpp"


/**
 * Construct main game window
 */
EditorScreen::EditorScreen(const QString &file_info, const bool create_map, QWidget *parent): QWidget(parent) {
    map = Map(file_info, create_map);

    gameArea = new GameArea(map, nullptr, nullptr, nullptr, true, this);

    auto *gameLayout = new QVBoxLayout;
    gameLayout->addWidget(gameArea);

    connect(gameArea, &GameArea::positionClicked, this, &EditorScreen::placeTileAtPosition);

    auto *leftLayout = new QVBoxLayout;
    auto *mapName = new QLineEdit(map.getName());
    mapName->setMinimumWidth(150);
    auto *mapAuthor = new QLineEdit(map.getAuthor());
    auto *mapWidth = new ValidatingLineEdit(QString::number(map.getWidth()));
    mapWidth->setValidator(new QIntValidator(1, MAX_MAP_SIZE, this));
    auto *mapHeight = new ValidatingLineEdit(QString::number(map.getHeight()));
    mapHeight->setValidator(new QIntValidator(1, MAX_MAP_SIZE, this));
    auto *snakeInitX = new ValidatingLineEdit(QString::number(map.getInitX()));
    snakeInitX->setValidator(new QIntValidator(0, map.getWidth() - 1, this));
    auto *snakeInitY = new ValidatingLineEdit(QString::number(map.getInitY()));
    snakeInitY->setValidator(new QIntValidator(0, map.getHeight() - 1, this));
    auto *snakeInitLength = new ValidatingLineEdit(QString::number(map.getInitSnakeLength()));
    snakeInitLength->setValidator(new QIntValidator(1, MAX_MAP_SIZE, this));
    auto *snakeInitDirection = new QComboBox();
    snakeInitDirection->addItem("Left", GAUCHE);
    snakeInitDirection->addItem("Right", DROITE);
    snakeInitDirection->addItem("Up", HAUT);
    snakeInitDirection->addItem("Down", BAS);
    snakeInitDirection->setCurrentIndex(map.getInitDirection());
    auto *saveButton = new QPushButton("Save map");
    auto *saveAsButton = new QPushButton("Save map as");
    auto *backButton = new QPushButton("Main Menu");

    auto *mapNameLabel = new QLabel("Map Name");
    auto *mapAuthorLabel = new QLabel("Map Author");
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
    leftLayout->addWidget(backButton);

    connect(mapName, &QLineEdit::editingFinished, [this, mapName] {
        map.setName(mapName->text());
    });
    connect(mapAuthor, &QLineEdit::editingFinished, [this, mapAuthor] {
        map.setAuthor(mapAuthor->text());
    });
    connect(mapWidth, &QLineEdit::editingFinished, [this, snakeInitX, mapWidth] {
        map.setWidth(mapWidth->text().toInt());
        snakeInitX->setValidator(new QIntValidator(0, map.getWidth() - 1, this));
        gameArea->resizeWidget();
        update();
    });
    connect(mapWidth, &ValidatingLineEdit::invalidInput, [this, mapWidth] {
        QMessageBox::warning(this, "Invalid Input",
                             "Please enter a valid map width between 1 and " + QString::number(MAX_MAP_SIZE) + ".");
        mapWidth->setFocus();
    });
    connect(mapHeight, &QLineEdit::editingFinished, [this, snakeInitY, mapHeight] {
        map.setHeight(mapHeight->text().toInt());
        snakeInitY->setValidator(new QIntValidator(0, map.getHeight() - 1, this));
        gameArea->resizeWidget();
        update();
    });
    connect(mapHeight, &QLineEdit::inputRejected, [this, mapHeight] {
        QMessageBox::warning(this, "Invalid Input",
                             "Please enter a valid map height between 1 and " + QString::number(MAX_MAP_SIZE) + ".");
        mapHeight->setFocus();
    });
    connect(snakeInitX, &QLineEdit::editingFinished, [this, snakeInitX] {
        map.setInitX(snakeInitX->text().toInt());
    });
    connect(snakeInitX, &ValidatingLineEdit::invalidInput, [this, snakeInitX] {
        QMessageBox::warning(this, "Invalid Input",
                             "Please enter a valid snake initial X between 0 and " + QString::number(map.getWidth() - 1)
                             + ".");
        snakeInitX->setFocus();
    });
    connect(snakeInitY, &QLineEdit::editingFinished, [this, snakeInitY] {
        map.setInitY(snakeInitY->text().toInt());
    });
    connect(snakeInitY, &ValidatingLineEdit::invalidInput, [this, snakeInitY] {
        QMessageBox::warning(this, "Invalid Input",
                             "Please enter a valid snake initial Y between 0 and " + QString::number(
                                 map.getHeight() - 1)
                             + ".");
        snakeInitY->setFocus();
    });
    connect(snakeInitLength, &QLineEdit::editingFinished, [this, snakeInitLength] {
        map.setInitSnakeLength(snakeInitLength->text().toInt());
    });
    connect(snakeInitLength, &ValidatingLineEdit::invalidInput, [this, snakeInitLength] {
        QMessageBox::warning(this, "Invalid Input",
                             "Please enter a valid snake initial length greater or equal to 1.");
        snakeInitLength->setFocus();
    });
    connect(snakeInitDirection, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, snakeInitDirection](const int index) {
                map.setInitDirection(static_cast<Direction>(snakeInitDirection->itemData(index).toInt()));
            });
    connect(saveButton, &QPushButton::clicked, [this, mapName, mapAuthor] {
        if (mapName->text().isEmpty() || mapAuthor->text().isEmpty()) {
            QMessageBox::warning(this, "Cannot Save", "Please fill in both the name and author fields before saving.");
            return;
        }

        map.save();
    });
    connect(saveAsButton, &QPushButton::clicked, [this, mapName, mapAuthor] {
        if (mapName->text().isEmpty() || mapAuthor->text().isEmpty()) {
            QMessageBox::warning(this, "Cannot Save", "Please fill in both the name and author fields before saving.");
            return;
        }

        auto fileName = QFileDialog::getSaveFileName(this, tr("New Map"), QString(), tr("Map Files (*.skm)"));
        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".skm"))
                fileName += ".skm";

            map.setFile(fileName);
            map.save();
        }
    });

    connect(backButton, &QPushButton::clicked, [this] {
        emit back();
    });

    auto *rightLayout = new QVBoxLayout;

    auto *tileTypeLabel = new QLabel("Tile Types");

    tileTypeTable->setColumnCount(1);
    tileTypeTable->setRowCount(map.getTypes().size());
    tileTypeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);;
    tileTypeTable->horizontalHeader()->hide();
    tileTypeTable->verticalHeader()->hide();
    tileTypeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    tileTypeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tileTypeTable->sortItems(0, Qt::AscendingOrder);
    tileTypeTable->setContextMenuPolicy(Qt::CustomContextMenu);
    tileTypeTable->setMinimumWidth(150);
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
    connect(tileTypeTable, &QTableWidget::customContextMenuRequested, this, &EditorScreen::showTileSelectorContextMenu);

    auto *addTileType = new QPushButton("Add tile type");

    connect(addTileType, &QPushButton::clicked, [this] {
        QString name = "custom_tile";
        int j = 1;
        // Ensure the name is unique
        while (map.getTypes().contains(name)) {
            // If it does, append a number to the newType name
            name = QString("custom_tile_%1").arg(j);
            j++;
        }

        map.createType(name);

        const int row = tileTypeTable->rowCount();
        tileTypeTable->insertRow(row);

        auto *item = new QTableWidgetItem(name);
        item->setIcon(QIcon(map.getTypes()[name].texture));
        tileTypeTable->setItem(row, 0, item);
        tileTypeTable->selectRow(row);

        defaulTypeSelect->addItem(name);
    });

    auto *customTextureLabel = new QLabel("Custom Textures");
    auto *customTextureTable = new QTableWidget(this);
    customTextureTable->setColumnCount(1);
    customTextureTable->setRowCount(3);
    customTextureTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    customTextureTable->horizontalHeader()->hide();
    customTextureTable->verticalHeader()->hide();
    customTextureTable->setSelectionMode(QAbstractItemView::NoSelection);
    customTextureTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    customTextureTable->setContextMenuPolicy(Qt::CustomContextMenu);
    customTextureTable->setFocusPolicy(Qt::NoFocus);
    customTextureTable->setMinimumWidth(150);
    customTextureTable->setMaximumHeight(150);
    const int rowHeight2 = customTextureTable->rowHeight(0);
    customTextureTable->setIconSize(QSize(rowHeight2, rowHeight2));

    auto *snakeHeadTexture = new QTableWidgetItem("Snake Head");
    snakeHeadTexture->setIcon(QIcon(map.getSnakeHeadTexture()));
    customTextureTable->setItem(0, 0, snakeHeadTexture);

    auto *snakeBodyTexture = new QTableWidgetItem("Snake Body");
    snakeBodyTexture->setIcon(QIcon(map.getSnakeBodyTexture()));
    customTextureTable->setItem(1, 0, snakeBodyTexture);

    auto *appleTexture = new QTableWidgetItem("Apple");
    appleTexture->setIcon(QIcon(map.getAppleTexture()));
    customTextureTable->setItem(2, 0, appleTexture);

    auto *bonusTexture = new QTableWidgetItem("Bonus");
    bonusTexture->setIcon(QIcon(map.getBonusTexture()));
    customTextureTable->setItem(2, 0, bonusTexture);

    connect(customTextureTable, &QTableWidget::doubleClicked,
            [this, customTextureTable](const QModelIndex &index) {
                const QString texturePath = QFileDialog::getOpenFileName(this, "Open Image", "",
                                                                         "Image Files (*.png *.jpg *.bmp)");
                if (texturePath.isEmpty())
                    return;

                const QPixmap newTexture(texturePath);
                if (newTexture.isNull())
                    return;

                switch (index.row()) {
                    case 0:
                        map.setSnakeHeadTexture(&newTexture);
                        break;
                    case 1:
                        map.setSnakeBodyTexture(&newTexture);
                        break;
                    case 2:
                        map.setAppleTexture(&newTexture);
                        break;
                    case 3:
                        map.setBonusTexture(&newTexture);
                        break;
                    default:
                        break;
                }

                auto *item = customTextureTable->item(index.row(), 0);
                item->setIcon(QIcon(newTexture));
                gameArea->update();
            });
    connect(customTextureTable, &QTableWidget::customContextMenuRequested,
            [this, customTextureTable](const QPoint &pos) {
                QMenu contextMenu;

                const auto action = new QAction("Reset", this);
                connect(action, &QAction::triggered, [this, pos, customTextureTable] {
                    auto *item = customTextureTable->itemAt(pos);
                    switch (customTextureTable->row(item)) {
                        case 0:
                            map.setSnakeHeadTexture(nullptr);
                            item->setIcon(QIcon(map.getSnakeHeadTexture()));
                            break;
                        case 1:
                            map.setSnakeBodyTexture(nullptr);
                            item->setIcon(QIcon(map.getSnakeBodyTexture()));
                            break;
                        case 2:
                            map.setAppleTexture(nullptr);
                            item->setIcon(QIcon(map.getAppleTexture()));
                            break;
                        case 3:
                            map.setBonusTexture(nullptr);
                            item->setIcon(QIcon(map.getBonusTexture()));
                            break;
                        default:
                            break;
                    }

                    gameArea->update();
                });
                contextMenu.addAction(action);

                contextMenu.exec(customTextureTable->mapToGlobal(pos));
            });

    auto *dafaultTypeLabel = new QLabel("Default Tile Type");
    for (auto it = types.begin(); it != types.end(); ++it) {
        defaulTypeSelect->addItem(it.key());
    }
    defaulTypeSelect->setCurrentText(map.getDefaultTile());

    connect(defaulTypeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](const int index) {
        map.setDefaultTile(defaulTypeSelect->itemText(index));
    });

    rightLayout->addWidget(tileTypeLabel);
    rightLayout->addWidget(tileTypeTable);
    rightLayout->addWidget(addTileType);
    rightLayout->addWidget(customTextureLabel);
    rightLayout->addWidget(customTextureTable);
    rightLayout->addWidget(dafaultTypeLabel);
    rightLayout->addWidget(defaulTypeSelect);


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

void EditorScreen::onTileSelectorItemDoubleClicked(const QTableWidgetItem *item) {
    lastEditedItemText = item->text();
}

void EditorScreen::onTileSelectorItemChanged(const QTableWidgetItem *item) {
    if (lastEditedItemText.isEmpty())
        return;

    const QString newText = item->text();
    if (map.getTypes().contains(newText)) {
        QMessageBox::warning(this, "Invalid Input", "A tile type with the name " + newText + " already exists.");
        tileTypeTable->item(item->row(), 0)->setText(lastEditedItemText);
        lastEditedItemText.clear();
        return;
    }

    map.setNewTypeName(lastEditedItemText, newText);

    const bool isSelected = defaulTypeSelect->currentText() == lastEditedItemText;
    defaulTypeSelect->removeItem(defaulTypeSelect->findText(lastEditedItemText));
    defaulTypeSelect->addItem(newText);
    if (isSelected) {
        defaulTypeSelect->setCurrentText(newText);
    }

    lastEditedItemText.clear();
}

void EditorScreen::showTileSelectorContextMenu(const QPoint &pos) {
    QMenu contextMenu;

    const QTableWidgetItem *item = tileTypeTable->itemAt(pos);

    if (item == nullptr)
        return;

    if (!map.getTypes().contains(item->text()))
        return;

    TileType type = map.getTypes()[item->text()];

    const auto label = new QLabel(" Tile type:", this);
    const auto action_label = new QWidgetAction(this);
    action_label->setDisabled(type.is_default);
    action_label->setDefaultWidget(label);
    contextMenu.addAction(action_label);

    // Create actions and add them to the group
    const auto action_traversable = new QAction("WALKABLE", this);
    action_traversable->setCheckable(true);
    action_traversable->setDisabled(type.is_default);
    action_traversable->setChecked(type.type & WALKABLE);
    connect(action_traversable, &QAction::triggered, [this, item, type] {
        // Remove spawn flags if traversable is unchecked
        const int flags = type.type & WALKABLE ? type.type & ~APPLE_SPAWN & ~BONUS_SPAWN : type.type | APPLE_SPAWN | BONUS_SPAWN;

        map.setTypeType(item->text(), flags ^ WALKABLE);
    });
    contextMenu.addAction(action_traversable);

    const auto action_apple = new QAction("APPLE SPAWN", this);
    action_apple->setCheckable(true);
    action_apple->setDisabled(type.is_default);
    action_apple->setChecked(type.type & APPLE_SPAWN);
    connect(action_apple, &QAction::triggered, [this, item, type] {
        map.setTypeType(item->text(), type.type ^ APPLE_SPAWN);
    });
    contextMenu.addAction(action_apple);

    const auto action_bonus = new QAction("BONUS SPAWN", this);
    action_bonus->setCheckable(true);
    action_bonus->setDisabled(type.is_default);
    action_bonus->setChecked(type.type & BONUS_SPAWN);
    connect(action_bonus, &QAction::triggered, [this, item, type] {
        map.setTypeType(item->text(), type.type ^ BONUS_SPAWN);
    });
    contextMenu.addAction(action_bonus);

    contextMenu.addSeparator();

    const auto action_delete = new QAction("Delete", this);
    action_delete->setDisabled(map.getTypes()[item->text()].is_default);
    connect(action_delete, &QAction::triggered, [this, item] {
        if (defaulTypeSelect->currentText() == item->text()) {
            defaulTypeSelect->setCurrentText("ground");
        }
        defaulTypeSelect->removeItem(defaulTypeSelect->findText(item->text()));

        map.deleteType(item->text());
        tileTypeTable->removeRow(item->row());
        gameArea->update();
    });
    contextMenu.addAction(action_delete);

    contextMenu.exec(tileTypeTable->mapToGlobal(pos));
}
