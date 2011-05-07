#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    iContractWidget = new ContractWidget;
    setCentralWidget(iContractWidget);
    createMenus();
    setWindowTitle(tr("Contract book"));
}

MainWindow::~MainWindow()
{
}



void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    openAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(openAct);
    connect(openAct, SIGNAL(triggered()),
        this, SIGNAL(openFile()));

    saveAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()),
        this, SIGNAL(saveFile()));

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    connect(exitAct, SIGNAL(triggered()),
        this, SLOT(close()));

    /*toolMenu = menuBar()->addMenu(tr("&Tools"));

    //addAct = new QAction(tr("&Add Entry..."), this);
    //toolMenu->addAction(addAct);
    //connect(addAct, SIGNAL(triggered()),
    //    iContractWidget, SLOT(addEntry()));

    //editAct = new QAction(tr("&Edit Entry..."), this);
    //editAct->setEnabled(false);
    //toolMenu->addAction(editAct);
    //connect(editAct, SIGNAL(triggered()),
    //    iContractWidget, SLOT(editEntry()));

    toolMenu->addSeparator();

    removeAct = new QAction(tr("&Remove Entry"), this);
    removeAct->setEnabled(false);
    toolMenu->addAction(removeAct);
    connect(removeAct, SIGNAL(triggered()),
        iContractWidget, SLOT(removeEntry()));*/

    connect(this,SIGNAL(openFile()),iContractWidget,  SLOT(openFile()));
    connect(this,SIGNAL(saveFile()),iContractWidget,  SLOT(saveFile()));

    connect(iContractWidget, SIGNAL(selectionChanged(QItemSelection)),  this, SLOT(updateActions(QItemSelection)));
}




void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();

    /*if (!indexes.isEmpty()) {
        removeAct->setEnabled(true);
        editAct->setEnabled(true);
    } else {
        removeAct->setEnabled(false);
        editAct->setEnabled(false);
    }*/
}
