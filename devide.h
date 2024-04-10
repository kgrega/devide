#ifndef DEVIDE_H
#define DEVIDE_H

#include "newdiag.h"
#include "qobjectdefs.h"
#include <QMainWindow>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class DevIde; }
QT_END_NAMESPACE

class DevIde : public QMainWindow
{
    Q_OBJECT

public:
    DevIde(QWidget *parent = nullptr);
    ~DevIde();
    void compiler();
    void running(QString appname);
    void save(QString type);
    void guioptions();
    void compilernew();
    void configfilemaker();
    QString projecttypedesc;

private slots:
    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionOpen_triggered();

    void on_actionSelect_All_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionBuild_triggered();

    void on_projectree_itemDoubleClicked(QListWidgetItem *item);

    void on_projectree_itemClicked(QListWidgetItem *item);

    void on_actionRun_triggered();

    void on_outputputton_clicked();

    void on_inputbutton_clicked();

    void commands(QString appname, QString testinput, QString testoutput);

    void on_actionCode_debug_triggered();

    void on_actionNew_triggered();
    //
    void newproject();
    void projectopen(QString projectfilename, QString path);
    void Debprojectopen(QString projectfilename);
    void gitprojectopen(QString gitpath);
    void fileopen(QString openfilename, QString path);
    void filesave(QString oldname, QString newname,QString devareastring);
    void on_actionRunning_triggered();

    void on_actionComplirer_triggered();

    void on_actionDark_triggered();

    void on_actionWhite_triggered();

    void on_actionPink_triggered();

    void on_actionClose_All_triggered();

    void on_actionCode_tester_Properties_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_QT_triggered();

private:
    Ui::DevIde *ui;
    QString opened;
    QString filecompilername;
    QString tmpfile;
    QString filenamedata;
    QStringList filenames;
    QString openprojecttype;
    QString openprojectfilename;
    QString openprojectname;

};
#endif // DEVIDE_H
