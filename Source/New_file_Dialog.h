#ifndef NEW_FILE_DIALOG_H
#define NEW_FILE_DIALOG_H

#include <QMainWindow>
#include <QDir>
#include <QKeyEvent>
#include <QFile>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
namespace Ui {
class New_file_Dialog;
}

class New_file_Dialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit New_file_Dialog(QWidget *parent = nullptr);
    ~New_file_Dialog();
    Ui::New_file_Dialog *ui;

public:
    void set_rootpath(QString folder);
    void set_folders(QString folder);
    void create_new_file();
    void cancel();
    void Updatefiletree(QString folderPath,QTreeWidgetItem* parentItem=nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    QTreeWidget* parent_tree_widget;
    QString rootpath;
};

#endif // NEW_FILE_DIALOG_H
