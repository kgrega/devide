#include "devide.h"
#include "compiler.h"
#include "qobjectdefs.h"
#include "runner.h"
#include "ui_devide.h"
#include "newdiag.h"
#include "highlighter.h"
#include <QProcess>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QListWidgetItem>
#include <QStringList>
#include <QString>
#include <QMessageBox>

#include <QTimer>


DevIde::DevIde(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DevIde)
{
    ui->setupUi(this);
    opened="0";
    Highlighter *highlighter = new Highlighter(ui->devareatext->document());

}

DevIde::~DevIde()
{
    on_actionExit_triggered();
    delete ui;

}


void DevIde::on_actionExit_triggered()
{
   if(opened != "0")
        save("file");
   if(QFile::exists(ui->projectree->currentRow() + ".tmp"))
       save("item");

   foreach(QString filename,filenames){
       //QMessageBox::information(this,"test",filename + ".tmp");
       QFile::remove(filename + ".tmp");}


   this->close();
}


void DevIde::on_actionSave_triggered()
{
    save("file");
    //tesztelésre
    QListWidgetItem *data=nullptr;
    if(ui->projectree->currentItem()!=nullptr){data=ui->projectree->currentItem();}

    if(opened == "0" || ui->compilerouttext->toPlainText()!="")
         save("file");
    if(QFile::exists(data->text() + ".tmp"))
        save("item");

    if(!filenames.isEmpty()){
    foreach(QString filename,filenames){
        //QMessageBox::information(this,"test",filename + ".tmp");
        QFile::remove(filename + ".tmp");}
        }
    //
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
    QFile openfile(QFileDialog ::getOpenFileName(this,"File open","/home/gr3g0ry/Asztal","C Files (*.c);;Header files(*.h);;Project files(*.project);;control file(control);;"));
    //
    QString openfilename(openfile.fileName());
    QString dirname(openfile.fileName());
    QStringList openfilenamelist;
    openfilenamelist=openfilename.split('/');
    openfilename=openfilenamelist.last();

    if(openfilenamelist.last()!="control"){
    ui->projectree->clear();
    openprojecttype="C";}

    if(openfilenamelist.last()=="control"){
        openprojecttype="debpackage";
        Debprojectopen(openfile.fileName());}
    else if(openfilenamelist.last()!="control"){
        openfilenamelist=openfilename.split('.');
        ui->compilerouttext->setText(openfilenamelist.last());
        if(openfilenamelist.last()=="project"){
            openprojectfilename=openfile.fileName();
            openfilenamelist=dirname.split(openfilename);
            dirname=openfilenamelist.first();
            ui->compilerouttext->setText(dirname);
            projectopen(openfilename,dirname);}
         else{
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
            filenamedata=openfile.fileName();}
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
    QListWidgetItem *item(ui->projectree->currentItem());
    QListWidgetItem *list;
    QFile filenamestr(item->text());
    QProcess comp;
    QString what,dirstr(filenamestr.fileName());
    QStringList arguments,dirlist(dirstr.split("DEBIAN/control"));


    if(openprojecttype=="debpackage"){
      comp.setProgram("dpkg-deb");
        arguments.append("-b");}

    if(opened == "1"){
    list = ui->projectree->currentItem();
    if(QFile::exists(list->text()+ ".tmp")){
        what = list->text()+ "tmp";}
    else {
        what = list->text();}
    arguments.append(list->text());}
    else{
       what=filecompilername;
       arguments.append(what);}

    if(filecompilername.isEmpty()){filecompilername.append(filenamedata);}
    QStringList dir(what.split('.'));
    if(openprojecttype!="debpackage"){
    comp.setProgram("gcc");
    arguments.append("-o");
    arguments.append(dir.first());}
    if(openprojecttype=="debpackage"){
    arguments.append(dirlist.first());}
    comp.start();//"gcc",arguments);
    comp.waitForFinished(100000);
    //
    foreach (QString data, arguments) {
        ui->compilerouttext->append("Arguments: " + data);}
    //

    ui->compilerouttext->append(comp.readAllStandardError());
    ui->compilerouttext->append(comp.readAllStandardOutput());
    comp.kill();
    arguments.clear();
    if(!comp.exitCode()){
    ui->compilerouttext->append("Running application one moment...");
    running(dir.first());}
    else
    ui->compilerouttext->append("Compilering failuled...");
}

void DevIde :: running(QString appname){
    QStringList datalist;
    QString data,terminal,arg;
    //
    QFile runningoptfile(QDir::currentPath() + "/profiles/run.opt");
    QTextStream runningoptfilestr(&runningoptfile);
    runningoptfile.open(QIODevice:: ReadOnly | QIODevice:: Text);
    data=runningoptfilestr.readLine();
    datalist=data.split('=');
    terminal=datalist.last();
    data=runningoptfilestr.readLine();
    datalist=data.split('=');
    arg=datalist.last();
    ui->compilerouttext->append("Running with: " + terminal + "|");
    ui->compilerouttext->append("Running: " + appname);

    data.clear();
    QProcess run;
    QStringList arguments;
    arguments << arg.split(";") << "-e" << appname;
    //
    foreach (QString datastr, arguments) data+=datastr + "|";
    ui->compilerouttext->append("terminal running datas: " + terminal + data);
    run.start(terminal,arguments);
    run.waitForFinished(-1);}


void DevIde :: save(QString type){
    int i=0;
    QFile savefile,missingfile;
    QDir savedir;
    QStringList savedirlist(openprojectfilename.split(openprojectname + ".project")),newfilelist;
    QListWidgetItem *data(ui->projectree->currentItem());
    QString savedata(ui->devareatext->toPlainText()),saveprojectfilename,newfile;
    QTextStream savefilestream(&savefile);
    //////////////////////////////////////////////////////////////////////////////////////
    savedirlist.removeLast();
    saveprojectfilename=savedirlist.last();
    savedir.setPath(saveprojectfilename);
    ui->compilerouttext->setText(savedirlist.first());

   if(openprojecttype=="C" or openprojecttype=="C++"){
       savefile.setFileName(openprojectfilename);
       filecompilername=savefile.fileName();
       savefile.open(QIODevice:: WriteOnly | QIODevice:: Text);
       savefilestream << "NAME=" + openprojectname + "\n";
       savefilestream << "TYPE=" + openprojecttype + "\n";
       data=ui->projectree->item(i);
            while(data!=nullptr){
                newfile=data->text();
                newfilelist=newfile.split('/');
                newfile=newfilelist.last();
                //debug_code
                if(!QFile::exists(data->text())){
                  ui->compilerouttext->append("A projektfájlban megadott állomány '" + newfile+ "' nem létezik. A projektfájlból el lett távolítva!");

                }else{
                  savefilestream << "FILE=" + newfile + "\n";
                  QFile::copy(data->text() + ".tmp",savedirlist.first() + "/" + newfile);}
                data=ui->projectree->item(i++);}
   //

   savefile.close();
   savefilestream.flush();}
   else{
    if(type == "file"){
        savefile.setFileName(QFileDialog :: getSaveFileName(this,"File Save","/home/gregory","C Files (*.c,*.cpp);;Header files(*.h);;Project files(*.project);;"));
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
            savefile.copy(QFileDialog :: getSaveFileName(this,"File Save","/home/gregory","C Files (*.c,*.cpp);;Header files(*.h);;"));}}
   }
   /////tesztelésre
   if(opened!='1'){opened='1';ui->projectree->addItem(savefile.fileName());}
}


void DevIde::on_actionBuild_triggered()
{
    if(ui->projectree->currentItem() == nullptr && opened=="0"){ui->compilerouttext->append("Nincs elmentve egyetlen forráskód sem! Mentsen el egyet!");}
    else{
    if(opened=="0"){
        save("file");
    compilernew();}
    if(ui->projectree->currentItem() == nullptr){ui->devareatext->append("Nincs elmentve egyetlen forráskód sem! Mentsen el egyet!");
    save("item"); on_projectree_itemDoubleClicked(ui->projectree->currentItem());}
    }

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

    //QFile::remove(item->text());

   //QFile::rename(item->text(),item->text()+".tmp");
    //compiler();//}
   compilernew();
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
    double resultvalue=0;
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
        output=outputsream.readLine();

        compare=resultstr.compare(output,Qt::CaseInsensitive);
        ui->compilerouttext->append("Program által adott eredmény:" + resultstr);
        //ui->compilerouttext->append(resultstr + result.setNum(compare));
        if(compare==1){
            step++;}}
    resultvalue=(step/i);

    ui->compilerouttext->append("Ellenőrzési napló:\nEredmény sikeressége:" + QString::number(step) + "/" + QString::number(i));
    ui->compilerouttext->append("Eredmény sikeressége:" + result.number((step*100)/i,10) + '%');
    //ui->compilerouttext->append("Eredmény sikeressége:" + result.number((step/i),'d',2));
    if(result.number((step*100)/i,10)=="100"){
        ui->compilerouttext->append("A forráskód a teszteseteken ugyanazt a eredményt kapta vissza! Gratula! :)");}
    else{
        ui->compilerouttext->append("A forráskód nem tökéletes! Egyes teszteseteken elbukott! Tekintse át a kódot és a ellenőrzési naplót!");}

    }


void DevIde::on_actionCode_debug_triggered()
{
   if(!(QFile::exists(ui->inputstr->text()) && QFile::exists(ui->outputstr->text()))) {ui->compilerouttext->append("Üres kimenet/bemeneti fájl!");
    }else{
    QListWidgetItem *list;
    list = ui->projectree->currentItem();
    if(list!=nullptr){
    QString str=list->text();
    QStringList data=str.split('.');
    compiler();

    commands(data.first(), ui->inputstr->text(), ui->outputstr->text());}
    else{ui->compilerouttext->append("Jelöljön ki egy forráskodot a folytatáshoz!");}
}}


void DevIde::on_actionNew_triggered()
{ 

    Newdiag *newprojects=new Newdiag();
    newprojects->show();

    //newproject();
    /// //
//    ui->devareatext->append(newprojects->datastr());
    ///
    ///
    connect(newprojects,SIGNAL(accepted()),this,SLOT(newproject()));

}


void DevIde::newproject(){
    //módosított_javított_és tényleg_2024_02_26
    QStringList projectfilenamelist,projectfilenamepathlist,gitprojectfilenamepathlist;
    QDir githubdir,projectdir;
    QString projectfilenamedata,projectfilenamepath,projectfilenameproject,gitpath;
    QFile projectfilename(QDir::currentPath()+"/project");
    QTextStream projectfilenamestr(&projectfilename);

    ////errefelé kellene keresni a probémát
    if(QFile::exists(QDir::currentPath() +  "/gitproject")){
        projectfilename.setFileName(QDir::currentPath() +  "/gitproject");}
    QMessageBox::information(this,"",QDir::currentPath() +  "/gitproject");
    projectfilename.open(QIODevice:: ReadOnly | QIODevice:: Text);
    projectfilenamedata=projectfilenamestr.readLine();
    projectfilenamelist=projectfilenamedata.split(".project");
    projectfilenamepath=projectfilenamelist.first();
    projectfilenamelist=projectfilenamepath.split('/');
    projectfilenameproject=projectfilenamelist.last();
    projectfilenamelist=projectfilenamedata.split("/" + projectfilenameproject + ".project");


    if(QFile::exists(QDir::currentPath() +  "/gitproject") && (!QFile::exists(QDir::currentPath() + "/addfile"))){
        gitprojectopen(projectfilenamedata);}
    else{
        if(projectfilenameproject=="control" && projectfilenameproject + ".project" != "control"){//wtf?_eredetilegnemmodosítottam volna
            Debprojectopen(projectfilenamelist.first()+ "/" + projectfilenameproject);}
        else if(!QFile::exists(QDir::currentPath() + "/addfile")){
            projectopen(projectfilenameproject + ".project",projectfilenamelist.first());}
    }
    projectfilename.close();
    //modositott_2024_02_27----------------------------------------------
    if(QFile::exists(QDir::currentPath() + "/addfile")){
        projectfilename.setFileName(QDir::currentPath() +"/addfile");
        projectfilename.open(QIODevice:: ReadOnly | QIODevice:: Text);
        projectfilenamedata=projectfilenamestr.readLine();
        projectdir.setCurrent(projectfilenamedata);
        projectfilenamelist=projectfilenamedata.split("/");
        projectfilenameproject=projectfilenamelist.last();
        projectfilenamelist=projectfilenamedata.split("/" + projectfilenameproject);
        fileopen(projectfilenameproject,projectfilenamelist.first());}
}

void DevIde::projectopen(QString projectfilename, QString path){

    QString data,filename;
    QStringList fullline;
    QDir projectpath(path);
    QFile projectfile(path + projectfilename),openfile;
    QTextStream projectfilestr(&projectfile);
    projectfile.open(QIODevice:: ReadOnly |QIODevice:: Text);
    ui->projectree->clear();
    while(!projectfilestr.atEnd()){
    data=projectfilestr.readLine();
    fullline=data.split("=");

    if(fullline.first()=="NAME"){
    openprojectname=fullline.last();}
    if(fullline.first()=="FILE"){
    filename=fullline.last();
    ui->devareatext->clear();
    openfile.setFileName(path + filename);
    QTextStream openfilestream(&openfile);
    openfile.open(QIODevice :: ReadOnly | QIODevice :: Text);
    ui->devareatext->append(openfilestream.readAll());
    ui->projectree->addItem(openfile.fileName());
    opened= "1";
    tmpfile.append(openfile.fileName()+".tmp");
    openfile.copy(openfile.fileName()+".tmp");
    openfile.close();}
    if(fullline.first()=="TYPE"){
    openprojecttype=fullline.last();}
    }

   }

void DevIde::on_actionRunning_triggered()
{
    Runner *runningopt=new Runner();
    runningopt->show();
}

void::DevIde::Debprojectopen(QString projectfilename){
    QString filename;
    QStringList debfilelist(projectfilename.split("control"));
    QDir projectpath(debfilelist.first());
    QFile projectfile;
    QTextStream projectfilestr(&projectfile);
    ui->projectree->clear();
    debfilelist=projectpath.entryList(QDir::Files);

    foreach (QString data, debfilelist) {
        fileopen(data, projectpath.path()+ "/");
    }

    openprojecttype="debpackage";}

/////////////////////////////////////////////
/// /////////////////////////////////////////////
/// /////////////////////////////////////////////
/// /////////////////////////////////////////////
/// /////////////////////////////////////////////
/// /////////////////////////////////////////
/// ///////////////

void DevIde::on_actionComplirer_triggered()
{
    Compiler *compiler=new Compiler();
    compiler->show();
}

/////////////////////////////////////////////

void DevIde::compilernew(){

    QFile configfile;
    QTextStream configfilestr(&configfile);
    QListWidgetItem *item(ui->projectree->currentItem());
    QListWidgetItem *list;
    QFile filenamestr(item->text());
    QProcess comp;
    QString argument,what,dirstr(filenamestr.fileName());
    QStringList argumentstr,arguments,dirlist(dirstr.split("/DEBIAN/control"));


    if(openprojecttype=="debpackage"){
      configfile.setFileName(QDir::currentPath() + "/profiles/deb.opt");
      configfile.open(QIODevice:: ReadOnly | QIODevice:: Text);
      while(!configfilestr.atEnd()){
          argument=configfilestr.readLine();
          argumentstr=argument.split('=');
          if(argumentstr.last()=='1'){arguments.append(argumentstr.first());}
      }arguments.append("-b");
      arguments.append(dirlist.first());

        configfile.close();//debian_version_END

        //teszter
           foreach (QString data, arguments) {
               ui->compilerouttext->append("Arguments_eleje:: " + data);}
           //teszter_END
          comp.start("dpkg-deb",arguments);
          comp.waitForFinished(-1);
    }
//DEBIAN PACKAGE--------------------------------------------end

   //GCC Compiler___________________________________________________________
    if(openprojecttype!="debpackage"){
     //comp.setProgram("gcc");
     configfile.setFileName(QDir::currentPath() + "/profiles/gcc.opt");
     configfile.open(QIODevice:: ReadOnly | QIODevice:: Text);
     argument=configfilestr.readLine();
      while(argument!="[warnings]"){
          argumentstr=argument.split('=');
          if(argumentstr.last()=='1'){
              if(argumentstr.first()!="|none"){arguments.append(argumentstr.first());}}
    argument=configfilestr.readLine();}

          while(!configfilestr.atEnd()){
             argument=configfilestr.readLine();
              argumentstr=argument.split('*');
              if(argumentstr.last()=='1'){
                  argument=argumentstr.first();
                  argumentstr=argument.split('|');
                  arguments.append(argumentstr.first());}}
                }//gcc_END
    //GCC Compiler___________________________________________________________END
//wtf?
    if(opened == "1"){
    list = ui->projectree->currentItem();
    if(QFile::exists(list->text()+ ".tmp")){
        what = list->text()+ "tmp";}
    else {
        what = list->text();}
    arguments.append(list->text());}
    else{
       what=filecompilername;
       arguments.append(what);}
    if(filecompilername.isEmpty()){filecompilername.append(filenamedata);}
    QStringList dir(what.split('.'));
    arguments.append("-o");
    arguments.append(dir.first());
//wtf?_END


    //teszter
       foreach (QString data, arguments) {
           ui->compilerouttext->append("Arguments_eleje:: " + data);}
       //teszter_END

        if(openprojecttype!="debpackage"){
    comp.start("gcc",arguments);
    comp.waitForFinished(-1);}
    //
    ui->compilerouttext->append(comp.readAllStandardError());
    ui->compilerouttext->append(comp.readAllStandardOutput());
    comp.kill();
    arguments.clear();
    if(!comp.exitCode()){
    if(openprojecttype!="debpackage"){
            ui->compilerouttext->append("Running application one moment...");
            running(dir.first());}
    else{   ui->compilerouttext->append("A fordítás befejezve a kész állományt itt megtalálhatja: "+ dirlist.first() + ".deb");}
        }else{
        ui->compilerouttext->append("Compilering failuled...");}
}



//Modosított verzio_2024_02_25__2024_02_26_////2024_03_01
void::DevIde::gitprojectopen(QString gitpath){

    //javított_veglegesen 2023_10_31
    QString data,datastring,filename;
    QStringList filenames;
    QDir projectpath(gitpath);

    ui->projectree->clear();
    ui->devareatext->clear();
    filenames=projectpath.entryList(QDir::AllEntries);

    foreach(QString file,filenames){
        if(file=="." || file==".." ){}
        else{
            QMessageBox::information(this,"",file);
            fileopen(file,gitpath);}}
}

//_gitegyelet


//openfile_2024_02_26
void DevIde::fileopen(QString openfilename, QString path){

    QFile openfile(path + openfilename);
    QTextStream openfilestream(&openfile);

    openfile.copy(openfile.fileName()+".tmp");
    openfile.copy(openfile.fileName()+".bckp");
    openfile.open(QIODevice :: ReadOnly | QIODevice :: Text);

    ui->projectree->addItem(path + openfilename);
    ui->devareatext->setPlainText(openfilestream.readAll());
    opened= "1";
    //
    openfile.copy(openfile.fileName()+".tmp");
    openfile.copy(openfile.fileName()+".bckp");
    openfile.close();
}
//openfile_END

//savefile____2024_02_29
void DevIde::filesave(QString oldname, QString newname,QString devareastring){
    //////////////////////////////////////////////////////////////////////
    QFile newfile(newname),oldfile(oldname);
    QTextStream newfilestr(&newfile),oldfilestr(&oldfile);//oldfilestr???
    QString newstr,oldstr;//oldstr??
    //int compare;
    if(devareastring==""){
        QFile::copy(oldfile.fileName() + ".tmp",newfile.fileName());
        QFile::remove(oldfile.fileName()+".tmp");
        QFile::remove(oldfile.fileName());
    }
    else{
        newfile.open(QIODevice :: WriteOnly | QIODevice :: Text);
        newfilestr << devareastring;
        newfilestr.flush();
        newfile.close();}
}   //openfile_END
