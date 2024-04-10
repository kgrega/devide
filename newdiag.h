#ifndef NEWDIAG_H
#define NEWDIAG_H

#include <QDialog>

//lang types
#define CFILE "#include <stdio.h> \n\n int main(){\nprintf(\"Hello World\");\nreturn 0;}"
#define CPLUSPLUSFILE "#include <iostream> \nint main(){\n using namespace std;\n cout << \"Hello World\";\n return 0;}"
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
