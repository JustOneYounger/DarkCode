#ifndef TABPAGE_H
#define TABPAGE_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QProcess>
#include <QRegularExpression>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabPage;
}
QT_END_NAMESPACE

class TabPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit TabPage(QWidget *parent = nullptr);
    ~TabPage();
    Ui::TabPage *ui;

public:
    void setPython();
    void getPip();
    void setPip();
};

#endif // TABPAGE_H
