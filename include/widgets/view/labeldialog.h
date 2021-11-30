#ifndef LABELDIALOG_H
#define LABELDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QLineEdit;
class QTabWidget;
QT_END_NAMESPACE

class SelectLabel;

class GeneralTab : public QWidget
{
  Q_OBJECT

 public:
  explicit GeneralTab(const QFileInfo &fileInfo, QWidget *parent = 0);

  QString getLabelName();

 private:
  QLineEdit *fileNameEdit;
};

class LabelDialog : public QDialog
{
  Q_OBJECT

 public:
  explicit LabelDialog(const QString &fileName, QWidget *parent = 0);

  QString getLabelName();

 private:
  QTabWidget *tabWidget;
  QDialogButtonBox *buttonBox;
  GeneralTab *tabGeneral;

 private:
  void accept() override;
  void reject() override;
  SelectLabel *m_selectLabel;

 public:
};

class SelectLabel : public QWidget
{
  Q_OBJECT

 public:
  explicit SelectLabel(const QFileInfo &fileInfo, QWidget *parent = 0);

 public:
  QString getSelected();

 private:
  QListWidget *applicationsListBox;
};

#endif // LABELDIALOG_H
