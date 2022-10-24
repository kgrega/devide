#include "devide.h"
#include "ui_devide.h"
#include <QProcess>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QListWidgetItem>
#include <QStringList>
#include <QString>
#include <QMessageBox>


DevIde::DevIde(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DevIde)
{
    ui->setupUi(this);
    opened="0";
}

DevIde::~DevIde()
{
    delete ui;
}


void DevIde::on_actionExit_triggered()
{
    compiler();
    //QFile :: remove();


}


void DevIde::on_actionSave_triggered()
{
    save();
}


void DevIde::on_actionUndo_triggered()
{
    ui->devareatext->undo();
}


void DevIde::on_actionRedo_triggered()
{
    ui->devareatext->redo();
}


void DevIde::on_actionOpen_triggered()
{
    QFile openfile(QFileDialog ::getOpenFileName(this,"File open","/home/gregory/Asztal","C Files (*.c);;Header files(*.h);;"));
    ui->devareatext->clear();
    QTextStream openfilestream(&openfile);
    openfile.open(QIODevice :: ReadOnly | QIODevice :: Text);
    ui->devareatext->append(openfilestream.readAll());
    ui->projectree->addItem(openfile.fileName());
    opened= "1";
    tmpfile.append(openfile.fileName()+".tmp");
    openfile.copy(openfile.fileName()+".tmp");

    //hightlight
    QStringList linestrlist;
    QString linestr;
    if(openfile.open(QIODevice::ReadOnly | QIODevice::Text)){

           //szinezés_coloring

              //linestr =
               ui->devareatext->setBackgroundRole(QPalette::HighlightedText);
                       //setStyleSheet("color:rgb(145,25,174)");
              //ui->devareatext->setText(filedata.readAll());
               //ui->devareatext->setPlaceholderText(openfilestream.readLine());
              ui->devareatext->setTextColor("color:rgb(100,25,174)");

           }while(openfilestream.atEnd()){
             linestrlist = linestr.split("#");
             if(!linestrlist.isEmpty()){ ui->devareatext->setStyleSheet("color:rgb(32, 74, 135)");} ui->devareatext->setText(linestr);
           //szinezés_coloring_end
       openfile.close();
     //  QMessageBox::information(this,"",filename);//debugolási célból,a listára nem vette fel az adott állományt.
       }


}


void DevIde::on_actionSelect_All_triggered()
{
    ui->devareatext->selectAll();

}


void DevIde::on_actionCopy_triggered()
{
      ui->devareatext->copy();
}


void DevIde::on_actionPaste_triggered()
{
    ui->devareatext->paste();
}


void DevIde::on_actionCut_triggered()
{
        ui->devareatext->cut();
}


void DevIde::compiler(){
    QProcess comp;
    QString what;
    QStringList arguments;
    QListWidgetItem *list;
    if(opened == "1"){
    list = ui->projectree->currentItem();
    what = list->text();
    arguments.append(list->text());}
    else{

       what=filecompilername;
       arguments.append(what);
    }
    QStringList dir(what.split('.'));

    arguments.append("-o");
    arguments.append(dir.first());

    comp.start("gcc",arguments);
    comp.waitForFinished(100000);
    ui->compilerouttext->append(comp.readAllStandardError());
    ui->compilerouttext->append(comp.readAllStandardOutput());
    comp.kill();
    arguments.clear();
    if(!comp.exitCode())
    ui->compilerouttext->append("Running application one moment...");
    else
    ui->compilerouttext->append("Compilering failuled...");
    running(dir.first());
}

void DevIde :: running(QString appname){
    QProcess comp;
    QStringList arguments("appname");
    arguments.append("sh");
    arguments.append(" sh " + appname + ";pause");

    QProcess :: execute("xterm",arguments);

    //comp.write("pause");
    //ui->compilerouttext->append(comp.readAllStandardError());
    //ui->compilerouttext->append(comp.readAllStandardOutput());
    //jelenleg tervezem hogy a lefordított programot egy terminál ablakban hajtsa végre.Egy másik példányban próbáltam hogy a IDE-re ide "konzolába" küldje
    //a bevitelimezőbe írt "inputot" de lefagyott.
}

void DevIde :: save(){
    QFile savefile;

    QListWidgetItem *data(ui->projectree->currentItem());
    if(data->isSelected()){
    if(!QFile::exists(data->text() + ".tmp")){
    savefile.setFileName(data->text() + ".tmp");
    savefile.copy(QFileDialog :: getSaveFileName(this,"File Save","/home/gregory","C Files (*.c,*.cpp);;Header files(*.h);;"));}
    else{
    }
    savefile.setFileName(QFileDialog :: getSaveFileName(this,"File Save","/home/gregory","C Files (*.c,*.cpp);;Header files(*.h);;"));
    filecompilername=savefile.fileName();
    QString savedata(ui->devareatext->toPlainText());
    QTextStream savefilestream(&savefile);
    savefile.open(QIODevice :: WriteOnly | QIODevice :: Text);
    savefilestream << savedata;
    savefilestream.flush();
    savefile.close();}
}
void DevIde::on_actionBuild_triggered()
{
    if(!opened.isNull()){
        QFile savefile(QFileDialog :: getSaveFileName(this,"File Save","/home/gregory","C Files (*.c,*.cpp);;Header files(*.h);;"));
        QString savedata(ui->devareatext->toPlainText());
        QTextStream savefilestream(&savefile);
        savefile.open(QIODevice :: WriteOnly | QIODevice :: Text);
        savefilestream << savedata;
        savefilestream.flush();
        savefile.close();}
        else {save();}
    compiler();
}


void DevIde::on_projectree_itemDoubleClicked(QListWidgetItem *item)
{
    ui->compilerouttext->append("File saved:"+ item->text()+"\n-------------------------------------------\n compiler result:\n");
    QFile savefile(item->text() + ".tmp");
    savefile.copy(item->text());
    compiler();//}
}


void DevIde::on_projectree_itemClicked(QListWidgetItem *item)
{
    QFile filenamestr(tmpfile);
    QTextStream filestream(&filenamestr);
    if(tmpfile != item->text() + ".tmp"){
        QString savedata(ui->devareatext->toPlainText());
        filenamestr.open(QIODevice :: WriteOnly | QIODevice :: Text);
        filestream << savedata;
        filestream.flush();
        filenamestr.close();
        ui->devareatext->clear();
        filenamestr.setFileName(item->text() + ".tmp");
        filenamestr.open(QIODevice :: ReadOnly | QIODevice :: Text);
        ui->devareatext->append(filestream.readAll());
        filenamestr.close();
        tmpfile=item->text() + ".tmp";}



}

void DevIde:: codehightlight(QTextStream *test){
    //tesztelésre
      //
      QPalette palette;
      palette.setBrush(QPalette::BrightText,QColor(100,200,100));
      //palette.setBrush(QPalette::Text,QColor(100,125,144));
//      palette.setBrush(QPalette::PlaceholderText,QColor(100,100,100));
      palette.setBrush(QPalette::Highlight, QColor(145, 45, 120));
      palette.setBrush(QPalette::HighlightedText,Qt::NoBrush);
      ui->devareatext->setPalette(palette);
/**/


      QSyntaxHighlighter *documents = nullptr;
      documents->setDocument(ui->devareatext->document());
      QStringList codedata(test->readAll());
      QTextBlock txtblock;
     // txtblock=codedata;

      documents->rehighlightBlock(codedata);
      //teszteésre_end
}
