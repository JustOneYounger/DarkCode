#ifndef DARKCODE_H
#define DARKCODE_H

#include <QMainWindow>
#include <DarkCode_include.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class DarkCode;
}
QT_END_NAMESPACE

class DarkCode : public QMainWindow
{
    Q_OBJECT

public:
    DarkCode(QWidget *parent = nullptr);
    ~DarkCode();

    void initWindow();
    void setShortcut();
    void create_file_tree(QString folderPath,QTreeWidgetItem* parentItem=nullptr);
    void open_new_python_file(QTreeWidgetItem *item, int column);

private:
    void open_program();
    void left_change();
    void right_change();
    void close_file();
    void save_file();
    void save_all();
    void new_file();
    void run_python();
    void clean_output();

    QShortcut* open_program_shortcut=nullptr;
    QShortcut* left_change_shortcut=nullptr;
    QShortcut* right_change_shortcut=nullptr;
    QShortcut* close_file_shortcut=nullptr;
    QShortcut* save_file_shortcut=nullptr;
    QShortcut* save_all_shortcut=nullptr;
    QShortcut* new_file_shortcut=nullptr;
    QShortcut* run_python_shortcut=nullptr;
    QShortcut* clean_output_shortcut=nullptr;

private:
    Ui::DarkCode *ui;
    bool isDragging;//是否正在拖动
    QPoint dragStartPos;//鼠标位置
    bool isMaxWindow = true;//是否是最大化窗口

    QSplitter* splitter_file_page;
    TabPage* tab_pages;
    QString folderPath;
    QMap<QWidget*,QString> filepath_widget_map;

    QString running_python;

protected:
    //定义窗口拖动
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};
#endif // DARKCODE_H
