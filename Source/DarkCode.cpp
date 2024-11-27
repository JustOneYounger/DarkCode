#include "DarkCode.h"
#include "ui_DarkCode.h"

DarkCode::DarkCode(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DarkCode)
    , tab_pages(new TabPage(this))
{
    ui->setupUi(this);
    qint64 timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    QString timestamp_Str = QString::number(timestamp);
    QString current_Path = QDir::currentPath();
    running_python=current_Path+"/"+timestamp_Str+".py";
    QFile file(running_python);
    if(file.open(QIODevice::WriteOnly)){
        file.close();
    }

    initWindow();
    connect(ui->listWidget_setting,&QListWidget::itemDoubleClicked,this,[this](QListWidgetItem *item){
        QString itemText = item->text();
        int index = ui->tabWidget_code_pages->indexOf(ui->tabWidget_code_pages->findChild<QWidget*>(itemText));
        if (index != -1) {
            ui->tabWidget_code_pages->setCurrentIndex(index);
        } else {
            if(itemText=="Python环境配置"){
                ui->tabWidget_code_pages->addTab(tab_pages->ui->widget_python, itemText);
                ui->tabWidget_code_pages->setCurrentWidget(tab_pages->ui->widget_python);
            }
            else{
                ui->tabWidget_code_pages->addTab(tab_pages->ui->widget_shortcut,itemText);
                ui->tabWidget_code_pages->setCurrentWidget(tab_pages->ui->widget_shortcut);
            }
        }
    });
    setShortcut();
    connect(ui->treeWidget_program_file,&QTreeWidget::itemDoubleClicked,this,&DarkCode::open_new_python_file);
}

DarkCode::~DarkCode()
{
    QFile file(running_python);
    if(file.exists()){
        file.remove();
    }
    delete ui;
}

void DarkCode::initWindow() {
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->showFullScreen();

    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.9);
    ui->centralwidget->setGraphicsEffect(opacityEffect);

    connect(ui->pushButton_min, &QPushButton::clicked, this, [this]() {
        this->showMinimized();
    });
    connect(ui->pushButton_max, &QPushButton::clicked, this, [this]() {
        if (isMaxWindow == false) {
            this->showMaximized();
            isMaxWindow = true;
        }
        else {
            this->resize(1000,600);
            isMaxWindow = false;
        }
    });
    connect(ui->pushButton_quit, &QPushButton::clicked, this, [this]() {
        this->close();
    });

    splitter_file_page = new QSplitter(Qt::Horizontal,ui->widget_work);
    splitter_file_page->addWidget(ui->toolBox_code_files);
    splitter_file_page->addWidget(ui->tabWidget_code_pages);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(splitter_file_page);
    ui->widget_work->setLayout(layout);
    ui->widget_work->setContentsMargins(0, 0, 0, 0);
    splitter_file_page->setStyleSheet("QSplitter::handle{background-color:#409EFF;}");
    splitter_file_page->setHandleWidth(1);

    ui->treeWidget_program_file->setColumnCount(1);
    ui->treeWidget_program_file->setHeaderLabel("");

    QVBoxLayout* layout_tab = new QVBoxLayout(ui->tabWidget_code_pages);
    layout_tab->setContentsMargins(0, 0, 0, 0);
}

void DarkCode::mousePressEvent(QMouseEvent* event) {
    if (ui->widget_headline->rect().contains(event->pos())) {
        isDragging = true;
        dragStartPos = event->pos();
    }
}
void DarkCode::mouseMoveEvent(QMouseEvent* event) {
    if (isDragging) {
        move(pos() + event->pos() - dragStartPos);
    }
}
void DarkCode::mouseReleaseEvent(QMouseEvent* event) {
    isDragging = false;
}

void DarkCode::setShortcut(){
    open_program_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_openprogram->keySequence(),this);
    left_change_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_left_change_file->keySequence(),this);
    right_change_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_right_change_file->keySequence(),this);
    close_file_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_close_file->keySequence(),this);
    save_file_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_save_file->keySequence(),this);
    save_all_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_save_all->keySequence(),this);
    new_file_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_new_file->keySequence(),this);
    run_python_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_run_python->keySequence(),this);
    clean_output_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_clean_output->keySequence(),this);

    connect(open_program_shortcut,&QShortcut::activated,this,&DarkCode::open_program);
    connect(left_change_shortcut,&QShortcut::activated,this,&DarkCode::left_change);
    connect(right_change_shortcut,&QShortcut::activated,this,&DarkCode::right_change);
    connect(close_file_shortcut,&QShortcut::activated,this,&DarkCode::close_file);
    connect(save_file_shortcut,&QShortcut::activated,this,&DarkCode::save_file);
    connect(save_all_shortcut,&QShortcut::activated,this,&DarkCode::save_all);
    connect(new_file_shortcut,&QShortcut::activated,this,&DarkCode::new_file);
    connect(run_python_shortcut,&QShortcut::activated,this,&DarkCode::run_python);
    connect(clean_output_shortcut,&QShortcut::activated,this,&DarkCode::clean_output);

    connect(tab_pages->ui->keySequenceEdit_openprogram,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(open_program_shortcut){
            delete open_program_shortcut;
        }
        open_program_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_openprogram->keySequence(),this);
        connect(open_program_shortcut,&QShortcut::activated,this,&DarkCode::open_program);
    });
    connect(tab_pages->ui->keySequenceEdit_left_change_file,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(left_change_shortcut){
            delete left_change_shortcut;
        }
        left_change_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_left_change_file->keySequence(),this);
        connect(left_change_shortcut,&QShortcut::activated,this,&DarkCode::left_change);
    });
    connect(tab_pages->ui->keySequenceEdit_right_change_file,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(right_change_shortcut){
            delete right_change_shortcut;
        }
        right_change_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_right_change_file->keySequence(),this);
        connect(right_change_shortcut,&QShortcut::activated,this,&DarkCode::right_change);
    });
    connect(tab_pages->ui->keySequenceEdit_close_file,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(close_file_shortcut){
            delete close_file_shortcut;
        }
        close_file_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_close_file->keySequence(),this);
        connect(close_file_shortcut,&QShortcut::activated,this,&DarkCode::close_file);
    });

    connect(tab_pages->ui->keySequenceEdit_save_file,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(save_file_shortcut){
            delete save_file_shortcut;
        }
        save_file_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_save_file->keySequence(),this);
        connect(save_file_shortcut,&QShortcut::activated,this,&DarkCode::save_file);
    });

    connect(tab_pages->ui->keySequenceEdit_save_all,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(save_all_shortcut){
            delete save_all_shortcut;
        }
        save_all_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_save_all->keySequence(),this);
        connect(save_all_shortcut,&QShortcut::activated,this,&DarkCode::save_all);
    });

    connect(tab_pages->ui->keySequenceEdit_new_file,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(new_file_shortcut){
            delete new_file_shortcut;
        }
        new_file_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_new_file->keySequence(),this);
        connect(new_file_shortcut,&QShortcut::activated,this,&DarkCode::new_file);
    });

    connect(tab_pages->ui->keySequenceEdit_run_python,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(run_python_shortcut){
            delete run_python_shortcut;
        }
        run_python_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_run_python->keySequence(),this);
        connect(run_python_shortcut,&QShortcut::activated,this,&DarkCode::run_python);
    });

    connect(tab_pages->ui->keySequenceEdit_clean_output,&QKeySequenceEdit::keySequenceChanged,this,[this](){
        if(clean_output_shortcut){
            delete clean_output_shortcut;
        }
        clean_output_shortcut=new QShortcut(tab_pages->ui->keySequenceEdit_clean_output->keySequence(),this);
        connect(clean_output_shortcut,&QShortcut::activated,this,&DarkCode::clean_output);
    });
}

void DarkCode::open_program(){
    QString directory=QFileDialog::getExistingDirectory(nullptr,"选择项目文件夹","",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    folderPath=directory;
    QString program=QDir(directory).dirName();
    ui->treeWidget_program_file->setHeaderLabel(program);
    ui->treeWidget_program_file->clear();
    QTreeWidgetItem* parentItem=new QTreeWidgetItem(ui->treeWidget_program_file,{"项目文件"});
    parentItem->setExpanded(true);
    create_file_tree(directory,parentItem);
}
void DarkCode::left_change(){
    int index=ui->tabWidget_code_pages->currentIndex();
    if(index!=0){
        ui->tabWidget_code_pages->setCurrentIndex(index-1);
    }
}
void DarkCode::right_change(){
    int index=ui->tabWidget_code_pages->currentIndex();
    if(index!=ui->tabWidget_code_pages->count()-1){
        ui->tabWidget_code_pages->setCurrentIndex(index+1);
    }
}
void DarkCode::close_file(){
    ui->tabWidget_code_pages->removeTab(ui->tabWidget_code_pages->currentIndex());
}
void DarkCode::save_file(){
    QWidget* currentWidget = ui->tabWidget_code_pages->currentWidget();
    QString filepath=filepath_widget_map[currentWidget];
    if (currentWidget) {
        Python_Editer* python_editer = currentWidget->findChild<Python_Editer*>();
        QPlainTextEdit* python_output = currentWidget->findChild<QPlainTextEdit*>();
        if (python_editer) {
            QString python_code = python_editer->toPlainText();
            QFile file(filepath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream python_out(&file);
                python_out << python_code;
                file.close();
            }
            python_output->appendPlainText("保存成功");
        }
    }
}
void DarkCode::save_all(){
    int tabCount = ui->tabWidget_code_pages->count();
    for (int i = 0; i < tabCount; ++i) {
        QWidget* currentWidget = ui->tabWidget_code_pages->widget(i);
        if (currentWidget) {
            QString filepath = filepath_widget_map[currentWidget];
            Python_Editer* python_editer = currentWidget->findChild<Python_Editer*>();
            QPlainTextEdit* python_output = currentWidget->findChild<QPlainTextEdit*>();
            if (python_editer) {
                QString python_code = python_editer->toPlainText();
                QFile file(filepath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream python_out(&file);
                    python_out << python_code;
                    file.close();
                }
                python_output->appendPlainText("全部文件保存成功");
            }
        }
    }
}
void DarkCode::new_file(){
    New_file_Dialog* new_file_dialog=new New_file_Dialog(ui->centralwidget);
    new_file_dialog->set_rootpath(folderPath);
    new_file_dialog->ui->comboBox_new_file_folder->addItem(folderPath);
    new_file_dialog->set_folders(folderPath);
    new_file_dialog->show();
}
void DarkCode::run_python(){
    QWidget* currentWidget = ui->tabWidget_code_pages->currentWidget();
    QString filepath=filepath_widget_map[currentWidget];
    if (currentWidget) {
        Python_Editer* python_editer = currentWidget->findChild<Python_Editer*>();
        QPlainTextEdit* python_output = currentWidget->findChild<QPlainTextEdit*>();
        if (python_editer) {
            QString python_code = python_editer->toPlainText();
            QFile file(running_python);
            QFile file_save(filepath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream running_out(&file);
                running_out << python_code;
                file.close();
            }
            if (file_save.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream python_out(&file_save);
                python_out << python_code;
                file_save.close();
            }
        }

        QProcess* process_run=new QProcess();
        process_run->setProgram(tab_pages->ui->lineEdit_pythonexe->text());
        process_run->setArguments({running_python});
        process_run->start();
        if (process_run->waitForFinished()){
            QString running_result=QString::fromLocal8Bit(process_run->readAllStandardOutput().trimmed());
            QString error_result = QString::fromLocal8Bit(process_run->readAllStandardError().trimmed());

            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");

            python_output->appendPlainText(formattedDateTime+" Starting "+filepath);
            if(!running_result.isEmpty()){
                python_output->appendPlainText(running_result+"\n");
            }
            if (!error_result.isEmpty()) {
                python_output->appendPlainText(error_result+"\n");
            }
        }
        process_run->terminate();
        process_run->waitForFinished();
        delete process_run;
    }
}

void DarkCode::clean_output(){
    QWidget* currentWidget = ui->tabWidget_code_pages->currentWidget();
    if (currentWidget) {
        QPlainTextEdit* python_output = currentWidget->findChild<QPlainTextEdit*>();
        if (python_output) {
            python_output->clear();
        }
    }
}

void DarkCode::create_file_tree(QString folderPath,QTreeWidgetItem* parentItem){
    QDir dir(folderPath);
    if (!dir.exists()) {
        return;
    }
    QFileInfoList fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);

    foreach (const QFileInfo &fileInfo, fileList) {
        if (fileInfo.isDir()) {
            QTreeWidgetItem *dirItem = new QTreeWidgetItem(parentItem);
            dirItem->setText(0, fileInfo.fileName());

            create_file_tree(fileInfo.absoluteFilePath(), dirItem);
        } else if (fileInfo.isFile()) {
            QTreeWidgetItem *fileItem = new QTreeWidgetItem(parentItem);
            fileItem->setText(0, fileInfo.fileName());
        }
    }
}

void DarkCode::open_new_python_file(QTreeWidgetItem *item, int column){
    //读取文件的路径
    QString itemText = item->text(0);
    int index = ui->tabWidget_code_pages->indexOf(ui->tabWidget_code_pages->findChild<QWidget*>(itemText));
    if(index!=-1){
        ui->tabWidget_code_pages->setCurrentIndex(index);
    }
    else{
        QString open_file_path=folderPath;
        std::vector<QString> filepath_vector;
        while (item != nullptr) {
            item = item->parent();
            if (item != nullptr) {
                filepath_vector.emplace_back(item->text(0));
            }
        }
        if (!filepath_vector.empty()) {
            filepath_vector.pop_back();
        }
        for (auto item = filepath_vector.rbegin(); item != filepath_vector.rend(); ++item) {
            open_file_path+="/";
            open_file_path+=*item;
        }
        open_file_path+="/"+itemText;

        QFileInfo fileInfo(open_file_path);
        QString fileExtension = fileInfo.suffix();
        if(fileExtension.toLower() == "py"){
            //建立新的布局
            QWidget* widget_python_editer=new QWidget(this);
            widget_python_editer->setObjectName(itemText);

            QSplitter* splitter_python_editer=new QSplitter(Qt::Vertical);
            splitter_python_editer->setHandleWidth(1);

            Python_Editer* python_editer=new Python_Editer(this);
            QPlainTextEdit* python_output=new QPlainTextEdit(this);
            python_output->setStyleSheet("background:transparent;color:white;");

            splitter_python_editer->addWidget(python_editer);
            splitter_python_editer->addWidget(python_output);

            QList<int> sizes;
            sizes << 800 << 200;
            splitter_python_editer->setSizes(sizes);

            QVBoxLayout* layout_python_editer = new QVBoxLayout(widget_python_editer);
            layout_python_editer->setContentsMargins(0, 0, 0, 0);
            layout_python_editer->addWidget(splitter_python_editer);

            ui->tabWidget_code_pages->addTab(widget_python_editer,itemText);
            ui->tabWidget_code_pages->setCurrentWidget(widget_python_editer);

            //添加python代码进入编辑器
            QFile file(open_file_path);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                QTextStream stream(&file);
                QString contents = stream.readAll();
                python_editer->setPlainText(contents);
                file.close();
            }

            filepath_widget_map.insert(widget_python_editer,open_file_path);
        }
    }
}
