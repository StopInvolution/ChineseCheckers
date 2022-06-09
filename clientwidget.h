#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include "chessboard.h"

namespace Ui {
class ClientWidget;
}

/**
 * @brief 客户端控制台
 */
class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr,ChessBoard* _parentChessBoard=nullptr);
    ~ClientWidget();
    ChessBoard *parentChessBoard;
    /**
     * @brief 处理指令
     * @param data 指令
     */
    void send(std::string data);
    void onSend();

private:
    Ui::ClientWidget *ui;
};

#endif // CLIENTWIDGET_H
