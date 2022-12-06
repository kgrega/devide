#include "packagewiz.h"
#include "ui_packagewiz.h"
#include <QFileDialog>

#include <QMessageBox>

Packagewiz::Packagewiz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Packagewiz)
{
    ui->setupUi(this);
}

Packagewiz::~Packagewiz()
{
    delete ui;
}

void Packagewiz::on_pushButton_2_pressed()
{
    ui->depends_text->appendPlainText(ui->depend_edit->text()+ ",");
}


void Packagewiz::on_createdebproject_pressed()
{
    QListWidgetItem *data;
    int i=0;
    //
    QStringList dirfiles;
//
    QDir debdir;
    debdir.setPath(ui->deb_dirname->text());
    debdir.mkdir("DEBIAN");
    debdir.setPath(debdir.path()+"/DEBIAN");
    QFile controlfile;
    controlfile.setFileName(debdir.path()+ "/control");
    QTextStream controlfilestr(&controlfile);
    controlfile.open(QIODevice :: WriteOnly | QIODevice :: Text);


    controlfilestr << "Package: " + ui->deb_name->text() + "\n";
    controlfilestr << "Version: " + ui->deb_ver->text() + "\n";
    controlfilestr << "Section: " + ui->deb_section->text() + "\n";
    controlfilestr << "Priority: " + ui->deb_priority->currentText() + "\n";
    controlfilestr << "Architecture: " + ui->deb_arch->currentText() + "\n";
    controlfilestr << "Depends: " + ui->depends_text->toPlainText() + "\n";
    controlfilestr << "Installed-size: " + ui->deb_size->text() + "\n";
    controlfilestr << "Maintainer: " + ui->debmaintainer_name->text() + "<" + ui->debmaintainer_mail->text() + ">\n";
    controlfilestr << "Description: " + ui->depend_desc->toPlainText() + "\n";
    controlfilestr.flush();

   // test///////////////////////////////////////////////

    data=ui->debdirlist->item(i);/*
    while(data!=nullptr){
        //debug_code
        QDir::nameFiltersFromString()
        QFile::copy(data->text(),ui->deb_dirname->text());
        data=ui->debdirlist->item(i++);
        QMessageBox::information(this,"Hello World",data->text() + " ----" + ui->deb_dirname->text(), QMessageBox::Ok);
//    QMessageBox::Information(this,"Hello World",data->text(),ui->deb_dirname->text());
    }*/
    debdir.setPath(data->text());
    dirfiles=debdir.entryList(QDir::Files);
    foreach(QString data,dirfiles){
        QMessageBox::information(this,"Hello World",data, QMessageBox::Ok);}
    //}
   //test_end///////////////////////////////////////////////
}


void Packagewiz::on_deb_section_add_pressed()
{
    ui->deb_section->clear();
    ui->deb_section->setText(ui->deb_section_list->currentText());
}


void Packagewiz::on_diradddebputton_pressed()
{
    ui->debdirlist->addItem(QFileDialog::getExistingDirectory(this,"mappa hazááadás a Debian projekthez","/home/",QFileDialog::ShowDirsOnly));
}

