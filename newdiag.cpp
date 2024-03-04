#include "newdiag.h"
#include "ui_newdiag.h"
#include "packagewiz.h"
#include "devide.h"
#include <QFileDialog>
#include <QTextStream>
#include <QListWidgetItem>
#include <QProcess>

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
    projectfile.setFileName(QDir::currentPath()+ "/project");
    projectfile.open(QIODevice::WriteOnly | QIODevice:: Text);
    projectfilestr << ui->project_path->text()+ "/" + ui->project_name->text() + "/DEBIAN/control";
    projectfilestr.flush();
    projectfile.close();
    debfile->show();
    connect(debfile,SIGNAL(accepted()),this,SLOT(EXITDIALOG()));}
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

    //javított_szintén
            projectfile.setFileName(QDir::currentPath()+ "/project");
                projectfile.open(QIODevice::WriteOnly | QIODevice:: Text);
                projectfilestr << ui->project_path->text()+ "/" + ui->project_name->text() + "/" + ui->project_name->text() + ".project";
                projectfilestr.flush();
                projectfile.close();
    this->done(1);
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
        //javított_szintén
                projectfile.setFileName(QDir::currentPath()+ "/project");
                    projectfile.open(QIODevice::WriteOnly | QIODevice:: Text);
                    projectfilestr << ui->project_path->text()+ "/" + ui->project_name->text() + "/" + ui->project_name->text() + ".project";
                    projectfilestr.flush();
                    projectfile.close();
        this->done(1);
    }

    ////////////////////////////////gitclient_2024_02_26
    if(ui->projecttype->currentText()=="Git client"){
        projectfile.setFileName(QDir::currentPath()+ "/gitproject");
        projectdir.mkdir(ui->project_name->text());
        projectfile.open(QIODevice:: WriteOnly | QIODevice:: Text);
        QProcess git;
        QStringList gitargs;
        gitargs.append("clone");
        gitargs.append(ui->project_url->text());
        gitargs.append(ui->project_path->text() + "/" + ui->project_name->text());
        git.start("git",gitargs);
        git.waitForFinished(-1);

        projectfilestr << ui->project_path->text()+ "/" + ui->project_name->text() + "/";
        projectfilestr.flush();
        projectfile.close();
        this->done(1);}
    ///    ////////////////////////////////gitclient_END
    ///
    ///
    ///
    ////////////////////////////////addfile_2024_02_26

    if(ui->projecttype->currentText()=="Add file"){
        projectfile.setFileName(QDir::currentPath() +"/addfile");
        projectfile.open(QIODevice:: WriteOnly | QIODevice:: Text);
        projectfilestr << ui->project_path->text() + "/" + ui->project_name->text();

        projectfilestr.flush();
        projectfile.close();

        this->done(1);}
    ///    ////////////////////////////////addfile_END
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

void Newdiag::on_projecttype_currentTextChanged(const QString &newprojecttype)
{
    QFile projectfile;
    QTextStream projectfilestr(&projectfile);
    QString path;
    QStringList pathlist;


    int i=0;
    if(newprojecttype=="Debian Package"){i=1;} else{i=0;}
    ui->project_path->setDisabled(i);
    ui->project_name->setDisabled(i);
    ui->project_path_set->setDisabled(i);

    if(newprojecttype=="Add file" && QFile::exists(QDir::currentPath() + "/project")){
        projectfile.setFileName(QDir::currentPath() + "/project");
        projectfile.open(QIODevice:: ReadOnly | QIODevice:: Text);
        path=projectfilestr.readLine();
        pathlist=path.split(".project");
        path=pathlist.first();
        pathlist=path.split('/');
        pathlist.takeLast();
        path="";
        foreach(QString data,pathlist){             path=path + data + "/";         }
        ui->project_path->setText(path);
        ui->project_path->setDisabled(1);}


}

void Newdiag::EXITDIALOG(){
        this->done(1);
            delete ui;
}
