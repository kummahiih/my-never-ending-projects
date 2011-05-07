#ifndef DUMMYCONTRACTMODEL_H
#define DUMMYCONTRACTMODEL_H

#include <QObject>
#include <QtGui>

#include <QMultiMap>
#include <QMap>
#include <QString>
#include <QVariant>

//Keep It Simple Stupid.. well. after some point 'the simple' is not that simple anymore
struct Role
{
    qint64 iRoleId;
    QString iDescription;
    double iMonthlySallary;
    double iHourlySallary;
    double iTargetOutcome;
    double iBonusMultiplier;
};
Q_DECLARE_METATYPE(Role)
QDataStream &operator<<(QDataStream &out, const Role &role);
QDataStream &operator>>(QDataStream &in, Role &role);

struct Employee
{
    qint64 iEmployeeId; //if you use user input as key, yo'll have problems with user interaction
    QString iSocialSecurityNumber;
    QString iName;

};
Q_DECLARE_METATYPE(Employee)
QDataStream &operator<<(QDataStream &out, const Employee &employee);
QDataStream &operator>>(QDataStream &in, Employee &employee);


struct Contract
{
    qint64 iContractId;
    qint64 iEmployeeId;
    qint64 iRoleId;
};
Q_DECLARE_METATYPE(Contract)
QDataStream &operator<<(QDataStream &out, const Contract &contract);
QDataStream &operator>>(QDataStream &in, Contract &contract);



struct Work
{
    qint64 iWorkId;
    qint64 iContractId;
    double iHours;
    double iMonths;
    double iRealizedOutcome;
};
Q_DECLARE_METATYPE(Work)
QDataStream &operator<<(QDataStream &out, const Work &work);
QDataStream &operator>>(QDataStream &in, Work &work);


class DummyContractModel //: public QObject
{
//Q_OBJECT
public:
    DummyContractModel();

    //for serialization
    DummyContractModel(const DummyContractModel &other);
    DummyContractModel &operator=(const DummyContractModel &other);


    friend QDataStream &operator<<(QDataStream &out, const DummyContractModel &dummyContractModel);
    friend QDataStream &operator>>(QDataStream &in, DummyContractModel &dummyContractModel);

    Role newRole(double aTargetOutcome, double iBonusMultiplier, double aMonthlySallary, double aHourlySallary, QString aDescription);
    Employee newEmployee(QString aName, QString aSocialSecurityNumber);
    Contract newContract(qint64 aRoleId, quint64 aiEmployeeId);
    Work newWork(quint64 aContractId, double aHours, double aRealizedOutcome, double aMonths);

    bool insertRole(QVariant aInsertedRole, int rowtypeindex = 0);
    bool insertEmployee(QVariant aInsertedEmployee, int rowtypeindex = 0);
    bool insertContract(QVariant aInsertedContract, int rowtypeindex = 0);
    bool insertWork(QVariant aInsertedWork, int rowtypeindex = 0);

    /*void updateEmployee(QString name, QVariant secNmbr, QVariant oldSecNmbr = QVariant());
    void deleteEmployee(QString secNmbr);

    void updateRole(QVariant description,
                    QVariant monthlySallary,
                    QVariant hourlySallary,
                    QVariant targetOutcome,
                    QVariant bonusMultiplier,
                    QVariant roleId = QVariant());
    void deleteRole(qint64 roleId);

    void updateContract(QString socialSecurityNumber,  qint64 roleId, QVariant contractId =  QVariant() );
    void deleteContract(qint64 contractId);

    void updateWork(qint64 iContractId,
                    QVariant hours,
                    QVariant months,
                    QVariant realizedOutcome,
                    QVariant workId =  QVariant());

    void deleteWork(qint64 workId);*/

    double calculateSallary(QVariant workId = QVariant(),
                            QVariant contractId = QVariant(),
                            QVariant roleId = QVariant(),
                            QVariant iEmployeeId = QVariant()) const;



protected:

    QList <Role> iRoles;
    QList <Employee> iEmployees;    
    QList <Contract> iContracts;
    QList <Work> iWorks;


    //these maps are here because I did know (or have enough time to test) how to serialize pointers to files
    //that's why i'll have to suffer all this.
    qint64 iNextRoleId; //this should be enough for any practical purpose. is used to avoid pointer serialization.
    //TODO: use some better way. cast and map old pointers to new ones or smth
    //roleid role
    QMap <qint64, Role> iIdRoles;

    qint64 iNextEmployeeId; //if you use user input as key, you'll have problems with user interaction
    //iEmployeeId, Employee
    QMap <qint64, Employee> iIdEmployees;


    qint64 iNextContractId;    
    QMap <qint64, Contract> iIdContracts;

    qint64 iNextWorkId;
    QMap <qint64, Work> iIdWorks;


    //iEmployeeId, Contract
    QMultiMap <qint64, Contract> iEmployeesContracts;
    //roleid, Contract
    QMultiMap <qint64, Contract> iRolesContracts;
    //contract id, work
    QMultiMap <qint64, Work> iContractsWork;

};

#endif // DUMMYCONTRACTMODEL_H
