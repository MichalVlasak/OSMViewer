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
        void selectAndDownloadArea();
        void centerMap(QPoint pos);

    public slots:

    private slots:
        void centerMapActivated();

    private:
        QPoint _pos;
};

#endif // MAPCONTEXTMENU_H
