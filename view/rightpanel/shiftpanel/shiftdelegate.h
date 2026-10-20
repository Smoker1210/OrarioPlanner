#ifndef SHIFTDELEGATE_H
#define SHIFTDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>

namespace GUI {

class ShiftDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    explicit ShiftDelegate();
    //formattazione visiva delle celle nella griglia
    QString displayText(const QVariant& value, const QLocale& locale) const override;
    //creazione dell'editor per la modifica
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

} // namespace GUI

#endif // SHIFTDELEGATE_H
