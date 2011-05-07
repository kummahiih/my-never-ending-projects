#ifndef TABLECONTRACTMODEL_H
#define TABLECONTRACTMODEL_H

#include <QAbstractTableModel>
#include "dummycontractmodel.h"



class TableContractModel : public QAbstractTableModel, private DummyContractModel
{
Q_OBJECT
public:
    TableContractModel(QObject *parent = 0);
    TableContractModel(const DummyContractModel &other, QObject *parent = 0);


    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    //bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:
    void addRole(double aTargetOutcome, double iBonusMultiplier, double aMonthlySallary, double aHourlySallary, QString aDescription);
    void addEmployee(QString aName, QString aSocialSecurityNumber);
    void addContract(int aRoleIndex, int aEmployeeIndex);
    void addWork(int aContractIndex, double aHours, double aRealizedOutcome, double aMonths);




private:

    void setTableModelUp();

    QVariant rowType(int row) const;
    QVariant rowTypeIndex(int row) const;
    int typeRow(int atype) const;


    //set these in a right way before calling insertRow and set to invalid afterwards.
    QVariant iInsertedEmployee;
    QVariant iInsertedRole;
    QVariant iInsertedContract;
    QVariant iInsertedWork;


signals:

public slots:

};

#endif // TABLECONTRACTMODEL_H
