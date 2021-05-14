#ifndef PAINTERSETTINGS_H
#define PAINTERSETTINGS_H

#include <QObject>

class PainterSettings
{
    Q_OBJECT
    public:
        PainterSettings();
        void configureColors(QJsonObject const& a_config);

    private:
        std::vector<QString> m_colors;
        qint32 m_penSize{};
        QColor m_color;

    private:
        QHash<QString, QColor> m_colorHash;
        QHash<QString, int> m_colorInthash;
};


#endif // PAINTERSETTINGS_H
