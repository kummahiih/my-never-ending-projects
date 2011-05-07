#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QtGui>
#include "contractwidget.h"


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:
    void openFile();
    void saveFile();

private slots:
    void updateActions(const QItemSelection &selection);



protected:
    //void changeEvent(QEvent *e);

private:

    void createMenus();

    ContractWidget *iContractWidget;
    QMenu *fileMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    /*
    QMenu *toolMenu;

    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;*/
};

#endif // MAINWINDOW_H
