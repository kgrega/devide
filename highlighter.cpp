#include "highlighter.h"
#include <QBrush>
#include <QColor>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{


    commentlinestart = QRegularExpression(QStringLiteral("/\\*"));
    commentlineend = QRegularExpression(QStringLiteral("\\*/"));

    commentline.setForeground(Qt::darkGreen);
    commentlines.setForeground(Qt::darkGreen);



}
//////////////////////////////////////////////////////////////

void Highlighter::highlightBlock(const QString &text){


    QRegularExpression textdata;
    QTextCharFormat attributeformat;
    QRegularExpressionMatchIterator matchiterator;
    //
    QColor color(0,60,0);
    QBrush colorbrush(color);

    //    color.setCmyk();
    //////????
    attributeformat.setForeground(colorbrush.color());
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral(" \"[a-zA-Z0-9 ]*\.*[a-zA-Z0-9]\""));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}

    ////    ///
    attributeformat.setForeground(Qt::blue);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("\"*\\b*[a-zA-Z0-9]*\""));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    /////   ///
    attributeformat.setForeground(Qt::darkBlue);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("[*|&|=|+|-|<|>|!]"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////    ////
    attributeformat.setForeground(Qt::darkRed);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("(\\bvirtual\\b|\\bsigned\\b|\\bregister\\b|\\bunsigned\\b|\\bdouble\\b|\\bshort\\b|\\bint\\b|\\blong\\b|\\bfloat\\b|\\bchar\\b|\\bstruct\\b|\\bbool\\b|\\bstring\\b|\\bconst\\b|\\bthis\\b|\\bregister\\b|\\benum\\b|\\bunion\\b|\\bvoid\\b|\\bwchar_t\\b|\\bchar8_t\\b|\\bchar16_t\\b|\\bchar32_t\\b|\\bauto\\b)"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////    ///
    attributeformat.setForeground(Qt::blue);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("(\\balignas\\b|\\balignof\\b|\\band_eq\\b|\\basm\\b|\\batomic_cancel\\b|\\batomic_commit\\b|\\batomic_noexcept\\b|\\bbitand\\b|\\bbitor\\b|\\bcatch\\b|\\bcompl\\b|\\bconcept\\b|\\bconsteval\\b|\\bconstexpr\\b|\\bconstinit\\b|\\bconst_cast\\b|\\bco_await\\b|\\bco_return\\b|\\bco_yield\\b|\\bdecltype\\b|\\bdynamic_cast\\b|\\bexplicit\\b|\\bexport\\b|\\bfriend\\b|\\binline\\b|\\bmutable\\b|\\bnoexcept\\b|\\bnot\\b|\\bnot_eq\\b|\\bnullptr\\b|\\boperator\\b|\\breflexpr\\b|\\breinterpret_cast\\b|\\brequires\\b|\\bsizeof\\b|\\bstatic_assert\\b|\\bstatic_cast\\b|\\bsynchronized\\b|\\btemplate\\b|\\bthis\\b|\\bthread_local\\b|\\bthrow\\b|\\btrue\\b|\\btry\\b|\\btypedef\\b|\\btypeid\\b|\\btypename\\b|\\busing\\b|\\bvolatile\\b|\\bxor\\b|\\bxor_eq\\b|\\bfinal\\b|\\boverride\\b|\\btransaction_safe\\b|\\btransaction_safe_dynamic\\b|\\bimport\\b|\\bmodule\\b|\\bor_eq\\b|\\band\\b|\\bextern\\b|\\bor\\b|\\bgoto|\\bdelete\\b|\\bcontinue\\b|\\bfor\\b|\\bforeach\\b|\\bwhile\\b|\\bif\\b|\\belse\\b|\\bbreak\\b|\\bdo\\b|\\bswitch\\b|\\bcase\\b|\\bdefault\\b|\\breturn\\b|\\bstd\\b|\\bNULL\\b|\\btrue\\b|\\bfalse\\b|\\bstatic\\b|\\bnew\\b|\\bnamespace\\b)"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////    ///
    attributeformat.setForeground(Qt::darkGreen);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("#\\w*[a-zA-Z0-9]*"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}

    ////    ///
    attributeformat.setForeground(colorbrush.color());
    attributeformat.setFontWeight(QFont::Bold);
    textdata= QRegularExpression(QStringLiteral("<[a-zA-Z0-9 ]*\.*[a-zA-Z0-9]>"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////  ///
    attributeformat.setForeground(Qt::darkMagenta);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("%(d|ld|lld|u|lu|llu|f|Lf|s|c|\\*)"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////  ///
    attributeformat.setForeground(Qt::darkYellow);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("^(class|public|protected|private|override)"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////    ///
    attributeformat.setForeground(Qt::darkCyan);
    textdata=QRegularExpression(QStringLiteral("\\b[A-Za-z]+(?=\\()"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////    ///
    attributeformat.setForeground(Qt::red);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("\\w*[a-zA-Z0-9]*::"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////    ///
    attributeformat.setForeground(Qt::darkMagenta);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("::\\w*[a-zA-Z0-9]()"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    ////   ///
    attributeformat.setForeground(Qt::darkRed);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("\\b[0-9]*"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}


    ////    ///commentlinetest
    attributeformat.setForeground(Qt::darkGreen);
    attributeformat.setFontWeight(QFont::Bold);
    textdata=QRegularExpression(QStringLiteral("//*.*"));
    matchiterator = textdata.globalMatch(text);
    while (matchiterator.hasNext()) {
        QRegularExpressionMatch match = matchiterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeformat);}
    /// //////////////////////////////

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentlinestart);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentlineend.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;}
        else {
            commentLength = endIndex - startIndex + match.capturedLength();}
        setFormat(startIndex, commentLength, commentlines);
        startIndex = text.indexOf(commentlinestart, startIndex + commentLength);
    }

}



