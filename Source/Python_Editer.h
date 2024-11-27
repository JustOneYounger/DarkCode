#ifndef PYTHON_EDITER_H
#define PYTHON_EDITER_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QTextBlock>
#include <QColor>
#include <Python_SyntaxHighLighter.h>
#include <QLocale>

class Python_Editer : public QPlainTextEdit
{
    Q_OBJECT
public:
    Python_Editer(QWidget* parent = nullptr);
    ~Python_Editer();

public:
    //左侧行号
    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();

protected:
    //左侧行号
    void resizeEvent(QResizeEvent* event) override;
    //括号补全
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    //左侧行号
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect& rect, int dy);

private:
    QWidget* linenumberarea;
    Python_SyntaxHighLighter* highlight;
};

#endif // PYTHON_EDITER_H
