#ifndef CENTERPOINTSTRUCT_H
#define CENTERPOINTSTRUCT_H

#include <QPointF>
#include <QString>

struct CenterPointStruct
{
        static const int UndefinedLevel = -1;

        CenterPointStruct()
        {
            position = QPointF(0., 0.);
            name = "";
            level = UndefinedLevel;
        }
        CenterPointStruct(const QPointF & pos, const QString & n, int lvl)
        {
            position = pos;
            name = n;
            level = lvl;
        }

        QPointF position; // WGS
        QString name;
        int level;
};

#endif // CENTERPOINTSTRUCT_H
