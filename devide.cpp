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
    on_actionExit_triggered();
    delete ui;

}


void DevIde::on_actionExit_triggered()
{
   if(!(opened != "1" && (!ui->devareatext->isUndoRedoEnabled())))
        save("file");
   if(ui->projectree->currentRow())
       save("item");

   foreach(QString filename,filenames){
       //QMessageBox::information(this,"test",filename + ".tmp");
       QFile::remove(filename + ".tmp");}


   this->close();
}


void DevIde::on_actionSave_triggered()
{
    save("file");
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
    filenames.append(openfile.fileName());
    ui->devareatext->clear();
    QTextStream openfilestream(&openfile);
    openfile.open(QIODevice :: ReadOnly | QIODevice :: Text);
    ui->devareatext->append(openfilestream.readAll());
    ui->projectree->addItem(openfile.fileName());
    opened= "1";
    tmpfile.append(openfile.fileName()+".tmp");
    openfile.copy(openfile.fileName()+".tmp");
    openfile.close();

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
    if(QFile::exists(list->text()+ ".tmp")){
        what = list->text()+ "tmp";}
    else {
        what = list->text();}
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
    //running(dir.first());
}

void DevIde :: running(QString appname){
    QProcess run;
    QStringList arguments(appname);
    QString inputfile(QFileDialog::getOpenFileName(this,"WTF?",""));
    //arguments.append();
    arguments.append("");
    run.setStandardInputFile(inputfile);
    run.start(appname,arguments);

/*    while(!run.waitForFinished(-1)){
        while(run.canReadLine()){
            ui->compilerouttext->append(run.readLine());}
    QByteArray userstdin("test");*/

    run.waitForFinished(10000);
    //connect(&run,SIGNAL(waitForReadyRead(-1)),&run,SLOT(write("test")));
    //run.write(userstdin);
    ui->compilerouttext->append(run.readAllStandardOutput());
//}

}

void DevIde :: save(QString type){
    QFile savefile;
    QListWidgetItem *data(ui->projectree->currentItem());
    QString savedata(ui->devareatext->toPlainText());
    QTextStream savefilestream(&savefile);
    if(type == "file"){
        savefile.setFileName(QFileDialog :: getSaveFileName(this,"File Save","/home/gregory","C Files (*.c,*.cpp);;Header files(*.h);;"));
        filecompilername=savefile.fileName();
        savefile.open(QIODevice :: WriteOnly | QIODevice :: Text);
        savefilestream << savedata;
        savefilestream.flush();
        savefile.close();}
    if(type =="item"){
        savefile.setFileName(data->text());
        savefile.open(QIODevice :: WriteOnly | QIODevice :: Text);
        savefilestream << savedata;
        savefilestream.flush();
        savefile.close();


   if(!QFile::exists(data->text() + ".tmp")){
            savefile.setFileName(data->text() + ".tmp");
            savefile.copy(QFileDialog :: getSaveFileName(this,"File Save","/home/gregory","C Files (*.c,*.cpp);;Header files(*.h);;"));}
}
}


void DevIde::on_actionBuild_triggered()
{
    QFile savefile;
    QListWidgetItem *data(ui->projectree->currentItem());
    if(opened!="1"){
        save("file");}

    if(data != nullptr){
    save("item");}
     compiler();
}


void DevIde::on_projectree_itemDoubleClicked(QListWidgetItem *item)
{
    QFile filenamestr(item->text());
    QTextStream filestream(&filenamestr);
        QString savedata(ui->devareatext->toPlainText());
        filenamestr.open(QIODevice :: WriteOnly | QIODevice :: Text);
        filestream << savedata;
        filestream.flush();
        filenamestr.close();
    ui->compilerouttext->append("File saved:"+ item->text()+"\n-------------------------------------------\n compiler result:\n");
   QFile::rename(item->text(),item->text()+".tmp");
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


void DevIde::on_actionRun_triggered()
{
    QListWidgetItem *data(ui->projectree->currentItem());
    if(data!=nullptr){
    QString runprogramcode(data->text());
    QStringList runrogramname(runprogramcode.split("."));

    if(QFile::exists(runrogramname.first())){
        ui->compilerouttext->append("Program running");
        running(runrogramname.first());}
    else
        ui->compilerouttext->append("Program is missing! You are compilered that source code?");
    }else
        ui->compilerouttext->append("First open source-code after compilered!");
}


void DevIde::on_outputputton_clicked()
{
    ui->outputstr->setText(QFileDialog::getOpenFileName(this,"Select outputfile"));
}


void DevIde::on_inputbutton_clicked()
{
    ui->inputstr->setText(QFileDialog::getOpenFileName(this,"Select inputfile"));
}


void DevIde::commands(QString appname, QString testinput, QString testoutput){
    QProcess tester;
    qint64 i=0,step=0;
    int compare;
    QString input,output,resultstr,result;
    QStringList argument,outputresult;

    tester.setProgram(appname);
    tester.setStandardInputFile(testinput);
    tester.open(QIODevice:: ReadOnly | QIODevice:: Text);
    QFile outputtestfile(testoutput);
    QTextStream outputsream(&outputtestfile);
    outputtestfile.open(QIODevice:: ReadOnly | QIODevice:: Text);
    tester.waitForFinished(-1);
    while(!outputsream.atEnd()){
      i++;
      resultstr=tester.readLine();
      ui->compilerouttext->append(resultstr + result.setNum(compare));
      output=outputsream.readLine();
      compare=resultstr.compare(output,Qt::CaseInsensitive);
      if(compare){
        step++;}

   }

    result.setNum((step/i));
        ui->compilerouttext->append("Eredmény sikeressége(i):" + result.setNum(i));
                ui->compilerouttext->append("Eredmény sikeressége(step):" + result.setNum(step));
    ui->compilerouttext->append("Eredmény sikeressége:" + result.setNum(step/i));




}
void DevIde::on_actionCode_debug_triggered()
{
    QListWidgetItem *list;
    list = ui->projectree->currentItem();
    if(list!=nullptr){
    QString str=list->text();
    QStringList data=str.split('.');
    compiler();
    commands(data.first(), ui->inputstr->text(), ui->outputstr->text());}
}

