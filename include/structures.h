#ifndef STRUCTURES_H
#define STRUCTURES_H

enum uiMode
{
	None = 0,
	SelectROI = 1,
	Paint = 2,
	MoveSelect = 3
};

struct colors
{
  QString name;
  QColor color;
  qint32 gray;
};

#endif // STRUCTURES_H