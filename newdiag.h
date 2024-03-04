#ifndef NEWDIAG_H
#define NEWDIAG_H

#include <QDialog>

//lang types
#define CFILE "#include stdio.h /n/n/n int main(){printf(HEllo world)return 0;}/n"
#define CPLUSPLUSFILE "#include <iostream> /n /n/n/ int main(){/n using namespace std;/n cout << HEllo world/n return 0;}/n"
//

namespace Ui {
class Newdiag;
}

class Newdiag : public QDialog
{
    Q_OBJECT

public:
    explicit Newdiag(QWidget *parent = nullptr);
    ~Newdiag();
    QString projecttypestr;
    QString datastr();

private slots:
    void on_createprojectbutton_pressed();

    void on_cancel_button_pressed();

    void on_project_path_set_pressed();

    void on_projecttype_currentTextChanged(const QString &newprojecttype);
    void EXITDIALOG();

private:
    Ui::Newdiag *ui;
};

#endif // NEWDIAG_H
