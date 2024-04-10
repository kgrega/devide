#include "runner.h"
#include <QDir>
#include "ui_runner.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

Runner::Runner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Runner)
{
    ui->setupUi(this);
    if(!QFile::exists(QDir::currentPath()+"/profiles/run.opt")){
         QMessageBox::information(this,"Hiba","A konfigurációs állomány nem létezik!");
         delete ui;}
    else{
    runnerlistadder();}
    if(QFile::exists(QDir::currentPath() + "/profiles/gui.opt")){
        QFile saveopt(QDir::currentPath() + "/profiles/gui.opt");
        QTextStream saveoptstr(&saveopt);
        saveopt.open(QIODevice:: ReadOnly | QIODevice::Text);
        QString backgroundcolor(saveoptstr.readLine()),buttoncheckboxcolor(saveoptstr.readLine()),textlinecolor(saveoptstr.readLine()),listcolor(saveoptstr.readLine());
        saveopt.close();
        this->setStyleSheet(backgroundcolor);
        ui->savebutton->setStyleSheet(buttoncheckboxcolor);
        ui->arguments->setStyleSheet(textlinecolor);
        ui->run_program_list->setStyleSheet(textlinecolor);}
     this->setWindowTitle(tr("Futtatás beállításai"));
    //

}

Runner::~Runner()
{
    delete ui;
}

void Runner::runnerlistadder(){
    QStringList programlist={"kitty","konsole","rxvt","pterm","qterminal","eterm","urxvt","xfce4-terminal","Terminator","Tilda","Tilix","xterm","uxterm","gnome-terminal"};
    QString runoptdata;
    QStringList runopdatalist;
    QFile runneroptfile(QDir::currentPath() + "/profiles/run.opt");
    QTextStream runneroptfilestr(&runneroptfile);
    runneroptfile.open(QIODevice:: ReadOnly |QIODevice::Text);
    runoptdata=runneroptfilestr.readLine();
    runopdatalist=runoptdata.split("PROGRAM=");
    ui->run_program_list->addItem(runopdatalist.last());
    runoptdata=runneroptfilestr.readLine();
    runopdatalist=runoptdata.split("ARGUMENTS=");
    ui->arguments->setText(runopdatalist.last());

    foreach(QString terminalstr,programlist){
        if(QFile::exists("/bin/" + terminalstr)){
            ui->run_program_list->addItem(terminalstr);}
        }

   }


void Runner::on_savebutton_pressed()
{
    QFile savedatafile(QDir::currentPath() + "/profiles/run.opt");
    QTextStream savedatafilestr(&savedatafile);
    savedatafile.open(QIODevice:: WriteOnly | QIODevice:: Text);
    savedatafilestr << "PROGRAM=" + ui->run_program_list->currentText()+ "\n";
    savedatafilestr << "ARGUMENTS=" + ui->arguments->toPlainText()+ "\n";
    accept();

}

