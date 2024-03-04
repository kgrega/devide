#ifndef COMPILER_H
#define COMPILER_H

#include "qlistwidget.h"
#include <QDialog>

namespace Ui {
class Compiler;
}

class Compiler : public QDialog
{
    Q_OBJECT

public:
    explicit Compiler(QWidget *parent = nullptr);
    ~Compiler();
    void aboutdata();
    void loadoptions();
    void compatibility();
    void warnings();
    void supporting();
    void doublesize();
    void saveopt();

private slots:
    void on_on_save_compiler_button_pressed();

    void on_warnings_select_itemDoubleClicked(QListWidgetItem *item);

    void on_warnings_selected_itemDoubleClicked(QListWidgetItem *item);

    void on_cancelButton_pressed();

private:
    Ui::Compiler *ui;
};

#endif // COMPILER_H
