#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>
#include <QPixmap>
#include <QString>
#include <windows.h>

#include "common.h"
#include "image_loader.h"
#include "feature_extract.h"
#include "indexer.h"
#include "searcher.h"

int main(int argc, char *argv[]) {
    // 控制台使用 UTF-8 编码，防止中文输出乱码
    SetConsoleOutputCP(CP_UTF8);

    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("MiniImageRetrieval");
    window.resize(800, 700);

    QVBoxLayout layout(&window);

    QPushButton btnLoad("1. 选择图库文件夹");
    QPushButton btnSearch("2. 选择查询图片并搜索");

    QLabel lblQuery("查询图片预览");
    lblQuery.setFixedHeight(200);
    lblQuery.setAlignment(Qt::AlignCenter);
    lblQuery.setStyleSheet("border: 1px solid gray;");

    // 结果展示区域：可滚动的缩略图网格
    QScrollArea* scrollArea = new QScrollArea(&window);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::Box);

    QWidget* resultContainer = new QWidget(scrollArea);
    QGridLayout* resultGrid = new QGridLayout(resultContainer);
    resultGrid->setSpacing(10);
    resultContainer->setLayout(resultGrid);
    scrollArea->setWidget(resultContainer);

    layout.addWidget(&btnLoad);
    layout.addWidget(&btnSearch);
    layout.addWidget(&lblQuery);
    layout.addWidget(scrollArea);

    ImageDB db = {0};

    // 清空搜索结果网格中的旧内容
    auto clearResults = [&]() {
        QLayoutItem* item;
        while ((item = resultGrid->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
    };

    // 按钮1：加载图库
    QObject::connect(&btnLoad, &QPushButton::clicked, [&](){
        QString dir = QFileDialog::getExistingDirectory(&window, "选择图库文件夹");
        if(dir.isEmpty()) return;

        load_image_paths(dir.toLocal8Bit().constData(), &db);
        extract_all(&db);
        build_index(&db);

        clearResults();
        QLabel* infoLabel = new QLabel(
            QString("图库加载完成，共 %1 张图片").arg(db.count), resultContainer);
        resultGrid->addWidget(infoLabel, 0, 0);
    });

    // 按钮2：查询
    QObject::connect(&btnSearch, &QPushButton::clicked, [&](){
        QString file = QFileDialog::getOpenFileName(
            &window, "选择查询图片", "", "Images (*.jpg *.png *.bmp)");
        if(file.isEmpty()) return;

        ImageInfo query;
        strcpy(query.path, file.toLocal8Bit().constData());
        if(extract_feature(&query) != 0) {
            clearResults();
            QLabel* errLabel = new QLabel("无法读取查询图片", resultContainer);
            resultGrid->addWidget(errLabel, 0, 0);
            return;
        }

        SearchResult res[20];
        int count = 0;
        // 传入查询图片路径，使查询图片本身不参与排序
        search_similar(&db, query.hist, 5, res, &count, query.path);

        // 显示查询图预览
        QPixmap pix(file);
        if(!pix.isNull()) {
            lblQuery.setPixmap(pix.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            lblQuery.setText("查询图片预览（无法加载）");
        }

        clearResults();

        if (count == 0) {
            QLabel* emptyLabel = new QLabel("未找到相似图片", resultContainer);
            resultGrid->addWidget(emptyLabel, 0, 0);
            return;
        }

        // 展示搜索结果缩略图
        int columns = 3;
        for(int i = 0; i < count; i++) {
            QFrame* card = new QFrame(resultContainer);
            card->setFrameShape(QFrame::StyledPanel);
            card->setFixedSize(220, 240);

            QVBoxLayout* cardLayout = new QVBoxLayout(card);
            cardLayout->setContentsMargins(8, 8, 8, 8);

            QLabel* imgLabel = new QLabel(card);
            imgLabel->setFixedSize(200, 160);
            imgLabel->setAlignment(Qt::AlignCenter);
            imgLabel->setStyleSheet("border: 1px solid lightgray;");

            QPixmap resultPix(QString::fromLocal8Bit(res[i].path));
            if(!resultPix.isNull()) {
                imgLabel->setPixmap(resultPix.scaled(
                    200, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                imgLabel->setText("无法加载图片");
            }

            QLabel* pathLabel = new QLabel(QString::fromLocal8Bit(res[i].path), card);
            pathLabel->setWordWrap(true);
            pathLabel->setStyleSheet("font-size: 10px; color: #333;");

            QLabel* simLabel = new QLabel(
                QString("相似度: %1").arg(res[i].similarity, 0, 'f', 4), card);
            simLabel->setStyleSheet("font-size: 12px; font-weight: bold;");

            cardLayout->addWidget(imgLabel);
            cardLayout->addWidget(pathLabel);
            cardLayout->addWidget(simLabel);

            int row = i / columns;
            int col = i % columns;
            resultGrid->addWidget(card, row, col, Qt::AlignTop);
        }

        // 让剩余空间留在上方，防止卡片被拉伸
        resultGrid->setRowStretch((count + columns - 1) / columns, 1);
    });

    window.show();
    return app.exec();
}
