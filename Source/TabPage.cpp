#include "TabPage.h"
#include "ui_TabPage.h"

TabPage::TabPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TabPage)
{
    ui->setupUi(this);
    setPython();
    getPip();
    setPip();
    connect(ui->pushButton_choosepython,&QPushButton::clicked,this,[this](){
        QString filename=QFileDialog::getOpenFileName(nullptr, "选择Python解释器", "", "Executable Files (*.exe)");
        if(!filename.isEmpty()){
            if (filename.contains("python.exe", Qt::CaseInsensitive)){
                ui->lineEdit_pythonexe->setText(filename);
            }
            else{
                QMessageBox::warning(nullptr,"警告","当前exe文件不是python解释器",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
            }
        }
    });
    ui->widget_python->setObjectName("Python环境配置");
    ui->widget_shortcut->setObjectName("快捷键配置");
}

TabPage::~TabPage()
{
    delete ui;
}


void TabPage::setPython(){
    ui->tableView_pip->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView_pip->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView_pip->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_pip->verticalHeader()->setVisible(false);
    ui->tableView_pip->horizontalHeader()->setVisible(false);
    ui->tableView_pip->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QProcess* process_getPython=new QProcess();
    process_getPython->start("python -c \"import sys; print(sys.executable)\"");
    if (process_getPython->waitForFinished()) {
        QString pythonExePath = QString::fromLocal8Bit(process_getPython->readAllStandardOutput().trimmed());
        pythonExePath.replace("\\","/");
        ui->lineEdit_pythonexe->setText(pythonExePath);
    }
    process_getPython->terminate();
    process_getPython->waitForFinished();
    delete process_getPython;
}

void TabPage::getPip(){
    QProcess* process_get_pip = new QProcess();
    process_get_pip->start("pip", QStringList() << "freeze");
    if (process_get_pip->waitForFinished()) {
        QString pipLibs = QString::fromUtf8(process_get_pip->readAllStandardOutput().trimmed());
        QDir currentDir = QDir::current();

        QFile file(currentDir.absoluteFilePath("requirements.txt"));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << pipLibs;
            file.close();
        }
    }
    process_get_pip->terminate();
    process_get_pip->waitForFinished();
    delete process_get_pip;
}

void TabPage::setPip(){
    QDir currentDir = QDir::current();
    QFile file(currentDir.absoluteFilePath("requirements.txt"));

    QStandardItemModel *model = new QStandardItemModel;
    model->setHorizontalHeaderLabels(QStringList() << "Package" << "Version");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream read(&file);
        QString line;
        static QRegularExpression re("(.+?)==(.+)");

        while (read.atEnd() == false) {
            line = read.readLine();
            QRegularExpressionMatch item=re.match(line);

            QStandardItem *packageItem = new QStandardItem(item.captured(1));
            QStandardItem *versionItem = new QStandardItem(item.captured(2));

            model->appendRow(QList<QStandardItem*>() << packageItem << versionItem);
        }
        file.close();
    }
    ui->tableView_pip->setModel(model);
}
