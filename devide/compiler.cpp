#include "compiler.h"
#include "qprocess.h"
#include "ui_compiler.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QProcess>

Compiler::Compiler(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Compiler)
{
    ui->setupUi(this);
    aboutdata();
    loadoptions();
    compatibility();
    warnings();
    supporting();
    doublesize();
}

Compiler::~Compiler()
{
    delete ui;
}


void Compiler::aboutdata(){
    QProcess about;
    QStringList aboutarg("--version");
    about.setProgram("dpkg-deb");
    about.setArguments(aboutarg);
    about.start();
    about.waitForFinished(-1);
    ui->debdpkg_about_data->setText(about.readAll());
    about.setProgram("gcc");
    about.start();
    about.waitForFinished(-1);
    ui->gcc_about_data->setText(about.readAll());

}

void Compiler::on_on_save_compiler_button_pressed()
{
//    QString debdpkgstr;
    QFile optfile(QDir::currentPath()+"/profiles/deb.opt");
    QTextStream optfilestr(&optfile);
    optfile.open(QIODevice:: WriteOnly| QIODevice:: Text);
    if(ui->deb_dpkg_verbose_enable->isChecked()){
        optfilestr << "--verbose=1\n";}
    else{
        optfilestr << "--verbose=0\n";}
    if(ui->deb_dpkg_debug_enable->isChecked()){
        optfilestr << "--debug=1\n";}
    else{
        optfilestr << "--debug=0\n";}
    optfile.flush();
    optfile.close();
    saveopt();
}

void Compiler::loadoptions(){
        QString optdatastring;
        QStringList optdatalist;
        QFile optfile(QDir::currentPath()+"/profiles/deb.opt");
        QTextStream optfilestr(&optfile);
        optfile.open(QIODevice:: ReadOnly| QIODevice:: Text);
        optdatastring=optfilestr.readLine();
        optdatalist=optdatastring.split("=");
            if(optdatalist.first()=="--verbose"){
                if(optdatalist.last()=="1")
                    ui->deb_dpkg_verbose_enable->setChecked(1);
                else
                    ui->deb_dpkg_verbose_enable->setChecked(0);
                }
        optdatastring=optfilestr.readLine();
        optdatalist=optdatastring.split("=");
            if(optdatalist.first()=="--debug"){
                if(optdatalist.last()=="1")
                    ui->deb_dpkg_debug_enable->setChecked(1);
                else
                    ui->deb_dpkg_debug_enable->setChecked(0);
                }
        optfile.close();
        optfile.setFileName(QDir::currentPath()+"/profiles/gcc.opt");
        optfile.open(QIODevice:: ReadOnly| QIODevice:: Text);
                        optdatastring=optfilestr.readLine();
                        optdatalist=optdatastring.split("=");
                        if(optdatalist.first()=="--verbose"){
                            if(optdatalist.last()=="1")
                                ui->gcc_verbose_enable->setChecked(1);
                            else
                                ui->gcc_verbose_enable->setChecked(0);
                        }
                        optdatastring=optfilestr.readLine();
                        optdatalist=optdatastring.split("=");
                        if(optdatalist.first()=="--debug"){
                            if(optdatalist.last()=="1")
                            ui->gcc_debug_enable->setChecked(1);
                            else
                             ui->gcc_debug_enable->setChecked(0);}

                        optdatastring=optfilestr.readLine();
                        optdatalist=optdatastring.split("=");
                        if(optdatalist.first()=="--extra-warnings"){
                            if(optdatalist.last()=="1")
                            ui->gcc_extra_warnings_enable->setChecked(1);
                            else
                             ui->gcc_extra_warnings_enable->setChecked(0);}

                        optdatastring=optfilestr.readLine();
                        optdatalist=optdatastring.split("=");
                        if(optdatalist.first()=="--all-warnings"){
                            if(optdatalist.last()=="1")
                            ui->gcc_all_warnings_enable->setChecked(1);
                            else
                             ui->gcc_all_warnings_enable->setChecked(0);}

}

void Compiler::supporting(){
    QString optdatastring,state;
    QStringList optdatalist,statelist;
    QFile optfile(QDir::currentPath()+"/profiles/gcc.opt");
    QTextStream optfilestr(&optfile);
    optfile.open(QIODevice:: ReadOnly| QIODevice:: Text);
    optdatastring=optfilestr.readLine();
    while(optdatastring!="[support]"){optdatastring=optfilestr.readLine();}
    optdatastring=optfilestr.readLine();

    while(optdatastring!="/-/"){
            optdatastring=optfilestr.readLine();
            if(optdatastring=="/-/"){}
                        else{
            optdatalist=optdatastring.split('=');
            if (optdatalist.last()=='1'){
                state=optdatalist.last();
                statelist=state.split('|');
            }
            optdatastring=optdatalist.first();
            optdatalist=optdatastring.split('|');
            ui->supportlist->addItem(optdatalist.last());}}
    ui->supportlist->currentTextChanged(optdatastring);
    optfile.close();}




void Compiler::warnings(){
    QString optdatastring,state;
    QStringList optdatalist,statelist;
    QFile optfile(QDir::currentPath()+"/profiles/gcc.opt");
    QTextStream optfilestr(&optfile);
    optfile.open(QIODevice:: ReadOnly| QIODevice:: Text);
    optdatastring=optfilestr.readLine();
    while(optdatastring!="[warnings]"){optdatastring=optfilestr.readLine();}
    optdatastring=optfilestr.readLine();

    while(optdatastring!="/-/"){
        optdatastring=optfilestr.readLine();
        if(optdatastring=="/-/"){}
        else{
            optdatalist=optdatastring.split('*');
            if(optdatalist.last()=='1'){
            optdatastring=optdatalist.first();
            optdatalist=optdatastring.split('|');
            ui->warnings_selected->addItem(optdatalist.last());}
            else{
                optdatastring=optdatalist.first();
                optdatalist=optdatastring.split('|');
                ui->warnings_select->addItem(optdatalist.last());}
    }}
    optfile.close();}


void Compiler::compatibility(){
    QString optdatastring,state;
    QStringList optdatalist,statelist;
    QFile optfile(QDir::currentPath()+"/profiles/gcc.opt");
    QTextStream optfilestr(&optfile);
    optfile.open(QIODevice:: ReadOnly| QIODevice:: Text);
    optdatastring=optfilestr.readLine();
    while(optdatastring!="[compat]"){optdatastring=optfilestr.readLine();}
    optdatastring=optfilestr.readLine();
    while(optdatastring!="/-/"){
        optdatastring=optfilestr.readLine();
        if(optdatastring=="/-/"){}
        else{
            optdatalist=optdatastring.split('=');
            if (optdatalist.last()=='1'){
                state=optdatalist.last();
                statelist=state.split('|');
                optdatastring=optdatalist.first();
                optdatalist=optdatastring.split('|');}
            else{
                optdatastring=optdatalist.first();
                optdatalist=optdatastring.split('|');
                ui->compiler_mode->addItem(optdatalist.last());}}

    ui->compiler_mode->currentTextChanged(optdatastring);}
    optfile.close();}

void Compiler::doublesize(){
    QString optdatastring,state;
    QStringList optdatalist;
    QFile optfile(QDir::currentPath()+"/profiles/gcc.opt");
    QTextStream optfilestr(&optfile);
    optfile.open(QIODevice:: ReadOnly| QIODevice:: Text);
    optdatastring=optfilestr.readLine();
    while(optdatastring!="[double]"){optdatastring=optfilestr.readLine();}

    while(optdatastring!="/-/"){
        optdatastring=optfilestr.readLine();
        if(optdatastring=="/-/"){}
        else{
            optdatalist=optdatastring.split('=');
            if (optdatalist.last()=='0'){
                optdatastring=optdatalist.first();
                optdatalist=optdatastring.split('|');
                ui->double_combo->addItem(optdatalist.last());}
            else{
                optdatastring=optdatalist.first();
                optdatalist=optdatastring.split('|');
                ui->double_combo->currentTextChanged(optdatastring);}
    }}
    optfile.close();
}

void Compiler::on_warnings_select_itemDoubleClicked(QListWidgetItem *item)
{
    ui->warnings_selected->addItem(item->text());
    ui->warnings_select->takeItem(ui->warnings_select->currentRow());
}


void Compiler::on_warnings_selected_itemDoubleClicked(QListWidgetItem *item)
{
    ui->warnings_select->addItem(item->text());
    ui->warnings_selected->takeItem(ui->warnings_selected->currentRow());

}

void Compiler::saveopt(){
    QString optdatastring,state;
    QStringList optdatalist;
    QListWidgetItem *item;
    int i=0;

    QFile optfile(QDir::currentPath()+"/profiles/gcc.opt"),opttmpfile(QDir::currentPath()+"/profiles/gcc.opt.tmp");
    QTextStream optfilestr(&optfile),opttmpfilestr(&opttmpfile);

    optfile.open(QIODevice:: ReadOnly| QIODevice:: Text);
    opttmpfile.open(QIODevice:: WriteOnly| QIODevice:: Text);
    //

    ////////////////////////////////////////////////////////////////////

        if(ui->gcc_verbose_enable->isChecked()){
            opttmpfilestr << "--verbose=1\n";}
        else{
            opttmpfilestr << "--verbose=0\n";}
        if(ui->gcc_debug_enable->isChecked()){
            opttmpfilestr << "--debug=1\n";}
        else{
            opttmpfilestr << "--debug=0\n";}
        if(ui->gcc_extra_warnings_enable->isChecked()){
            opttmpfilestr << "--extra-warnings=1\n";}
        else{
            opttmpfilestr << "--extra-warnings=0\n";}
        if(ui->gcc_all_warnings_enable->isChecked()){
            opttmpfilestr << "--all-warnings=1\n";}
        else{
            opttmpfilestr << "--all-warnings=0\n";}
    //

    optdatastring=optfilestr.readLine();
    while(optdatastring!="[double]"){optdatastring=optfilestr.readLine();}
    opttmpfilestr << "[double]\n";
    while(optdatastring!="/-/"){
        optdatastring=optfilestr.readLine();
        optdatalist=optdatastring.split('=');
        optdatastring=optdatalist.first();
        optdatalist=optdatastring.split('|');
            if(optdatastring=="/-/"){opttmpfilestr << optdatastring + "\n";}
        else{
        if (optdatalist.last()==ui->double_combo->currentText()){
            opttmpfilestr << optdatastring + "=1\n";}
        else {
            opttmpfilestr << optdatastring + "=0\n";}
            }}
    while(optdatastring!="[compat]"){optdatastring=optfilestr.readLine();}
    opttmpfilestr << "[compat]\n";
    while(optdatastring!="/-/"){
        optdatastring=optfilestr.readLine();
        optdatalist=optdatastring.split('=');
        optdatastring=optdatalist.first();
        optdatalist=optdatastring.split('|');
        if(optdatastring=="/-/"){opttmpfilestr << optdatastring + "\n";}
        else{
        if (optdatalist.last()==ui->compiler_mode->currentText()){
            opttmpfilestr << optdatastring + "=1\n";}
        else {
            opttmpfilestr << optdatastring + "=0\n";}
            }}

    while(optdatastring!="[support]"){optdatastring=optfilestr.readLine();}
    opttmpfilestr << "[support]\n";
    while(optdatastring!="/-/"){
        optdatastring=optfilestr.readLine();
        optdatalist=optdatastring.split('=');
        optdatastring=optdatalist.first();
        optdatalist=optdatastring.split('|');
        if(optdatastring=="/-/"){opttmpfilestr << optdatastring + "\n";}
        else{
        if (optdatalist.last()==ui->supportlist->currentText()){
            opttmpfilestr << optdatastring + "=1\n";}
        else {
            opttmpfilestr << optdatastring + "=0\n";}
            }}

////////////////////////////////
   while(optdatastring!="[warnings]"){optdatastring=optfilestr.readLine();}
   opttmpfilestr <<  "[warnings]\n";
   while(optdatastring!="/-/"){
        state=optdatastring=optfilestr.readLine();
        optdatalist=optdatastring.split('*');
        optdatastring=optdatalist.first();
        optdatalist=optdatastring.split('|');
    i=0;

    ui->warnings_selected->setCurrentRow(i);
    item=ui->warnings_selected->currentItem();

    while(item!=nullptr){
        if(item->text()==optdatalist.last()){
            state= optdatastring + "*1\n";break;}
        else{state= optdatastring + "*0\n";;}
            ui->gcc_about_data->append(item->text());
        i++;
        ui->warnings_selected->setCurrentRow(i);
        item=ui->warnings_selected->currentItem();}
        if(optdatastring!="/-/") opttmpfilestr << state; else opttmpfilestr << "/-/";
}
opttmpfile.close();
optfile.close();

QFile::remove(QDir::currentPath()+"/profiles/gcc.opt");
QFile::rename(QDir::currentPath()+"/profiles/gcc.opt.tmp",QDir::currentPath()+"/profiles/gcc.opt");

///function_end/vége
    delete this;
}


void Compiler::on_cancelButton_pressed()
{
     delete this;
}

