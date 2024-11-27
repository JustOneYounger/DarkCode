#ifndef PYTHON_SYNTAXHIGHLIGHTER_H
#define PYTHON_SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextFormat>
#include <QTextDocument>
class Python_SyntaxHighLighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Python_SyntaxHighLighter(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    QRegularExpression Control_Expression;
    QTextCharFormat Control_Format;

    QRegularExpression Def_Expression;
    QTextCharFormat Def_Format;

    QRegularExpression Logical_Expression;
    QTextCharFormat Logical_Format;

    QRegularExpression Special_Expression;
    QTextCharFormat Special_Format;

    QRegularExpression Other_Expression;
    QTextCharFormat Other_Format;

    QRegularExpression Comment_Expression;
    QTextCharFormat Comment_Format;

    QRegularExpression String_Expression;
    QTextCharFormat String_Format;

    QRegularExpression Name_Expression;
    QTextCharFormat Name_Format;

    QRegularExpression Class_Self_Expression;
    QTextCharFormat Class_Self_Format;
};

#endif // PYTHON_SYNTAXHIGHLIGHTER_H
