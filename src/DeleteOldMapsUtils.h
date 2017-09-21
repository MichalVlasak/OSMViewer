#ifndef DELETEOLDMAPSUTILS_H
#define DELETEOLDMAPSUTILS_H

#include "DeleteOldMapsWidget.h"

#include <QString>

class DeleteOldMapsUtils
{
    public:
        static void tryDeleteFile(const QString & filePath, const DeleteOldMapsWidget::DeleteSettings & settings, bool enableDeleteAll = true);
};

#endif // DELETEOLDMAPSUTILS_H
