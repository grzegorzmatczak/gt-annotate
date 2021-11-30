#ifndef PAINTERSETTINGS_H
#define PAINTERSETTINGS_H

#include <QColor>
#include <QHash>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "widgets/view/structures.h"


class PainterSettings
{
    public:
        PainterSettings();
        void configureColors(QJsonObject const& a_config);
        void changeColor(QColor color, QString label);

    public:
        std::vector<QString> m_colors;
        std::vector<QString> m_colors_background;
        std::vector<QString> m_colors_foreground;
        int m_penSize{};
        QColor m_color;
    
    private:
        void addrgbColors(QJsonObject json);
        
    public:
        QHash<QString, QColor> m_colorHash;
        QHash<QString, int> m_colorIntHash;
};

#endif // PAINTERSETTINGS_H
