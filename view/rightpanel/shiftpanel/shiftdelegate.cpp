#include "shiftdelegate.h"
#include <QPainter>
#include <QApplication>
namespace GUI {

ShiftDelegate::ShiftDelegate() {}
QString ShiftDelegate::displayText(const QVariant& value, const QLocale& locale) const{
    QString testoRaw = value.toString().trimmed();
    if(testoRaw.isEmpty())
        return QString();
    if(testoRaw.contains('/')){
        QStringList parti = testoRaw.split('/');
        return parti.join('\n');
    }
    if(testoRaw.contains('-')){
        return "/";
    }
    return testoRaw;
}
QWidget* ShiftDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const{
    if(index.column() == 0 || index.column() == 8){
        return nullptr;
    }
    QLineEdit* editor = new QLineEdit(parent);
    editor->setAlignment(Qt::AlignCenter);
    editor->setFrame(false);
    return editor;
}

void ShiftDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(text);
}
void ShiftDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const{
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    model->setData(index, lineEdit->text(), Qt::EditRole);
}

void ShiftDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Salviamo lo stato del painter
    painter->save();

    // Inizializza lo sfondo e lo stato di selezione standard di Qt (es. riga selezionata)
    QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

    // Recuperiamo il testo già formattato da displayText() (con il \n)
    QString text = displayText(index.data(Qt::DisplayRole), opt.locale);

    if (!text.isEmpty()) {
        if (opt.state & QStyle::State_Selected) {
            // Se la cella è selezionata/in focus, usiamo un testo scuro/nero visibile o il colore di testo evidenziato reale
            QColor selectedTextColor = opt.palette.color(QPalette::Active, QPalette::HighlightedText);

            // Se la palette di sistema restituisce comunque un colore troppo chiaro o invisibile, forziamo il nero/blu scuro
            if (selectedTextColor == Qt::white && !(opt.state & QStyle::State_Active)) {
                selectedTextColor = Qt::black;
            }

            painter->setPen(selectedTextColor);
        } else {
            // Cella normale
            painter->setPen(opt.palette.color(QPalette::Active, QPalette::Text));
        }
        painter->setFont(opt.font);

        // FONDAMENTALE: Qt::TextWordWrap permette di interpretare \n e spezzare il testo
        painter->drawText(opt.rect, Qt::AlignCenter | Qt::TextWordWrap, text);
    }

    painter->restore();
}
} // namespace GUI
