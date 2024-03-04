#ifndef RUNNER_H
#define RUNNER_H

#include <QDialog>

namespace Ui {
class Runner;
}

class Runner : public QDialog
{
    Q_OBJECT

public:
    explicit Runner(QWidget *parent = nullptr);
    ~Runner();
    void runnerlistadder();

private slots:
    void on_savebutton_pressed();

private:
    Ui::Runner *ui;
};

#endif // RUNNER_H
