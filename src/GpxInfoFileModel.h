#ifndef GPXINFOFILEMODEL_H
#define GPXINFOFILEMODEL_H

#include <QStandardItemModel>

#include "GpxManager.h"

class GpxInfoFileModel : public QStandardItemModel
{
    public:
        explicit GpxInfoFileModel(QObject *parent = nullptr);
        virtual ~GpxInfoFileModel() = default;

    public:
        /**
         * @brief The HeaderTableEnum enum Enumerator poloziek, ktore oznacuju stlpce tabulky
         */
        enum class HeaderTableEnum
        {
            Time, Latitude, Longitude, Elevation, HeartRate, Cadention, Temperature, Last
        };

        static const int ERROR_INDEX = -1;

    public:
        void initializeTableHeader();
        void clear();

        int getColumnIndex(HeaderTableEnum headerItem) const;
        void setItemPoints(const GpxManager::PointVector & points);

    private:
        /**
         * @brief The HeaderIdent struct Struktura identifikujuca nazov stlpca a jeho poradie.
         * Poradie stlpcov zacina ako vzdy od nuly
         */
        struct HeaderIdent
        {
                int col;
                QString colName;
        };

        /**
         * @brief HeaderMap Mapa mapujuca na prvok stlpca strukturu, ktora obsahuje poradove cislo
         * tohto stlpca a taktiez jeho nazov. Tymto sposobom je zabezpecene, ze pri inicializacii
         * mapy sa zadefinuje poradove cislo stlpca kazdej polozky v tabulke. Nasledne pri vkladani
         * novej polozky TableItem do tabulky sa uz len pomocou identifikatora polozky zisti
         * v ktorom stlpci sa ma tato polozka nachadzat a tam ju aj vlozi.
         */
        using HeaderMap =  std::map<HeaderTableEnum, HeaderIdent>;

    private: // Members
        HeaderMap _headerMap;
};

#endif // GPXINFOFILEMODEL_H
