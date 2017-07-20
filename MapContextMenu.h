#ifndef MAPCONTEXTMENU_H
#define MAPCONTEXTMENU_H

#include <QWidget>

class MapContextMenu : public QWidget
{
        Q_OBJECT
    public:
        explicit MapContextMenu(QWidget *parent = 0);

        void show(const QPointF & pos);

    signals:
        void downloadArea();

    public slots:

    private:
};

#endif // MAPCONTEXTMENU_H
