#include "newdiag.h"
#include "ui_newdiag.h"
#include "packagewiz.h"
#include "devide.h"
#include <QFileDialog>

Newdiag::Newdiag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Newdiag)
{
    ui->setupUi(this);
}

Newdiag::~Newdiag()
{
    delete ui;
}

void Newdiag::on_createprojectbutton_pressed()
{
    QFile projectfile(ui->project_path->text() + '/' + ui->project_name->text() + '/' +  ui->project_name->text() + ".project");
    QTextStream projectfilestr(&projectfile);
    QDir projectdir(ui->project_path->text());
    if(ui->projecttype->currentText()=="Debian Package"){
    Packagewiz *debfile=new Packagewiz();

        debfile->show();}
    if(ui->projecttype->currentText()=="C++"){
    projectdir.mkdir(ui->project_name->text());
    projectfile.open(QIODevice:: WriteOnly | QIODevice:: Text);
    projectfilestr << "NAME=" + ui->project_name->text() + "\n";
    projectfilestr << "TYPE=" + ui->projecttype->currentText() + "\n";
    projectfilestr << "FILE=" + ui->project_name->text() + ".c\n";
    projectfilestr << "FILE=" + ui->project_name->text() + ".h\n";
    projectfile.close();
    projectfile.setFileName(ui->project_path->text() + '/' + ui->project_name->text() + "/" + ui->project_name->text() + ".c");
    projectfile.open(QIODevice:: WriteOnly | QIODevice:: Text);
    projectfilestr << CPLUSPLUSFILE;
    projectfilestr.flush();
    }
    if(ui->projecttype->currentText()=="C"){
        projectdir.mkdir(ui->project_name->text());
        projectfile.open(QIODevice:: WriteOnly | QIODevice:: Text);
        projectfilestr << "NAME=" + ui->project_name->text() + "\n";
        projectfilestr << "TYPE=" + ui->projecttype->currentText() + "\n";
        projectfilestr << "FILE=" + ui->project_name->text() + ".c\n";
        projectfilestr << "FILE=" + ui->project_name->text() + ".h\n";
        projectfile.close();
        projectfile.setFileName(ui->project_path->text() + '/' + ui->project_name->text() + "/" + ui->project_name->text() + ".c");
        projectfile.open(QIODevice:: WriteOnly | QIODevice:: Text);
        projectfilestr << CFILE;
        projectfilestr.flush();
    }
    this->done(1);

}


void Newdiag::on_cancel_button_pressed()
{
    delete ui;
}


void Newdiag::on_project_path_set_pressed()
{
    QString path(QFileDialog::getExistingDirectory(this,"Project helye","/home",QFileDialog::ShowDirsOnly));
    ui->project_path->setText(path);
}


QString Newdiag::datastr(){
    return projecttypestr;

}
