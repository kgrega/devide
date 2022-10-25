#ifndef DEVIDE_H
#define DEVIDE_H

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
    void save();

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
//
    void codehightlight(QTextStream *test);
    void commands(QString appname, QString testinput,QString testoutput);
    void on_actionTester_triggered();

    void on_output_clicked();

    void on_input_clicked();

private:
    Ui::DevIde *ui;
    QString opened;
    QString filecompilername;
    QString tmpfile;
};
#endif // DEVIDE_H
