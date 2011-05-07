#ifndef CONTRACTWIDGET_H
#define CONTRACTWIDGET_H

#include <QWidget>
#include <QItemSelection>
#include <QTableView>

#include <QLineEdit>

#include <QSortFilterProxyModel>

#include "tablecontractmodel.h"
#include "tablecontractmodel.h"


QT_BEGIN_NAMESPACE
//class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE

class ContractWidget : public QWidget
{
Q_OBJECT
public:
    ContractWidget(QWidget *parent = 0);


public slots:
    void openFile();
    void saveFile();



    //void addEntry();
    //void addEntry(QString name, QString securitynumber);
    //void editEntry();
    //void removeEntry();

private slots:
    void addRole();
    void addEmployee();
    void addContract();
    void addWork();

signals:
    void selectionChanged(const QItemSelection &selected);

    void addRole(double aTargetOutcome, double iBonusMultiplier, double aMonthlySallary, double aHourlySallary, QString aDescription);
    void addEmployee(QString aName, QString aSocialSecurityNumber);
    void addContract(int aRoleIndex, int aEmployeeIndex);
    void addWork(int aContractIndex, double aHours, double aRealizedOutcome, double aMonths);

private:
    void connecModelToView();
    QTableView *tableView;
    TableContractModel * iContractModel;

    //QPushButton * iDeleteButton;
    //this is just an exercise, and I guess I'm not going to learn a thing from this one..

    QLineEdit * iDescriptionEdit;
    QLineEdit * iMonthlyEdit;
    QLineEdit * iTargetEdit;
    QLineEdit * iBonusEdit;
    QLineEdit * iHourlyEdit;
    QPushButton * iRoleButton;
    //QPushButton * iDeleteRoleButton;


    QLineEdit * iNameEdit;
    QLineEdit * iSecNumEdit;
    QPushButton * iEmployeeButton;
    //QPushButton * iDeleteEmployeeButton;

    QSortFilterProxyModel * iRoleProxyModel;
    QTableView *iRoleTableView;

    QSortFilterProxyModel * iEmployeeProxyModel;
    QTableView *iEmployeeTableView;

    QSortFilterProxyModel * iContractProxyModel;
    QTableView *iContractTableView;

    QPushButton * iContractButton;
    //QPushButton * iDeleteContractButton;

    QLineEdit * iMonthEdit;
    QLineEdit * iOutcomeEdit;
    QLineEdit * iHourEdit;
    QPushButton * iWorkButton;

    //QPushButton * iDeleteWorkButton;

};

#endif // CONTRACTWIDGET_H
