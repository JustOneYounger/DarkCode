#include "Python_Editer.h"
#include <LineNumberArea.h>
Python_Editer::Python_Editer(QWidget* parent)
    :QPlainTextEdit(parent)
    ,linenumberarea(new LineNumberArea(this))
{
    setStyleSheet("background:transparent;color:#E6DB74;font-family:'Consolas';font-size:14px;");
    setTabStopDistance(fontMetrics().horizontalAdvance(" ")*8);
    setWordWrapMode(QTextOption::NoWrap);
    highlight=new Python_SyntaxHighLighter(document());
    //左侧行号渲染
    connect(this, &Python_Editer::blockCountChanged, this, &Python_Editer::updateLineNumberAreaWidth);
    connect(this, &Python_Editer::updateRequest, this, &Python_Editer::updateLineNumberArea);
    connect(this, &Python_Editer::cursorPositionChanged, this, &Python_Editer::highlightCurrentLine);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

Python_Editer::~Python_Editer(){
    delete linenumberarea;
}

int Python_Editer::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}
void Python_Editer::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
void Python_Editer::updateLineNumberArea(const QRect& rect, int dy)
{
    if (dy)
        linenumberarea->scroll(0, dy);
    else
        linenumberarea->update(0, rect.y(), linenumberarea->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
void Python_Editer::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    linenumberarea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
void Python_Editer::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        // QColor lineColor = QColor("#ECF5FF");
        // selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}
void Python_Editer::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(linenumberarea);
    // painter.fillRect(event->rect(), Qt::white);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();

    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    // 获取当前光标所在的行号
    int currentLineNumber = textCursor().blockNumber();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            if (blockNumber == currentLineNumber) {
                painter.setPen(QColor("#E6DB74"));
                QFont font = painter.font();
                font.setBold(true);
                painter.setFont(font);
            } else {
                painter.setPen(QColor("#75715E"));
                QFont font = painter.font();
                font.setBold(false);
                painter.setFont(font);
            }
            painter.drawText(0, top, linenumberarea ->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void Python_Editer::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_ParenLeft || e->key() == Qt::Key_BraceLeft || e->key() == Qt::Key_BracketLeft) {
        QTextCursor cursor = textCursor();
        if(e->key() == Qt::Key_ParenLeft){
            cursor.insertText("()");
        }
        else if(e->key() == Qt::Key_BraceLeft){
            cursor.insertText("{}");
        }
        else{
            cursor.insertText("[]");
        }
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
    }
    else if(e->key() == Qt::Key_ParenRight || e->key() == Qt::Key_BraceRight || e->key() == Qt::Key_BracketRight){
        QTextCursor cursor = textCursor();

        QTextCursor nextChar_cursor = textCursor();
        nextChar_cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor);
        QString nextChar = nextChar_cursor.selectedText();

        if(e->key() == Qt::Key_ParenRight && nextChar==")"){
            cursor.movePosition(QTextCursor::Right);
            setTextCursor(cursor);
        }
        else if(e->key() == Qt::Key_BraceRight && nextChar=="}"){
            cursor.movePosition(QTextCursor::Right);
            setTextCursor(cursor);
        }
        else if(e->key() == Qt::Key_BracketRight && nextChar=="]"){
            cursor.movePosition(QTextCursor::Right);
            setTextCursor(cursor);
        }
        else{
            cursor.insertText(e->text());
            setTextCursor(cursor);
        }
    }
    else if (e->text() == "'" || e->text() == "\"") {
        QTextCursor cursor_select = textCursor();
        QTextCursor cursor_work=textCursor();
        if(!cursor_select.atEnd()){
            cursor_select.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,1);
            QString select=cursor_select.selectedText();
            if(select=="'" && e->text()=="'"){
                cursor_work.movePosition(QTextCursor::Right);
                setTextCursor(cursor_work);
            }
            else if(select=="\"" && e->text()=="\""){
                cursor_work.movePosition(QTextCursor::Right);
                setTextCursor(cursor_work);
            }
            else if(select!="'" && e->text()=="'"){
                cursor_work.insertText("''");
                cursor_work.movePosition(QTextCursor::Left);
                setTextCursor(cursor_work);
            }
            else if(select!="\"" && e->text()=="\""){
                cursor_work.insertText("\"\"");
                cursor_work.movePosition(QTextCursor::Left);
                setTextCursor(cursor_work);
            }
        }
        else{
            if(e->text()=="'"){
                cursor_work.insertText("''");
                cursor_work.movePosition(QTextCursor::Left);
                setTextCursor(cursor_work);
            }
            else if(e->text()=="\""){
                cursor_work.insertText("\"\"");
                cursor_work.movePosition(QTextCursor::Left);
                setTextCursor(cursor_work);
            }
        }
    }
    else if(e->key()==Qt::Key_Return){
        QTextCursor cursor=textCursor();
        QString text = cursor.block().text();

        int tabCount = 0;
        for (int i = 0; i < text.length(); ++i) {
            if (text[i] == '\t') {
                ++tabCount;
            } else {
                break;
            }
        }
        cursor.insertBlock();
        QString newLineTab=QString("\t").repeated(tabCount);
        if(text.endsWith(":")){
            cursor.insertText(newLineTab+"\t");
        }
        else{
            cursor.insertText(newLineTab);
        }
    }
    else if(e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_Slash){
        QTextCursor cursor=textCursor();
        QString text=cursor.block().text();
        if(text.startsWith("#")){
            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
            cursor.removeSelectedText();
        }
        else{
            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.insertText("#");
            cursor.movePosition(QTextCursor::Down);
            setTextCursor(cursor);
        }
    }
    else{
        QPlainTextEdit::keyPressEvent(e);
    }
}
