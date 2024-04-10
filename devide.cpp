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
    ui->maintoolBar->addAction(ui->actionNew);
    ui->maintoolBar->addAction(ui->actionOpen);
    ui->maintoolBar->addAction(ui->actionSave);
    ui->maintoolBar->addAction(ui->actionExit);
    ui->maintoolBar->addSeparator();
    ui->maintoolBar->addAction(ui->actionUndo);
    ui->maintoolBar->addAction(ui->actionRedo);
    ui->maintoolBar->addSeparator();
    ui->maintoolBar->addAction(ui->actionPaste);
    ui->maintoolBar->addAction(ui->actionCopy);
    ui->maintoolBar->addAction(ui->actionCut);
    ui->maintoolBar->addSeparator();
    ui->maintoolBar->addAction(ui->actionBuild);
    ui->maintoolBar->addAction(ui->actionRun);
    ui->maintoolBar->addAction(ui->actionCode_debug);
    ui->maintoolBar->addSeparator();
    ui->maintoolBar->addAction(ui->actionRun);
    ui->maintoolBar->addAction(ui->actionComplirer);
    ui->dockWidget->hide();
    QFile::remove(QDir::currentPath()+ "/addfile");
    QFile::remove(QDir::currentPath()+ "/project");
    QFile::remove(QDir::currentPath()+ "/gitproject");
    guioptions();
}

DevIde::~DevIde()
{
    on_actionExit_triggered();
    delete ui;

}


void DevIde::on_actionExit_triggered()
{
    QListWidgetItem *dataitem;
    if(opened != "0")
        on_actionSave_triggered();
    if(QFile::exists(ui->projectree->currentRow() + ".tmp"))
        on_actionSave_triggered();

    for(int i=0;i<ui->projectree->count();i++){
        dataitem=ui->projectree->item(i);
        QFile::remove(dataitem->text() + ".tmp");}


    this->close();
}


void DevIde::on_actionSave_triggered()
{
    //////
    int i=0;
    QFile savefile,missingfile;
    QDir savedir;
    QStringList savedirlist,newfilelist;
    QListWidgetItem *data(ui->projectree->currentItem());
    QString savedata(ui->devareatext->toPlainText()),saveprojectfilename,newfile;
    QTextStream savefilestream(&savefile);

    //////
    for(int i=0;i<ui->projectree->count();i++){
        data=ui->projectree->item(i);
        this->on_projectree_itemClicked(data);
    }
    //////////

    /// teljesenműködik__perfekt
    if(openprojecttype=="C" or openprojecttype=="C++"){
        savefile.setFileName(openprojectfilename);
        filecompilername=savefile.fileName();
        savefile.open(QIODevice:: WriteOnly | QIODevice:: Text);
        savefilestream << "NAME=" + openprojectname + "\n";
        savefilestream << "TYPE=" + openprojecttype + "\n";
        for(int i=0;i<ui->projectree->count();i++){
            data=ui->projectree->item(i);
            savedir.setPath(data->text());
            newfile=data->text();
            newfilelist=newfile.split('/');
            newfile=newfilelist.last();


                savefilestream << "FILE=" + newfile + "\n";
                filesave(data->text() + ".tmp",data->text(),"");
                }
            savefile.close();
            savefilestream.flush();
            return;}

    /// teljesenműködik__perfekt_de_tesztelendő
    ////////////////debian_git_jellegű mentés
    if(openprojecttype=="debpackage" or openprojecttype=="gitproject"){
        for(int i=0;i<ui->projectree->count();i++){
            data=ui->projectree->item(i);
            newfile=data->text();
            newfilelist=newfile.split('/');
            newfile=newfilelist.last();

                filesave(data->text() + ".tmp",data->text(),"");}

            savefile.close();
            savefilestream.flush();
            return;}
    //////////////////////////////////////////////

    /// javítandó
    if(opened == "0"){
        if(ui->devareatext->toPlainText()!=""){
            data=ui->projectree->item(0);

            if(data==nullptr){
                ui->projectree->clear();}
            savefile.setFileName(QFileDialog :: getSaveFileName(this,"File Save(mert üres a fájl)","/home/gregory","C Files (*.c);;C++ Files (*.cpp);;Header files(*.h);;Project files(*.project);;"));
            filesave("",savefile.fileName(),ui->devareatext->toPlainText());
            fileopen(savefile.fileName(),"");
            opened= "1";
            savedir.setPath(savefile.fileName());
            openprojectfilename=savedir.path();
            return;}}

    else if(opened=="1"){
        i=0;
        data=ui->projectree->item(i);
        while(data!=nullptr){
            filesave(data->text() + ".tmp",data->text(),"");
            data=ui->projectree->item(i++);}
        return;}
    //////////


    ////////////////////////////////////////////
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
    QFile openfile(QFileDialog ::getOpenFileName(this,"File open","/home/gr3g0ry/Asztal","C Files (*.c);;C++ Files (*.cpp);;Header files(*.h);;Project files(*.project);;control file(control);;"));
    if(!openfile.exists()){return;}
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
    QFile projectfile;
    QTextStream projectfilestr(&projectfile);
    if(openprojecttype=="C" or openprojecttype=="C++" or openprojecttype=="debpackage"){
        projectfile.setFileName(QDir::currentPath()+ "/project");
        projectfile.open(QIODevice::WriteOnly | QIODevice:: Text);
        projectfilestr << dirname;
        projectfilestr.flush();
        projectfile.close();}
on_projectree_itemClicked(ui->projectree->item(0));}


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
    if(!QFile::exists(QDir::currentPath()+ "/profiles/gcc.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/gcc.opt" +") nem létezik!");
        configfilemaker();
        return;}
    if(!QFile::exists(QDir::currentPath()+ "/profiles/deb.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/deb.opt" +") nem létezik!");
        configfilemaker();
        return;}

    if(!QFile::exists(QDir::currentPath()+ "/profiles/run.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/run.opt" +") nem létezik!");
        configfilemaker();
        return;}

    QListWidgetItem *compiler;
    if(ui->projectree->currentItem() == nullptr && opened=="0"){ui->compilerouttext->append("Nincs elmentve egyetlen forráskód sem! Mentsen el egyet!");}
    else{
        on_actionSave_triggered();
        if(ui->projectree->currentItem() == nullptr){
            ui->projectree->setCurrentRow(0);}
        //compiler=ui->projectree->currentItem();

        //compilernew();
        on_projectree_itemDoubleClicked(ui->projectree->currentItem());}

}


void DevIde::on_projectree_itemDoubleClicked(QListWidgetItem *item)
{
    if(!QFile::exists(QDir::currentPath()+ "/profiles/gcc.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/gcc.opt" +") nem létezik!");
        configfilemaker();
        return;}
    if(!QFile::exists(QDir::currentPath()+ "/profiles/deb.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/deb.opt" +") nem létezik!");
        configfilemaker();
        return;}

    if(!QFile::exists(QDir::currentPath()+ "/profiles/run.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/run.opt" +") nem létezik!");
        configfilemaker();
        return;}

    QFile filenamestr(item->text());
    QTextStream filestream(&filenamestr);
        QString savedata(ui->devareatext->toPlainText());
        filenamestr.open(QIODevice :: WriteOnly | QIODevice :: Text);
        filestream << savedata;
        filestream.flush();
        filenamestr.close();
    ui->compilerouttext->append("File saved:"+ item->text()+"\n-------------------------------------------\n compiler result:\n");

    //

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

    if(!QFile::exists(QDir::currentPath()+ "/profiles/run.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/run.opt" +") nem létezik!");
        configfilemaker();
        return;}

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
    if(!QFile::exists(QDir::currentPath()+ "/profiles/gcc.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/gcc.opt" +") nem létezik!");
        configfilemaker();
        return;}
    if(!QFile::exists(QDir::currentPath()+ "/profiles/deb.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/deb.opt" +") nem létezik!");
        configfilemaker();
        return;}

    if(!QFile::exists(QDir::currentPath()+ "/profiles/run.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/run.opt" +") nem létezik!");
        configfilemaker();
        return;}

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
    //módosított_javított_és tényleg_2024_02_26_2024_03_10
    QStringList projectfilenamelist,projectfilenamepathlist,gitprojectfilenamepathlist;
    QDir githubdir,projectdir;
    QString projectfilenamedata,projectfilenamepath,projectfilenameproject,gitpath;
    QFile projectfilename(QDir::currentPath()+"/project");
    QTextStream projectfilenamestr(&projectfilename);

    ////
    //módosított_javított_és tényleg_2024_03_10
    if(QFile::exists(QDir::currentPath() +  "/project")){
        projectfilename.setFileName(QDir::currentPath() +  "/project");}
    /////módosított_javított_és tényleg_2024_03_10_END
    if(QFile::exists(QDir::currentPath() +  "/gitproject")){
        projectfilename.setFileName(QDir::currentPath() +  "/gitproject");}

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
            projectopen(projectfilenameproject + ".project",projectfilenamelist.first() + "/");}
    }
    projectfilename.close();
    //modositott_2024_03_10----------------------------------------------
    if(QFile::exists(QDir::currentPath() + "/addfile")){
        projectfilename.setFileName(QDir::currentPath() +"/addfile");
        projectfilename.open(QIODevice:: ReadOnly | QIODevice:: Text);
        projectfilenamedata=projectfilenamestr.readLine();
        projectfilenameproject=projectfilenamedata;
        projectfilenamelist=projectfilenamedata.split(".project");
        if(projectfilenamelist.last()== ".project"){
            fileopen("",projectfilenameproject);}
        else{
            ui->projectree->addItem(projectfilenameproject);
            ui->projectree->setCurrentRow(ui->projectree->count()-1);}}}

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

        if(fullline.first()=="TYPE"){
            openprojecttype=fullline.last();}
        if(fullline.first()=="NAME"){
            openprojectname=fullline.last();}

        if(fullline.first()=="FILE"){
            fileopen(fullline.last(),path);}
    }
    //openprojecttype="CCPLUSPLUS";
    openprojectfilename=path + projectfilename;

   }

void DevIde::on_actionRunning_triggered()
{

    if(!QFile::exists(QDir::currentPath()+ "/profiles/run.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/run.opt" +") nem létezik!");
        configfilemaker();
        return;}

    Runner *runningopt=new Runner();
    runningopt->show();
}

void::DevIde::Debprojectopen(QString projectfilename){
    QString filename;
    QStringList debfilelist(projectfilename.split("control"));
    QDir projectpath(debfilelist.first());
    QFile projectfile;
    QTextStream projectfilestr(&projectfile);

    debfilelist=projectpath.entryList(QDir::Files);

    if(!QFile::exists(QDir::currentPath()+ "/addfile")){
        ui->projectree->clear();
        foreach (QString data, debfilelist) {
            fileopen(data, projectpath.path()+ "/");}
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
    if(!QFile::exists(QDir::currentPath()+ "/profiles/gcc.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/gcc.opt" +") nem létezik!");
        configfilemaker();
        return;}
    if(!QFile::exists(QDir::currentPath()+ "/profiles/deb.opt")){
        ui->compilerouttext->append("A konfigurációs állomány("+ QDir::currentPath()+ "/profiles/deb.opt" +") nem létezik!");
        configfilemaker();
        return;}


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
        for(int i=0;i<ui->projectree->count();i++){
            list=ui->projectree->item(i);
            on_projectree_itemClicked(list);}
        for(int i=0;i<ui->projectree->count();i++){
            list=ui->projectree->item(i);
            QFile::remove(list->text()+ ".tmp");}
        ui->projectree->setCurrentItem(item);

        configfile.setFileName(QDir::currentPath() + "/profiles/deb.opt");
      configfile.open(QIODevice:: ReadOnly | QIODevice:: Text);
      while(!configfilestr.atEnd()){
          argument=configfilestr.readLine();
          argumentstr=argument.split('=');
          if(argumentstr.last()=='1'){arguments.append(argumentstr.first());}
      }arguments.append("-b");
      arguments.append(dirlist.first());

        configfile.close();//debian_version_END

        //
          comp.start("dpkg-deb",arguments);
          comp.waitForFinished(-1);
          for(int i=0;i<ui->projectree->count();i++){
              list=ui->projectree->item(i);
              QFile::copy(list->text(),list->text()+ ".tmp");}

          ui->projectree->setCurrentItem(item);
          on_projectree_itemClicked(ui->projectree->currentItem());

    }
//DEBIAN PACKAGE--------------------------------------------end

   //GCC Compiler___________________________________________________________
    if(openprojecttype!="debpackage"){
     //
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
//
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
//_END



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

            fileopen(file,gitpath);}}
}

//_gitegyelet


//openfile_2024_02_26
void DevIde::fileopen(QString openfilename, QString path){

    QFile openfile(path + openfilename);
    QTextStream openfilestream(&openfile);

    if(QFile::exists(openfile.fileName() + ".tmp")){
        ui->compilerouttext->append("A megadott fájl(" + openfile.fileName() + ")használata során a program váratlanul megállt.");
        ui->compilerouttext->append("Mentés nem történt,így egy szerkeztett állapot került megnyitásra! Kérem szükség esetén mielőbb mentsen!");}


    openfile.open(QIODevice :: ReadOnly | QIODevice :: Text);

    ui->projectree->addItem(path + openfilename);
    ui->devareatext->setPlainText(openfilestream.readAll());
    opened= "1";
    //
    openfile.copy(openfile.fileName()+".tmp");
//
    openfile.close();
}
//openfile_END

//savefile____2024_02_29
void DevIde::filesave(QString oldname, QString newname,QString devareastring){
    QFile newfile(newname),oldfile(oldname);
    QTextStream newfilestr(&newfile),oldfilestr(&oldfile);//oldfilestr???
    QString newstr,oldstr;//oldstr??
    //int compare;
    if(devareastring==""){
        QFile::remove(newfile.fileName());
        QFile::copy(oldfile.fileName(),newfile.fileName());
        //

    }
    else{
        newfile.open(QIODevice :: WriteOnly | QIODevice :: Text);
        newfilestr << devareastring;
        newfilestr.flush();
        newfile.close();}
    //filesve-----------------------------END

}   //openfile_END

//////////////////////********************************
void DevIde::configfilemaker(){
    QFile deb(QDir::currentPath() + "/profiles/deb.opt"),gcc(QDir::currentPath() + "/profiles/gcc.opt"),run(QDir::currentPath() + "/profiles/run.opt");
    QTextStream debstr(&deb),gccstr(&gcc),runstr(&run);
    QDir optpath(QDir::currentPath());
    optpath.mkdir(QDir::currentPath() + "/profiles");
    if(!QFile::exists(deb.fileName())){
        deb.open(QIODevice:: WriteOnly | QIODevice:: Text);
        debstr << "--verbose=0\n--debug=0";
        deb.flush();
        deb.close();}
    if(!QFile::exists(run.fileName())){
        run.open(QIODevice:: WriteOnly | QIODevice:: Text);
        runstr << "PROGRAM=\nARGUMENTS=";
        run.flush();
        run.close();}
    if(!QFile::exists(gcc.fileName())){
        gcc.open(QIODevice:: WriteOnly | QIODevice:: Text);
        gccstr << "--verbose=0\n--debug=0\n--extra-warnings=0\n--all-warnings=0\n[double]\n-mlong-double-128|128=0\n-mlong-double-64|64=0\n-mlong-double-80|80=0\n/-/\n[compat]\n|none=1\n/-/\n[support]\n|none=1\n/-/\n[warnings]\n/-/";
        gcc.flush();
        gcc.close();}
    ui->compilerouttext->append("A konfigurációs állomány létrejött! Próbálja meg újra!");}
/////////////////////////////////////////////***********************************************************

void DevIde::on_actionDark_triggered()
{
    QFile saveopt(QDir::currentPath() + "/profiles/gui.opt");
    QTextStream saveoptstr(&saveopt);
    saveopt.open(QIODevice:: WriteOnly | QIODevice::Text);
    saveoptstr << "background: #666666;" " color: none;\n";
    saveoptstr << "background-color: #a3a3a3;" " color:Black;\n";
    saveoptstr << "background: #8c8c8c;" " color: #000000;\n";
    saveoptstr << "background-color: #CCCCCC;" " color: Black;\n";

    saveoptstr.flush();
    saveopt.close();
    guioptions();

}


void DevIde::on_actionWhite_triggered()
{
    QFile saveopt(QDir::currentPath() + "/profiles/gui.opt");
    QTextStream saveoptstr(&saveopt);
    saveopt.open(QIODevice:: WriteOnly | QIODevice::Text);

    saveoptstr << "background-color: None }\n";
    saveoptstr << "background-color: None;" " color: none;\n";
    saveoptstr << "background-color: none;" " color:none;\n";
    saveoptstr << "background: none;" " color: none;\n";

    saveoptstr.flush();
    saveopt.close();
    guioptions();

}


void DevIde::on_actionPink_triggered()
{
    QFile saveopt(QDir::currentPath() + "/profiles/gui.opt");
    QTextStream saveoptstr(&saveopt);
    saveopt.open(QIODevice:: WriteOnly | QIODevice::Text);

    saveoptstr << "background-color: #b2478f;\n";
    saveoptstr << "background-color: #660066;" " color: White;\n";
    saveoptstr << "background-color: #ff66cc;" " color: #660066;\n";
    saveoptstr << "background: #660066;" " color: White;\n";


    saveoptstr.flush();
    saveopt.close();
    guioptions();

}

///////////////////////////////////////////////////
void DevIde::guioptions(){
    if(QFile::exists(QDir::currentPath() + "/profiles/gui.opt")){
        QFile saveopt(QDir::currentPath() + "/profiles/gui.opt");
        QTextStream saveoptstr(&saveopt);
        saveopt.open(QIODevice:: ReadOnly | QIODevice::Text);
        QString backgroundcolor(saveoptstr.readLine()),buttoncheckboxcolor(saveoptstr.readLine()),textlinecolor(saveoptstr.readLine()),listcolor(saveoptstr.readLine());
        saveopt.close();
        this->setStyleSheet(backgroundcolor);
        ui->devareatext->setStyleSheet(textlinecolor);
        ui->compilerouttext->setStyleSheet(textlinecolor);
        ui->projectree->setStyleSheet(textlinecolor);
        ui->inputstr->setStyleSheet(textlinecolor);
        ui->outputstr->setStyleSheet(textlinecolor);
        ui->inputbutton->setStyleSheet(buttoncheckboxcolor);
        ui->outputputton->setStyleSheet(buttoncheckboxcolor);}
    else{
        QDir optpath(QDir::currentPath());
        optpath.mkdir(QDir::currentPath() + "/profiles");}
}

void DevIde::on_actionClose_All_triggered()
{
    QListWidgetItem *dataitem;
    if(opened != "0")
        on_actionSave_triggered();
    if(QFile::exists(ui->projectree->currentRow() + ".tmp"))
        on_actionSave_triggered();


    for(int i=0;i<ui->projectree->count();i++){
        dataitem=ui->projectree->item(i);
        QFile::remove(dataitem->text() + ".tmp");}

    opened=0;
    ui->projectree->clear();
    ui->devareatext->clear();
    QFile::remove(QDir::currentPath()+ "/addfile");
    QFile::remove(QDir::currentPath()+ "/project");
    QFile::remove(QDir::currentPath()+ "/gitproject");

}


void DevIde::on_actionCode_tester_Properties_triggered()
{
    ui->dockWidget->show();
}


void DevIde::on_actionAbout_triggered()
{
    QMessageBox::about(this,"Névjegy","     DEVIDE \nVerzió: 1.00 \nFejlesztő: Kiss Gergely");
}


void DevIde::on_actionAbout_QT_triggered()
{
    QMessageBox::aboutQt(this,"QT információk");
}

