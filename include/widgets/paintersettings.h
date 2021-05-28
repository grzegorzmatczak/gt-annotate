#ifndef PAINTERSETTINGS_H
#define PAINTERSETTINGS_H

#include <QColor>
#include <QHash>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "widgets/structures.h"


class PainterSettings
{
    public:
        PainterSettings();
        void configureColors(QJsonObject const& a_config);
        void changeColor(QColor color, QString label);

    public:
        std::vector<QString> m_colors;
        int m_penSize{};
        QColor m_color;
        
    public:
        QHash<QString, QColor> m_colorHash;
        QHash<QString, int> m_colorInthash;
};

#endif // PAINTERSETTINGS_H
