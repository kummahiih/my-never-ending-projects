#include <QtGui>

#include <QList>
#include <QTableView>
//#include <QStackedLayout>

#include <QVBoxLayout>


#include <QGridLayout>
#include <QLabel>

#include <QPushButton>


#include <QLineEdit>

#include <QDoubleValidator>
#include <QIntValidator>

//this is uglyyyy
#include <QAbstractSlider>

#include <QProxyModel>
#include <QRegExp>


#include <QDebug>


#include "contractwidget.h"

#include "dummycontractmodel.h" //for file operations

ContractWidget::ContractWidget(QWidget *parent) :
    QWidget(parent)
{
    this->resize(800, 200);
    iContractModel = new TableContractModel();


    tableView = new QTableView;
    tableView->setSortingEnabled(true);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->verticalHeader()->hide();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    QVBoxLayout * mainlayout = new QVBoxLayout();
    setLayout(mainlayout);

    QGridLayout *  deletelayout = new QGridLayout();




    deletelayout->addWidget(tableView,0,0,15,5);

    //this is just an exercise, and I guess I'm not going to learn a thing from this one..
    //iDeleteButton = new QPushButton(tr("Delete entry"));
    //deletelayout->addWidget(iDeleteButton,0,5);

    mainlayout->addLayout(deletelayout);
    //tableView->resizeColumnsToContents(); ?? where to call?

    QVBoxLayout * layout = new QVBoxLayout();// new window?
    mainlayout->addLayout(layout, 5); // new window?



    QGridLayout *  roleLayout = new QGridLayout();


    iDescriptionEdit = new QLineEdit();

    roleLayout->addWidget(new QLabel(tr("Role Description")), 0,0);
    roleLayout->addWidget(iDescriptionEdit, 1,0);

    iMonthlyEdit = new QLineEdit();
    iMonthlyEdit->setValidator(new QDoubleValidator(iMonthlyEdit));
    roleLayout->addWidget(new QLabel(tr("Monthly compensation")), 0,1);
    roleLayout->addWidget(iMonthlyEdit, 1,1);

    iTargetEdit = new QLineEdit();
    iTargetEdit->setValidator(new QDoubleValidator(iTargetEdit));
    roleLayout->addWidget(new QLabel(tr("Target outcome")), 0,2);
    roleLayout->addWidget(iTargetEdit, 1,2);

    iBonusEdit = new QLineEdit();
    iBonusEdit->setValidator(new QDoubleValidator(iBonusEdit));
    roleLayout->addWidget(new QLabel(tr("Bonus \%")), 0,3);
    roleLayout->addWidget(iBonusEdit, 1,3);

    iHourlyEdit = new QLineEdit();
    iHourlyEdit->setValidator(new QDoubleValidator(iHourlyEdit));
    roleLayout->addWidget(new QLabel(tr("Hourly compensation")), 0,4);
    roleLayout->addWidget(iHourlyEdit, 1,4);

    iRoleButton = new QPushButton(tr("Add role"));
    roleLayout->addWidget(iRoleButton,1,5);

    layout->addLayout(roleLayout);


    QGridLayout *  employeeLayout = new QGridLayout();

    iNameEdit = new QLineEdit();
    employeeLayout->addWidget(new QLabel(tr("Name")), 0,0);
    employeeLayout->addWidget(iNameEdit,1,0);

    iSecNumEdit = new QLineEdit();
    employeeLayout->addWidget(new QLabel(tr("Social security number")), 0,1);
    employeeLayout->addWidget(iSecNumEdit,1,1);

    iEmployeeButton = new QPushButton(tr("Add employee"));
    employeeLayout->addWidget(iEmployeeButton,1,2);
    layout->addLayout(employeeLayout);

    QGridLayout *  contractLayout = new QGridLayout();



    iEmployeeProxyModel = new QSortFilterProxyModel(this);

    //connection happens in connecModelToView cause of
    //the model might be loaded from a file
    iEmployeeProxyModel->setDynamicSortFilter(true);

    iEmployeeTableView = new QTableView;
    iEmployeeTableView->setModel(iEmployeeProxyModel);
    iEmployeeTableView->setSortingEnabled(true);
    iEmployeeTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //employeeTableView->horizontalHeader()->setStretchLastSection(true);
    iEmployeeTableView->verticalHeader()->hide();
    //employeeTableView->horizontalHeader()->hide();
    iEmployeeTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    iEmployeeTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    for(int h= 0;h<4;h++){
        iEmployeeTableView->setColumnHidden(h,true);// BUUUG!!!!!! I FOUND a BUG!!!
        iEmployeeTableView->setColumnWidth(h,0);
        iEmployeeTableView->hideColumn(h);
        qDebug() <<"hidden?"<< iEmployeeTableView->isColumnHidden(h);
    }


    iEmployeeProxyModel->setFilterRegExp(QRegExp(tr("^Employee$"), Qt::CaseInsensitive));
    iEmployeeProxyModel->setFilterKeyColumn(0);
    iEmployeeProxyModel->sort(5, Qt::AscendingOrder);

    contractLayout->addWidget(new QLabel(tr("Employee")), 0,0);
    contractLayout->addWidget(iEmployeeTableView,1,0);

    iRoleProxyModel = new QSortFilterProxyModel(this);

    iRoleTableView = new QTableView;
    iRoleTableView->setModel(iRoleProxyModel);
    iRoleTableView->setSortingEnabled(true);
    iRoleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //roleTableView->horizontalHeader()->setStretchLastSection(true);
    iRoleTableView->verticalHeader()->hide();
    //roleTableView->horizontalHeader()->hide();
    iRoleTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    iRoleTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    for(int h= 0;h<6;++h){
        iRoleTableView->setColumnHidden(h,true);// BUUUG!!!!!! I FOUND a BUG!!!
        iRoleTableView->setColumnWidth(h,0);
        iRoleTableView->hideColumn(h);

        qDebug() <<"hidden?"<< iRoleTableView->isColumnHidden(h);
    }

    iRoleProxyModel->setFilterRegExp(QRegExp(tr("^Role$"), Qt::CaseInsensitive));
    iRoleProxyModel->setFilterKeyColumn(0);
    iRoleProxyModel->sort(6, Qt::AscendingOrder);


    contractLayout->addWidget(new QLabel(tr("Role")), 0,1);
    contractLayout->addWidget(iRoleTableView, 1,1);
    iContractButton = new QPushButton(tr("Add contract"));
    contractLayout->addWidget(iContractButton,1,2);
    layout->addLayout(contractLayout);




    QGridLayout *  workLayout = new QGridLayout();
    iContractProxyModel = new QSortFilterProxyModel(this);

    iContractTableView = new QTableView;
    iContractTableView->setModel(iContractProxyModel);
    iContractTableView->setSortingEnabled(true);
    iContractTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //roleTableView->horizontalHeader()->setStretchLastSection(true);
    iContractTableView->verticalHeader()->hide();
    //roleTableView->horizontalHeader()->hide();
    iContractTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    iContractTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    for(int h= 0;h<4;++h){
        iContractTableView->setColumnHidden(h,true);// BUUUG!!!!!! I FOUND a BUG!!!
        iContractTableView->setColumnWidth(h,0);
        iContractTableView->hideColumn(h);
        qDebug() <<"hidden?"<< iContractTableView->isColumnHidden(h);
    }

    iContractProxyModel->setFilterRegExp(QRegExp(tr("^Contract$"), Qt::CaseInsensitive));
    iContractProxyModel->setFilterKeyColumn(0);
    iContractProxyModel->sort(6, Qt::AscendingOrder);



    iMonthEdit = new QLineEdit();
    iMonthEdit->setValidator(new QDoubleValidator(iMonthEdit));
    workLayout->addWidget(new QLabel(tr("Done months")), 0,1);
    workLayout->addWidget(iMonthEdit,1,1);

    iOutcomeEdit = new QLineEdit();
    iOutcomeEdit->setValidator(new QDoubleValidator(iOutcomeEdit));
    workLayout->addWidget(new QLabel(tr("Achieved outcome")), 0,2);
    workLayout->addWidget(iOutcomeEdit,1,2);

    iHourEdit = new QLineEdit();
    iHourEdit->setValidator(new QDoubleValidator(iHourEdit));
    workLayout->addWidget(new QLabel(tr("Done hours")), 0,3);
    workLayout->addWidget(iHourEdit,1,3);

    workLayout->addWidget(new QLabel(tr("Contract")), 1,0);
    workLayout->addWidget(iContractTableView,2,0,3,3);

    iWorkButton = new QPushButton(tr("Add work"));
    workLayout->addWidget(iWorkButton,2,3);

    layout->addLayout(workLayout);


    connect(iRoleButton, SIGNAL(clicked()), this, SLOT(addRole()));
    connect(iEmployeeButton, SIGNAL(clicked()), this, SLOT(addEmployee()));
    connect(iContractButton, SIGNAL(clicked()), this, SLOT(addContract()));
    connect(iWorkButton, SIGNAL(clicked()), this, SLOT(addWork()));

    connecModelToView();

}

void ContractWidget::connecModelToView(){
    tableView->setModel(iContractModel);
    //iContractModel->setTableModelUp();
    //tableView->update(0, 0, 3, 11);

    connect(tableView->selectionModel(),
        SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
        this, SIGNAL(selectionChanged(QItemSelection)));


    iEmployeeProxyModel->setSourceModel(iContractModel);
    iRoleProxyModel->setSourceModel(iContractModel);
    iContractProxyModel->setSourceModel(iContractModel);

    connect(this,SIGNAL(addRole(double,double,double,double,QString)),iContractModel,SLOT(addRole(double,double,double,double,QString)));
    connect(this,SIGNAL(addEmployee(QString,QString)),iContractModel,SLOT(addEmployee(QString,QString)));
    connect(this,SIGNAL(addContract(int,int)),iContractModel,SLOT(addContract(int,int)));
    connect(this,SIGNAL(addWork(int,double,double,double)),iContractModel,SLOT(addWork(int,double,double,double)));


}



void ContractWidget::addRole(){
    emit this->addRole(
            this->iTargetEdit->text().toDouble(),
            this->iBonusEdit->text().toDouble(),
            this->iMonthlyEdit->text().toDouble(),
            this->iHourlyEdit->text().toDouble(),
            this->iDescriptionEdit->text()
            );
}

void ContractWidget::addEmployee(){
    emit this->addEmployee(
            this->iNameEdit->text(),
            this->iSecNumEdit->text()
            );
}

void ContractWidget::addContract(){
    if(!this->iRoleTableView->selectionModel()->hasSelection()
        ||!this->iEmployeeTableView->selectionModel()->hasSelection())
        return;

    emit this->addContract(
            this->iRoleProxyModel->mapToSource(
                    this->iRoleTableView->selectionModel()->currentIndex()
                    ).row(),
            this->iEmployeeProxyModel->mapToSource(
                this->iEmployeeTableView->selectionModel()->currentIndex()
                ).row()
            );
}

void ContractWidget::addWork(){
    if(!this->iContractTableView->selectionModel()->hasSelection())
        return;
    emit this->addWork(
            this->iContractProxyModel->mapToSource(
                    this->iContractTableView->selectionModel()->currentIndex()
                    ).row(),
            this->iHourEdit->text().toDouble(),
            this->iOutcomeEdit->text().toDouble(),
            this->iMonthEdit->text().toDouble()
            );
}






void ContractWidget::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        //versioning. see QDataStream documentation
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_2);

        quint32 magic;
        quint32 version;
        in >> magic;
        in >> version;


        QList<DummyContractModel> testList;
        in >> testList;

        if(testList.size() != 1 || magic != (quint32)0xE0001337 || version != (quint32)1){
            //qDebug() <<testList.size() << magic<< "vs" << (quint32)0xE0001337 << version << "vs" << (quint32)1;
            qDebug() <<(testList.size() != 1) << (magic != (quint32)0xE0001337) << (version != (qint32)1);

            QMessageBox::information(this, tr("Syntax error"), tr("Unable to understand the file!") );
            return;
        }

        //in >> (*iContractModel);
        delete iContractModel;
        iContractModel = new TableContractModel(testList.at(0));

        connecModelToView();

    }
}

void ContractWidget::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty()) {

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QDataStream out(&file);
        //versioning. see QDataStream documentation
        out.setVersion(QDataStream::Qt_4_2);
        out << (quint32)0xE0001337; //magic
        out << (quint32)1;//version

        QList<DummyContractModel> testList;
        testList.append(*((DummyContractModel*)iContractModel));

        out << testList;
        //out <<(*iContractModel);

        //needed?
        //file.flush();
        //file.close();

        //TODO: save first to temporary file, move, delete temporary
        //if(????){
        //    QMessageBox::information(this, tr("Invalid data"), tr("Something went wrong while writing the file"));
        //    return;
        //}

    }
}
