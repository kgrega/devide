#include "packagewiz.h"
#include "ui_packagewiz.h"
#include <QFileDialog>

#include <QMessageBox>
#include <QProcess>
#include <QTextStream>

Packagewiz::Packagewiz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Packagewiz)
{
    ui->setupUi(this);
    if(QFile::exists(QDir::currentPath() + "/profiles/gui.opt")){
        QFile saveopt(QDir::currentPath() + "/profiles/gui.opt");
        QTextStream saveoptstr(&saveopt);
        saveopt.open(QIODevice:: ReadOnly | QIODevice::Text);
        QString backgroundcolor(saveoptstr.readLine()),buttoncheckboxcolor(saveoptstr.readLine()),textlinecolor(saveoptstr.readLine()),listcolor(saveoptstr.readLine());
        saveopt.close();
        this->setStyleSheet(backgroundcolor);
        ui->deb_name->setStyleSheet(textlinecolor);
        ui->debmaintainer_name->setStyleSheet(textlinecolor);
        ui->deb_ver->setStyleSheet(textlinecolor);
        ui->deb_section->setStyleSheet(textlinecolor);
        ui->deb_size->setStyleSheet(textlinecolor);
        ui->debmaintainer_mail->setStyleSheet(textlinecolor);
        ui->deb_website->setStyleSheet(textlinecolor);
        ui->depend_edit->setStyleSheet(textlinecolor);
        ui->deb_dirname->setStyleSheet(textlinecolor);
        ui->pushButton_2->setStyleSheet(buttoncheckboxcolor);
        ui->createdebproject->setStyleSheet(buttoncheckboxcolor);
        ui->diradddebputton->setStyleSheet(buttoncheckboxcolor);
        ui->deb_section_add->setStyleSheet(buttoncheckboxcolor);
        ui->deb_section_list->setStyleSheet(listcolor);
        ui->deb_arch->setStyleSheet(listcolor);
        ui->deb_priority->setStyleSheet(listcolor);
        ui->debdirlist->setStyleSheet(listcolor);
        ui->depends_text->setStyleSheet(textlinecolor);
        ui->depend_desc->setStyleSheet(textlinecolor);}
     this->setWindowTitle(tr("Debian telepítőcsomag létrehozzása"));
    //
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
    QStringList dirfiles,arguments;
    QProcess copy;
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

    ///////////////////////////////////////////
      controlfile.setFileName(QDir::currentPath()+ "/project");
          controlfile.open(QIODevice::WriteOnly | QIODevice:: Text);
          controlfilestr << ui->deb_dirname->text()+ "/DEBIAN/control";
          controlfilestr.flush();
          controlfile.close();
          // ////////////////////////////////////////////
          data=ui->debdirlist->item(i);
          while(data!=nullptr){
              dirfiles.append(data->text());
          data=ui->debdirlist->item(i++);}
          foreach(QString data,dirfiles){
              arguments.append("-r");
              arguments.append(data);
              arguments.append(ui->deb_dirname->text());
              copy.start("cp",arguments);
              copy.waitForFinished(-1);

              arguments.clear();}
          this->done(1);
          //////////////////////////////////////////////
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


void Packagewiz::on_browse_debpack_pressed()
{
    ui->deb_dirname->setText(QFileDialog::getExistingDirectory(this,"Mappa kiválasztása a csomaghoz","/home/",QFileDialog::ShowDirsOnly));
}


void Packagewiz::on_debdirlist_itemDoubleClicked(QListWidgetItem *item)
{
    ui->debdirlist->takeItem(ui->debdirlist->currentRow());
}

