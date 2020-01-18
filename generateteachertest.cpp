#include "generateteachertest.h"
#include <QPrinter>
#include <QDesktopServices>
#include <QDebug>
#include <QDate>

GenerateTeachTest::GenerateTeachTest(QObject *parent) : QObject(parent)
{

}

void GenerateTeachTest::set_model(QStandardItemModel* model, CustomTypes::RequestType request_type)
{
    if (request_type == CustomTypes::RequestCreatedFullTeacherReportRequest){
        m_model_teacher_test = model;
        generate_full_report();
    } else if(request_type == CustomTypes::RequestCreatedTeacherWithZerosReportRequest){
        m_model_teacher_test = model;
        generate_teacher_with_question_report();
    } else if (request_type == CustomTypes::RequestCreatedTeacherWithoutZerosReportRequest){
        m_model_teacher_test = model;
        generate_teacher_without_question_report();
    }
}

void GenerateTeachTest::generate_document()
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName("teacher_test.pdf");
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);
    QDesktopServices::openUrl(QUrl("teacher_test.pdf"));
}

void GenerateTeachTest::generate_full_report()
{
    QString html = QString("%1"
                           "%2"
                           "<p>&nbsp;</p>"
                           "%4"
                           "<br>"
                           "<br>"
                           "<hr>"
                           "<b>Lista nauczycieli bez stworzonych pytań:</b>"
                           "<hr>"
                           "%3")
            .arg(generate_header())
            .arg(generate_frame())
            .arg(generate_teacher_without_test())
            .arg(generate_teacher());
    document.setHtml(html);
    generate_document();
}

void GenerateTeachTest::generate_teacher_with_question_report()
{
    QString html = QString("%1"
                           "%2"
                           "<p>&nbsp;</p>"
                           "<b>Lista nauczycieli bez stworzonych pytań:</b>"
                           "<hr>"
                           "%3")
            .arg(generate_header())
            .arg(generate_frame())
            .arg(generate_teacher_without_test());
    document.setHtml(html);
    generate_document();
}

void GenerateTeachTest::generate_teacher_without_question_report()
{
    QString html = QString("%1"
                           "%2"
                           "<p>&nbsp;</p>"
                           "%3")
            .arg(generate_header())
            .arg(generate_frame())
            .arg(generate_teacher());
    document.setHtml(html);
    generate_document();
}

QString GenerateTeachTest::generate_header()
{
    QString header = QString("<h1 align = 'center'>Raport nauczycieli</h1>");

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
                        "<hr>"
                        "<table width='100%'>"
                        "<tr>"
                        "<td><b>%1</b></td>"
                        "</tr>"
                        "</table>"
                        "<hr>")
                    .arg(m_model_teacher_test->data(m_model_teacher_test->index(teacher_number, 0)).toString()
                    + " " + m_model_teacher_test->data(m_model_teacher_test->index(teacher_number, 1)).toString());
            teacher += "<table width='100%'>";
            for (int question_id = 0; question_id < m_model_teacher_test->rowCount(m_model_teacher_test->index(teacher_number, 0)) ; question_id++){
                teacher += QString(
                            "<tr>"
                            "<td>%1</td>"
                            "</tr>").arg(m_model_teacher_test->index(teacher_number, 0).child(question_id, 0).data().toString());
            }
            teacher += QString(
                        "</table>"
                        "<br>"
                        "<br>");
        }
    }
    return teacher;
}

QString GenerateTeachTest::generate_teacher_without_test()
{
    QString teacher_without_test = "<table>";
    for (int teacher_number = 0; teacher_number < m_model_teacher_test->rowCount(); teacher_number++){
        if (m_model_teacher_test->rowCount(m_model_teacher_test->index(teacher_number, 0)) == 0){
            teacher_without_test += QString(
                        "<tr>"
                        "<td>%1</td>"
                        "</tr>").arg(m_model_teacher_test->data(m_model_teacher_test->index(teacher_number, 0)).toString()
                    + " " + m_model_teacher_test->data(m_model_teacher_test->index(teacher_number, 1)).toString());
        }
    }
    teacher_without_test += "</table>";

    return teacher_without_test;
}
