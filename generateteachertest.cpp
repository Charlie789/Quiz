#include "generateteachertest.h"
#include <QTextDocument>
#include <QPrinter>
#include <QDesktopServices>
#include <QDebug>
#include <QDate>

GenerateTeachTest::GenerateTeachTest(QObject *parent) : QObject(parent)
{

}

void GenerateTeachTest::set_model(QStandardItemModel* model, CustomTypes::RequestType request_type)
{
    if(request_type == CustomTypes::RequestCreatedTestWithZerosRequest || request_type == CustomTypes::RequestCreatedTestWithoutZerosRequest){
        m_model_teacher_test = model;
        generate_document();
    }
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
    for (int teacher_number = 0; teacher_number < m_model_teacher_test->rowCount(); teacher_number++){
        if (m_model_teacher_test->rowCount(m_model_teacher_test->index(teacher_number, 0)) > 0){
            teacher += QString(
                        "<table width='100%'>"
                        "<tr>"
                        "<td width='5%'>%1</td>"
                        "<td width='10%'>%2</td>"
                        "<td width='10%'>%3</td>"
                        "</tr>"
                        "</table>"
                        "<hr>")
                    .arg(teacher_number + 1)
                    .arg(m_model_teacher_test->data(m_model_teacher_test->index(teacher_number, 0)).toString())
                    .arg(m_model_teacher_test->data(m_model_teacher_test->index(teacher_number, 1)).toString());
            teacher += "<table width='100%'>";
            for (int question_id = 0; question_id < m_model_teacher_test->rowCount(m_model_teacher_test->index(teacher_number, 0)) ; question_id++){
                teacher += QString(
                            "<tr>"
                            "<td>%1</td>"
                            "</tr>").arg(m_model_teacher_test->index(teacher_number, 0).child(question_id, 0).data().toString());
            }
            teacher += "</table>";
        }
    }

    return teacher;
}
