#ifndef PACKAGEWIZ_H
#define PACKAGEWIZ_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class Packagewiz;
}

class Packagewiz : public QDialog
{
    Q_OBJECT

public:
    explicit Packagewiz(QWidget *parent = nullptr);
    ~Packagewiz();

private slots:
    void on_pushButton_2_pressed();

    void on_createdebproject_pressed();

    void on_deb_section_add_pressed();

    void on_diradddebputton_pressed();

    void on_browse_debpack_pressed();

    void on_debdirlist_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::Packagewiz *ui;
};

#endif // PACKAGEWIZ_H
