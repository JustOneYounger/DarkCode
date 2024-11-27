#include "New_file_Dialog.h"
#include "ui_New_file_Dialog.h"

New_file_Dialog::New_file_Dialog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::New_file_Dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    int x = (parent->width() - this->width()) / 2;
    int y = (parent->height() - this->height()) / 2;
    this->move(x, y);
    ui->lineEdit_new_file_name->setText("NewPythonFile.py");

    parent_tree_widget=parent->findChild<QTreeWidget*>("treeWidget_program_file");

    connect(ui->pushButton_sure_new_file,&QPushButton::clicked,this,&New_file_Dialog::create_new_file);
    connect(ui->pushButton_cancel_new_file,&QPushButton::clicked,this,&New_file_Dialog::cancel);
}

New_file_Dialog::~New_file_Dialog()
{
    delete parent_tree_widget;
    delete ui;
}

void New_file_Dialog::set_rootpath(QString folder){
    this->rootpath=folder;
}

void New_file_Dialog::set_folders(QString folder){
    QDir dir(folder);
    if (!dir.exists()) {
        return;
    }
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();
    foreach (const QFileInfo& fileInfo, list) {
        QString folderPath = fileInfo.absoluteFilePath();
        ui->comboBox_new_file_folder->addItem(folderPath);
        set_folders(folderPath);
    }
}

void New_file_Dialog::create_new_file(){
    QString folder_name=ui->comboBox_new_file_folder->currentText();
    QString file_name=ui->lineEdit_new_file_name->text();
    if(!folder_name.isEmpty() && !file_name.isEmpty()){
        QString filepath=folder_name+"/"+file_name;
        QFile file(filepath);
        if(file.open(QIODevice::WriteOnly)){
            file.close();
        }
        else{
            QMessageBox::warning(nullptr,"失败",filepath,QMessageBox::Ok);
        }
    }

    parent_tree_widget->clear();
    QTreeWidgetItem* parentItem=new QTreeWidgetItem(parent_tree_widget,{"项目文件"});
    parentItem->setExpanded(true);
    Updatefiletree(this->rootpath,parentItem);

    this->close();
}

void New_file_Dialog::cancel(){
    this->close();
}

void New_file_Dialog::keyPressEvent(QKeyEvent* event){
    if(event->key()==Qt::Key_Return||event->key()==Qt::Key_Enter){
        create_new_file();
    }
    else if(event->key()==Qt::Key_Escape){
        cancel();
    }
}

void New_file_Dialog::Updatefiletree(QString folderPath,QTreeWidgetItem* parentItem){
    QDir dir(folderPath);
    if (!dir.exists()) {
        return;
    }
    QFileInfoList fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);

    foreach (const QFileInfo &fileInfo, fileList) {
        if (fileInfo.isDir()) {
            QTreeWidgetItem *dirItem = new QTreeWidgetItem(parentItem);
            dirItem->setText(0, fileInfo.fileName());

            Updatefiletree(fileInfo.absoluteFilePath(), dirItem);
        } else if (fileInfo.isFile()) {
            QTreeWidgetItem *fileItem = new QTreeWidgetItem(parentItem);
            fileItem->setText(0, fileInfo.fileName());
        }
    }
}
