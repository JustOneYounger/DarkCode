#include "Python_SyntaxHighLighter.h"

Python_SyntaxHighLighter::Python_SyntaxHighLighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    // 初始化正则表达式
    Control_Expression=QRegularExpression("\\b(break|continue|for|while|if|elif|else|try|catch|except|finally|with)\\b");
    Def_Expression=QRegularExpression("\\b(def|class|return|global|nonlocal|assert|import|from|as)\\b");
    Logical_Expression=QRegularExpression("\\b(and|or|not|in|is)\\b");
    Special_Expression=QRegularExpression("\\b(None|True|False|lambda|yield|await|async|with)\\b");
    Other_Expression=QRegularExpression("\\b(raise|del|pass)\\b");
    Comment_Expression=QRegularExpression("#.*");

    String_Expression=QRegularExpression(R"(('(.*?)')|(\"(.*?)\"))");
    Name_Expression=QRegularExpression(R"(\b(def|class)\s+([A-Za-z_][A-Za-z0-9_]*)\b)");
    Class_Self_Expression=QRegularExpression("self");

    // 初始化高亮格式
    Control_Format.setForeground(QColor("#A6E22E"));
    Def_Format.setForeground(QColor("#66D9EF"));
    Logical_Format.setForeground(QColor("#F92672"));
    Special_Format.setForeground(QColor("#FD971F"));
    Other_Format.setForeground(QColor("#AE81FF"));
    Comment_Format.setForeground(QColor("#75715E"));
    Comment_Format.setFontItalic(true);

    String_Format.setForeground(QColor("#A6E22E"));
    Name_Format.setForeground(QColor("#66D9EF"));
    Class_Self_Format.setForeground(QColor("#AE81FF"));
}

void Python_SyntaxHighLighter::highlightBlock(const QString& text) {
    QRegularExpressionMatchIterator match_Control_Iterator = Control_Expression.globalMatch(text);
    while (match_Control_Iterator.hasNext()) {
        QRegularExpressionMatch match_Control = match_Control_Iterator.next();
        setFormat(match_Control.capturedStart(), match_Control.capturedLength(), Control_Format);
    }
    QRegularExpressionMatchIterator match_Def_Iterator = Def_Expression.globalMatch(text);
    while (match_Def_Iterator.hasNext()) {
        QRegularExpressionMatch match_Def = match_Def_Iterator.next();
        setFormat(match_Def.capturedStart(), match_Def.capturedLength(), Def_Format);
    }
    QRegularExpressionMatchIterator match_Logical_Iterator = Logical_Expression.globalMatch(text);
    while (match_Logical_Iterator.hasNext()) {
        QRegularExpressionMatch match_Logical = match_Logical_Iterator.next();
        setFormat(match_Logical.capturedStart(), match_Logical.capturedLength(), Logical_Format);
    }
    QRegularExpressionMatchIterator match_Special_Iterator = Special_Expression.globalMatch(text);
    while (match_Special_Iterator.hasNext()) {
        QRegularExpressionMatch match_Special = match_Special_Iterator.next();
        setFormat(match_Special.capturedStart(), match_Special.capturedLength(), Special_Format);
    }
    QRegularExpressionMatchIterator match_Other_Iterator = Other_Expression.globalMatch(text);
    while (match_Other_Iterator.hasNext()) {
        QRegularExpressionMatch match_Other = match_Other_Iterator.next();
        setFormat(match_Other.capturedStart(), match_Other.capturedLength(), Other_Format);
    }
    QRegularExpressionMatchIterator match_Comment_Iterator = Comment_Expression.globalMatch(text);
    while (match_Comment_Iterator.hasNext()) {
        QRegularExpressionMatch match_Comment = match_Comment_Iterator.next();
        setFormat(match_Comment.capturedStart(), match_Comment.capturedLength(), Comment_Format);
    }
    QRegularExpressionMatchIterator match_String_Iterator = String_Expression.globalMatch(text);
    while (match_String_Iterator.hasNext()) {
        QRegularExpressionMatch match_String = match_String_Iterator.next();
        setFormat(match_String.capturedStart(), match_String.capturedLength(), String_Format);
    }
    QRegularExpressionMatchIterator match_Name_Iterator = Name_Expression.globalMatch(text);
    while (match_Name_Iterator.hasNext()) {
        QRegularExpressionMatch match_Name = match_Name_Iterator.next();
        setFormat(match_Name.capturedStart(), match_Name.capturedLength(), Name_Format);
    }
    QRegularExpressionMatchIterator match_Class_Self_Iterator = Class_Self_Expression.globalMatch(text);
    while (match_Class_Self_Iterator.hasNext()) {
        QRegularExpressionMatch match_Class_Self = match_Class_Self_Iterator.next();
        setFormat(match_Class_Self.capturedStart(), match_Class_Self.capturedLength(), Class_Self_Format);
    }
}
