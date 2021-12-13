#include "widgets/view/labeldialog.h"

#include <QtWidgets>
#include "includespdlog.h"

#define DEBUG

GeneralTab::GeneralTab(const QFileInfo &fileInfo, QWidget *parent)
	: QWidget(parent)
{
	QLabel *fileNameLabel = new QLabel(tr("File Name:"));
	fileNameEdit = new QLineEdit(fileInfo.fileName());

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(fileNameLabel);
	mainLayout->addWidget(fileNameEdit);

	mainLayout->addStretch(1);
	setLayout(mainLayout);
}

QString GeneralTab::getLabelName()
{
	return fileNameEdit->text();
}

LabelDialog::LabelDialog(const QString &fileName, QWidget *parent)
	: QDialog(parent)
{
	#ifdef DEBUG
	Logger->debug("LabelDialog::LabelDialog()");
	#endif
	QFileInfo fileInfo(fileName);

	tabWidget = new QTabWidget;
	tabGeneral = new GeneralTab(fileInfo);

	m_selectLabel = new SelectLabel(fileInfo);
	tabWidget->addTab(m_selectLabel, tr("Applications"));
	tabWidget->addTab(tabGeneral, tr("General"));

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabWidget);
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);

	setWindowTitle(tr("Tab Dialog"));
}

QString LabelDialog::getLabelName()
{
	QString stringLabel = tabGeneral->getLabelName();

	if (stringLabel == "")
	{
		stringLabel = m_selectLabel->getSelected();
	}
	return stringLabel;
}

void LabelDialog::accept()
{
	#ifdef DEBUG
	Logger->debug("LabelDialog::LabelDialog() accept");
	#endif
	QDialog::accept();
}

void LabelDialog::reject()
{
	#ifdef DEBUG
	Logger->debug("LabelDialog::LabelDialog() reject");
	#endif
	QDialog::reject();
}

SelectLabel::SelectLabel(const QFileInfo &fileInfo, QWidget *parent)
	: QWidget(parent)
{
	#ifdef DEBUG
	Logger->debug("SelectLabel::SelectLabel()");
	#endif
	QLabel *topLabel = new QLabel(tr("Open with:"));

	applicationsListBox = new QListWidget;
	QStringList applications;

	applications.append(tr("dron"));
	applications.append(tr("bird"));
	applications.append(tr("cloud"));
	applicationsListBox->insertItems(0, applications);

	QCheckBox *alwaysCheckBox;

	if (fileInfo.suffix().isEmpty())
		alwaysCheckBox =
				new QCheckBox(tr("Always use this application to "
								"open this type of file"));
	else
		alwaysCheckBox = new QCheckBox(tr("Always use this application to "
							"open files with the extension '%1'").arg(fileInfo.suffix()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(topLabel);
	layout->addWidget(applicationsListBox);
	layout->addWidget(alwaysCheckBox);
	setLayout(layout);
}

QString SelectLabel::getSelected()
{
	#ifdef DEBUG
	Logger->debug("SelectLabel::getSelected()");
	#endif
	QList<QListWidgetItem *> lista = applicationsListBox->selectedItems();
	
	#ifdef DEBUG
	qDebug() << "lista.size():" << lista.size();
	#endif
	if (lista.size() == 1)
	{
		QListWidgetItem *temp = lista[0];
		
		#ifdef DEBUG
		qDebug() << "QListWidgetItem* temp = lista[0]:" << temp;
		qDebug() << "temp->text():" << temp->text();
		Logger->debug("SelectLabel::getSelected() return temp->text()");
		#endif
		return temp->text();
	}
	else
	{
		#ifdef DEBUG
		Logger->debug("SelectLabel::getSelected() return ");
		#endif
		return "";
	}
}
