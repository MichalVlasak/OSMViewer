#include "DeleteOldMapsUtils.h"

#include <QFileInfo>

void DeleteOldMapsUtils::tryDeleteFile(const QString & filePath, const DeleteOldMapsWidget::DeleteSettings & settings, bool enableDeleteAll)
{
    QFile file(filePath);

    if(file.exists() == true && settings.deleteEnabled == true)
    {
        if(settings.deleteType == DeleteOldMapsWidget::DeleteAll)
        {
            if(enableDeleteAll == true)
            {
                file.remove();
            }
        }
        else if(settings.deleteType == DeleteOldMapsWidget::DeleteOldAsTime)
        {
            QFileInfo fileInfo(file);

            // Ak pod Win testujem created() nad dlazdicou, tak mi stale vracia datum prveho stiahnutia
            // aj ked ju vymazem a nanovo stiahnem, preto kontrolujem datum podlednej upravy
            if(fileInfo.lastModified() < settings.deleteTime)
            {
                file.remove();
            }
        }
    }
}
