#include "pdfexporter.h"
#include "qtextformat.h"
#include <QDate>
#include <QStyledItemDelegate>
#include <QPrinter>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextTable>
namespace GUI{

QString PdfExporter::generaHTML(QTableWidget* table,int numeroSettimana,int anno, const std::tuple<double,double,int>& infoUtili,const QString& note) {
    QString html = R"(
    <!DOCTYPE html>
    <html>
    <head>
<style>
            @page {
                size: A4 landscape;
                margin: 6mm;
            }
            body {
                background-color: #ffffff;
                color: #000000;
                font-family: Arial, Helvetica, sans-serif;
                margin: 0;
                padding: 0;
                letter-spacing:0.2px;
            }
            .header {
                margin-bottom: 0px;
                border-bottom: 0px solid #000000;
                padding-bottom: 0px;
            }
            .header-left{
                text-align: left;
            }
            .header-right{
                text-align: right;
            }
            .metric-label {
                font-weight: normal;
                margin-right: 2px;
            }
            .metric-item {
                display: inline-block;
                margin-left: 12px;
            }
            h1 {
                font-size: 8pt;
                margin: 0;
                text-transform: uppercase;
                letter-spacing: 1px;
                margin-bottom: 4px;
            }
            p.title {
                font-size: 12pt;
                margin: 0 0 0 0;
                font-weight: bold;
            }
            table {
                border-collapse: collapse;
            }
            tr {
                page-break-inside: avoid !important;
                break-inside: avoid !important;
            }
            thead {
                display: table-header-group;
            }
            th {
                border: 1px solid #000000 !important;
                background-color: #f0f0f0;
                color: #000000;
                font-weight: bold;
                font-size: 9.5pt;
                text-align: center;
                vertical-align: middle;
                padding: 6px 2px;
            }
            td {
                border: 1px solid #000000;
                text-align: center;
                vertical-align: middle;
                padding: 6px 2px;
                font-size: 9pt;
                word-wrap: break-word;
            }
            td.col-employee {
                font-weight: bold;
                text-align: center;
                padding-left: 6px;
                background-color: #fafafa;
            }
            td.col-totale {
                font-weight: bold;
                background-color: #f0f0f0;
            }
            .notes-box {
                margin-top: 4px;
                padding: 3px 6px;
                border: 1px solid #000000;
                background-color: #fafafa;
                font-size: 9.5pt;
                text-align: left;
                page-break-inside: avoid !important;
                break-inside: avoid !important;
            }
        </style>
    </head>
    <body>
        <div class='header'>
            <div class='header-left'>
                <p class='title'>Orari Settimana %1 del %2</p>
            </div>
            <div class='header-right'>
            <span class="metric-item"><span class="metric-label">Redditività:</span> %3</span>
            <span class="metric-item"><span class="metric-label">Incasso:</span> %4</span>
            <span class="metric-item"><span class="metric-label">Ore Totali:</span> %5</span>
            </div>
        </div>
        <table>
            <thead>
                <tr>
    )";
    double Redditivita = std::get<0>(infoUtili);
    double oreTotali = std::get<1>(infoUtili);
    QString RedditivitaClean = QString::number(Redditivita, 'f',2);
    QString OreClean = QString::number(oreTotali, 'f', 2);

    html = html.arg(numeroSettimana).arg(anno).arg(RedditivitaClean).arg(std::get<2>(infoUtili)).arg(OreClean);

    int totalCols = table->columnCount();
    int totalRows = table->rowCount();

    // 1. INTESTAZIONI COLONNE
    for (int col = 0; col < totalCols; ++col) {
        QTableWidgetItem* headerItem = table->horizontalHeaderItem(col);
        QString headerText = headerItem ? headerItem->text().trimmed() : "";

        if (col == 0) {
            html += QString("<th>%1</th>").arg(headerText.isEmpty() ? "Dipendente" : headerText);
        } else if (col == totalCols - 1) {
            html += QString("<th>%1</th>").arg(headerText.isEmpty() ? "Tot Ore" : headerText);
        } else {
            if (headerText.contains(" ")) {
                headerText.replace(" ", "<br>");
            } else {
                headerText.replace("\n", "<br>");
            }
            html += QString("<th>%1</th>").arg(headerText);
        }
    }
    html += "</tr></thead><tbody>";

    // 2. RIGHE DIPENDENTI
    for (int row = 0; row < totalRows; ++row) {
        html += "<tr>";
        for (int col = 0; col < totalCols; ++col) {

            QTableWidgetItem* item = table->item(row, col);
            QString cellText = item ? item->text().trimmed() : "";

            if (col == 0) {
                cellText.replace('/', " ");
                cellText.replace("\n", "<br>");
                html += QString("<td class='col-employee'>%1</td>").arg(cellText);

            } else if (col == totalCols - 1) {
                cellText.replace("\n", "<br>");
                html += QString("<td class='col-totale'>%1</td>").arg(cellText);

            } else {
                if (cellText.contains('/')) {
                    cellText.replace('/', "<br>");
                } else if (cellText.contains('-')) {
                    cellText = "/";
                }
                else if(cellText.contains("T:")){
                    cellText.replace("T:","");
                }
                else {
                    cellText.replace("\n", "<br>");
                }
                html += QString("<td>%1</td>").arg(cellText);
            }
        }
        html += "</tr>";
    }

    html += "</tbody></table>";

    // 3. BOX NOTE
    QString notePulite = note.trimmed();
    if (!notePulite.isEmpty()) {
        html += QString(R"(
            <div class='notes-box'>
                <b>NOTE:</b> %1
            </div>
        )").arg(notePulite);
    }

    html += "</body></html>";

    return html;
}

bool PdfExporter::esportaOrarioSettimanale(const QString& filePath, QTableWidget* table, int numeroSettimana, int Anno, const std::tuple<double,double,int> infoUtili, const QString& note){
if (filePath.isEmpty() || !table) return false;

// Configurazione Printer A4 Landscape
QPrinter printer(QPrinter::PrinterResolution);
printer.setOutputFormat(QPrinter::PdfFormat);
printer.setOutputFileName(filePath);
printer.setPageSize(QPageSize(QPageSize::A4));
printer.setPageOrientation(QPageLayout::Landscape);

QPageLayout pageLayout = printer.pageLayout();
pageLayout.setUnits(QPageLayout::Millimeter);
pageLayout.setMargins(QMarginsF(6, 6, 6, 6));
printer.setPageLayout(pageLayout);

// Prepariamo il QTextDocument
QTextDocument doc;
QRectF pageRect = printer.pageRect(QPrinter::Point);
doc.setPageSize(pageRect.size());

// Carichiamo l'HTML generato
doc.setHtml(generaHTML(table, numeroSettimana, Anno, infoUtili, note));

// FORZIAMO LA TABELLA AL 100% DELLA LARGHEZZA TRAMITE QT C++ API
QTextCursor cursor(&doc);
cursor.movePosition(QTextCursor::Start);

// Troviamo la prima tabella nel documento (quella dei turni)
while (!cursor.atEnd()) {
    QTextTable* pdfTable = cursor.currentTable();
    if (pdfTable) {
        QTextTableFormat format = pdfTable->format();

        // Forza l'allineamento e la larghezza al 100% dell'A4
        format.setAlignment(Qt::AlignHCenter);
        format.setWidth(QTextLength(QTextLength::PercentageLength, 100));

        // Definiamo le percentuali esatte per ogni colonna (16% Dip, 7% Tot, 11% x 7 Giorni)
        QVector<QTextLength> columnWidths;
        columnWidths.append(QTextLength(QTextLength::PercentageLength, 12.5)); // Dipendente
        for (int i = 0; i < 7; ++i) {
            columnWidths.append(QTextLength(QTextLength::PercentageLength, 11.5)); // 7 Giorni
        }
        columnWidths.append(QTextLength(QTextLength::PercentageLength, 7)); // Tot Ore

        format.setColumnWidthConstraints(columnWidths);
        pdfTable->setFormat(format);
        break; // Applicato alla tabella
    }
    cursor.movePosition(QTextCursor::NextBlock);
}

//tutto lo schifo sopra perché html decideva di non collaborare con la width btw

doc.print(&printer);
return true;
}

}