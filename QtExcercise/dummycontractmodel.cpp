#include <QDebug>
#include <QString>


#include "dummycontractmodel.h"

//qDebug() << "foo"; ...

//TODO: refactor some kind of template functions to control copypaste and typo errors


QDataStream &operator<<(QDataStream &out, const Employee &employee)
 {
     out << employee.iEmployeeId << employee.iName << employee.iSocialSecurityNumber;
     return out;
 }

QDataStream &operator>>(QDataStream &in, Employee &employee)
 {
     in >> employee.iEmployeeId  >> employee.iName >> employee.iSocialSecurityNumber;
     return in;
 }


QDataStream &operator<<(QDataStream &out, const Contract &contract)
{
    out << contract.iContractId << contract.iRoleId << contract.iEmployeeId;
    return out;
}

QDataStream &operator>>(QDataStream &in, Contract &contract)
{
    in >>  contract.iContractId >> contract.iRoleId >> contract.iEmployeeId;
    return in;
}

QDataStream &operator<<(QDataStream &out, const Role &role)
{
    out << role.iRoleId << role.iDescription << role.iMonthlySallary << role.iHourlySallary << role.iTargetOutcome << role.iBonusMultiplier;
    return out;
}

QDataStream &operator>>(QDataStream &in, Role &role)
{
    in >> role.iRoleId >> role.iDescription >> role.iMonthlySallary >> role.iHourlySallary >> role.iTargetOutcome >> role.iBonusMultiplier;
    return in;
}

QDataStream &operator<<(QDataStream &out, const Work &work)
{
    out << work.iWorkId << work.iContractId << work.iHours << work.iMonths << work.iRealizedOutcome;
    return out;
}
QDataStream &operator>>(QDataStream &in, Work &work)
{
    in >> work.iWorkId >> work.iContractId >> work.iHours >> work.iMonths >> work.iRealizedOutcome;
    return in;
}

Role DummyContractModel::newRole(double aTargetOutcome, double aBonusMultiplier, double aMonthlySallary, double aHourlySallary, QString aDescription){
    Role hackRole;
    hackRole.iTargetOutcome = aTargetOutcome;
    hackRole.iRoleId = iNextRoleId;
    hackRole.iMonthlySallary = aMonthlySallary;
    hackRole.iHourlySallary = aHourlySallary;
    hackRole.iBonusMultiplier = aBonusMultiplier;
    hackRole.iDescription = aDescription;// tr works only for QObjects // tr("Employee's Name");
    return hackRole;
}

Employee DummyContractModel::newEmployee(QString aName, QString aSocialSecurityNumber){
    Employee hackEmployee;
    hackEmployee.iName = aName ;
    hackEmployee.iSocialSecurityNumber = aSocialSecurityNumber;
    hackEmployee.iEmployeeId = iNextEmployeeId;
    return hackEmployee;
}

Contract DummyContractModel::newContract(qint64 aRoleId, quint64 aEmployeeId){
    Contract hackContract;
    hackContract.iContractId = iNextContractId;
    hackContract.iRoleId = aRoleId;
    hackContract.iEmployeeId = aEmployeeId;
    return hackContract;
}

Work DummyContractModel::newWork(quint64 aContractId, double aHours, double aRealizedOutcome, double aMonths){
    Work hackWork;
    hackWork.iContractId = aContractId;
    hackWork.iHours = aHours;
    hackWork.iRealizedOutcome =aRealizedOutcome;
    hackWork.iMonths = aMonths;
    hackWork.iWorkId = iNextWorkId;
    return hackWork;
}


bool DummyContractModel::insertRole(QVariant aInsertedRole, int rowtypeindex){
    if (!aInsertedRole.isValid() || !aInsertedRole.canConvert<Role>())
        return false;
    Role insertedrole = aInsertedRole.value<Role>();

    if(iIdRoles.contains(insertedrole.iRoleId))
        return false;

    this->iRoles.insert(rowtypeindex, insertedrole);
    this->iIdRoles[insertedrole.iRoleId] = insertedrole;
    iNextRoleId ++;

    qDebug() << "DummyContractModel::insertRole success";
    return true;
}

bool DummyContractModel::insertEmployee(QVariant aInsertedEmployee, int rowtypeindex){
    if(!aInsertedEmployee.isValid() || !aInsertedEmployee.canConvert<Employee>())
        return false;
    Employee insertedemployee = aInsertedEmployee.value<Employee>();

    if(this->iIdEmployees.contains(insertedemployee.iEmployeeId))
        return false;

    this->iEmployees.insert(rowtypeindex, insertedemployee);
    this->iIdEmployees[insertedemployee.iEmployeeId] = insertedemployee;
    iNextEmployeeId++;

    qDebug() << "DummyContractModel::insertEmployee success";
    return true;
}

bool DummyContractModel::insertContract(QVariant aInsertedContract, int rowtypeindex){
    if(!aInsertedContract.isValid() || !aInsertedContract.canConvert<Contract>())
        return false;
    Contract insertedcontract = aInsertedContract.value<Contract>();

    if(this->iIdContracts.contains(insertedcontract.iContractId))
        return false;

    if(!this->iIdEmployees.contains(insertedcontract.iEmployeeId)
        || !this->iIdRoles.contains(insertedcontract.iRoleId))
        return false;

    this->iContracts.insert(rowtypeindex, insertedcontract);
    this->iIdContracts[insertedcontract.iContractId] = insertedcontract;

    this->iRolesContracts.insertMulti(insertedcontract.iRoleId, insertedcontract);
    this->iEmployeesContracts.insertMulti(insertedcontract.iEmployeeId, insertedcontract);


    iNextContractId ++;

    qDebug() << "DummyContractModel::insertContract success";


    return true;

}
bool DummyContractModel::insertWork(QVariant aInsertedWork, int rowtypeindex){
    if(!aInsertedWork.isValid() || !aInsertedWork.canConvert<Work>())
        return false;
    Work insertedwork = aInsertedWork.value<Work>();

    if(this->iIdWorks.contains(insertedwork.iWorkId))
        return false;
    if(!this->iIdContracts.contains(insertedwork.iContractId))
        return false;

    this->iWorks.insert(rowtypeindex, insertedwork);
    this->iIdWorks[insertedwork.iWorkId] = insertedwork;

    this->iContractsWork.insertMulti(insertedwork.iContractId, insertedwork);
    iNextWorkId++;
    qDebug() << "DummyContractModel::insertWork success";
    return true;
}


DummyContractModel::DummyContractModel()
{
    iNextRoleId = 0;
    iNextEmployeeId = 0;
    iNextContractId = 0;
    iNextWorkId = 0;
    //see TableContractModel::rowType

}

DummyContractModel::DummyContractModel(const DummyContractModel &other)
{
    (*this) =  other;
}

DummyContractModel & DummyContractModel::operator = (const DummyContractModel &other){
    iEmployees = other.iEmployees;
    iRoles = other.iRoles;
    iContracts = other.iContracts;
    iWorks = other.iWorks;

    iNextEmployeeId = other.iNextEmployeeId;
    iIdEmployees = other.iIdEmployees;

    iNextRoleId = other.iNextRoleId;
    iIdRoles = other.iIdRoles;

    iNextContractId = other.iNextContractId;
    iIdContracts = other.iIdContracts;

    iNextWorkId = other.iNextWorkId;
    iIdWorks = other.iIdWorks;

    iEmployeesContracts = other.iEmployeesContracts;

    iRolesContracts = other.iRolesContracts;

    iContractsWork = other.iContractsWork;

    return *this;
}

QDataStream &operator<<(QDataStream &out, const DummyContractModel &dummyContractModel){
    out << dummyContractModel.iEmployees
            << dummyContractModel.iRoles
            << dummyContractModel.iContracts
            << dummyContractModel.iWorks

            << dummyContractModel.iIdEmployees
            << dummyContractModel.iNextRoleId
            << dummyContractModel.iIdRoles

            << dummyContractModel.iNextContractId
            << dummyContractModel.iIdContracts

            << dummyContractModel.iNextWorkId
            << dummyContractModel.iIdWorks

            << dummyContractModel.iEmployeesContracts
            << dummyContractModel.iRolesContracts
            << dummyContractModel.iContractsWork;
return out;
}

QDataStream &operator>>(QDataStream &in, DummyContractModel &dummyContractModel){
    in >> dummyContractModel.iEmployees
            >> dummyContractModel.iRoles
            >> dummyContractModel.iContracts
            >> dummyContractModel.iWorks

            >> dummyContractModel.iIdEmployees
            >> dummyContractModel.iNextRoleId
            >> dummyContractModel.iIdRoles

            >> dummyContractModel.iNextContractId
            >> dummyContractModel.iIdContracts

            >> dummyContractModel.iNextWorkId
            >> dummyContractModel.iIdWorks

            >> dummyContractModel.iEmployeesContracts
            >> dummyContractModel.iRolesContracts
            >> dummyContractModel.iContractsWork;
return in;
}




 double DummyContractModel::calculateSallary(QVariant aWorkId,
                        QVariant aContractId,
                        QVariant aRoleId,
                        QVariant aEmployeeId)const {
     double compensation = 0;

     if(aWorkId.isValid() && aWorkId.canConvert<qint64>()){
         qint64 workId = aWorkId.value<qint64>();
         if(!this->iIdWorks.contains(workId)){
            qDebug() << "workId invalid:"<< workId;
            return 0;
     }
     else{

         qint64 contractId = this->iIdWorks[workId].iContractId;
         if(!this->iIdContracts.contains(contractId)){
            qDebug() << "contractId invalid:"<< contractId;
            return 0;
         }
         qint64 roleId = this->iIdContracts[contractId].iRoleId;
         if(!this->iIdRoles.contains(roleId)){
            qDebug() << "roleId invalid:"<< roleId;
            return 0;
         }
         compensation += this->iIdWorks[workId].iMonths * this->iIdRoles[roleId].iMonthlySallary;
         if (this->iIdWorks[workId].iRealizedOutcome >= this->iIdRoles[roleId].iTargetOutcome / ( this->iIdWorks[workId].iMonths ) ){
             compensation *= (1+this->iIdRoles[roleId].iBonusMultiplier/100.0);
             //qDebug() <<"bonus";
         }
         compensation += this->iIdWorks[workId].iHours * this->iIdRoles[roleId].iHourlySallary;
         return compensation;
        }
     }

     if(aContractId.isValid() && aContractId.canConvert<qint64>()){
         qint64 contractId = aContractId.value<qint64>();
         if(!this->iIdContracts.contains(contractId)){
            qDebug() << "contractId invalid:"<< contractId;
            return 0;
         }

         QList<Work> values = iContractsWork.values(contractId);
         for (int i = 0; i < values.size(); ++i)
              compensation += calculateSallary(QVariant(values.at(i).iWorkId));
         return compensation;
     }

     if(aRoleId.isValid() && aRoleId.canConvert<qint64>()){
         qint64 roleId = aRoleId.value<qint64>();
         if(!this->iIdRoles.contains(roleId)){
             qDebug() << "roleId invalid:"<< roleId;
             return 0;
         }

         QList<Contract> values = this->iRolesContracts.values(roleId);
         for (int i = 0; i < values.size(); ++i)
              compensation += calculateSallary(QVariant(), QVariant(values.at(i).iContractId));
         return compensation;
     }

     if(aEmployeeId.isValid() && aEmployeeId.canConvert<qint64>()){
         qint64 employerId = aEmployeeId.value<quint64>();
         if(!this->iIdEmployees.contains(employerId)){
             qDebug() << "employerId invalid:"<< employerId;
             return 0;
         }

         QList<Contract> values = this->iEmployeesContracts.values(employerId);
         for (int i = 0; i < values.size(); ++i)
              compensation += calculateSallary(QVariant(), QVariant(values.at(i).iContractId));
         return compensation;
     }

    qDebug() << "all parameters invalid:";
    return 0;
}

