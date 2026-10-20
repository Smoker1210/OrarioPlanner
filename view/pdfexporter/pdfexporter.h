#ifndef PDFEXPORTER_H
#define PDFEXPORTER_H
#include <QString>
#include <QTableWidget>
namespace GUI{
class PdfExporter
{
private:
    static QString generaHTML(QTableWidget* table, int numeroSettimana, int anno, const std::tuple<double,double,int>& infoUtili, const QString& note);
public:
    PdfExporter() = delete;
    static bool esportaOrarioSettimanale(const QString& filePath, QTableWidget* table, int numeroSettimana, int Anno, const std::tuple<double,double,int> infoUtili, const QString& note);
};
}
#endif // PDFEXPORTER_H
