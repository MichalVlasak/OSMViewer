#ifndef POSITIONEDITORWIDGET_H
#define POSITIONEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class PositionEditorWidget;
}

class PositionEditorWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit PositionEditorWidget(QWidget *parent = nullptr);
        ~PositionEditorWidget();

        void setCurrentPosition(const QPointF & point);
        QPointF getCurrentPosition() const;

    private:
        Ui::PositionEditorWidget * _ui = nullptr;
};

#endif // POSITIONEDITORWIDGET_H
