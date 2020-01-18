#include "generateteachertest.h"
#include <QTextDocument>
#include <QPrinter>
#include <QDesktopServices>
#include <QDebug>
#include <QDate>
#include "customtypes.h"

GenerateTeachTest::GenerateTeachTest(QObject *parent) : QObject(parent)
{

}

void GenerateTeachTest::generate_document()
{
    QString html = QString("%1"
                           "%2"
                           "<p>&nbsp;</p>"
                           "<hr>"
                           "%3"
                           "<hr>")
            .arg(generate_header())
            .arg(generate_frame())
            .arg(generate_teacher());
    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName("teacher_test.pdf");
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);
    QDesktopServices::openUrl(QUrl("teacher_test.pdf"));
}

QString GenerateTeachTest::generate_header()
{
    QString header = QString("<h1 align = 'center'>Lista nauczycieli wraz z pytaniami</h1>");

    return header;
}

QString GenerateTeachTest::generate_frame()
{
    QString frame = QString("<table style=\"height: 125px; background-color:#000;\" cellspacing=\"0\" width=\"200\" align=right>"
                                "<tbody>"
                                "<tr>"
                                "<td style='padding:1px '>"
                                "<table width= 100% style='background-color:#fff;'>"
                                "<tr>"
                                "<td>Data wystawienia: %1</td>"
                                "</tr>"
                                "</td>"
                                "</tr>"
                                "</tbody>"
                                "</table>").arg(QDate::currentDate().toString("dd.MM.yyyy"));
    return frame;
}

QString GenerateTeachTest::generate_teacher()
{
    QString teacher;

    return teacher;
}
