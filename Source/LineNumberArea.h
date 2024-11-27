#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include <Python_Editer.h>
#include <QKeyEvent>
#include <QPainter>
class LineNumberArea:public QWidget
{
public:
    LineNumberArea(Python_Editer* editer):QWidget(editer),python_editer(editer){};
    QSize sizeHint() const override
    {
        return QSize(python_editer->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        python_editer->lineNumberAreaPaintEvent(event);
    }
private:
    Python_Editer* python_editer;
};

#endif // LINENUMBERAREA_H
