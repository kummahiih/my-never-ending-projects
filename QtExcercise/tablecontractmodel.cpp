#include <QDebug>
//qDebug() << "foo"; ...

#include "tablecontractmodel.h"


TableContractModel::TableContractModel(QObject *parent)
    : QAbstractTableModel(parent), DummyContractModel()
{

    setTableModelUp();
}

TableContractModel::TableContractModel(const DummyContractModel &other, QObject *parent )
        : QAbstractTableModel(parent), DummyContractModel(other)

{
    //setTableModelUp(); this hack is allready done
}

void TableContractModel::setTableModelUp(){
        Role hackRole = newRole(0, 0, 0, 0, tr("Role description"));
        this->iInsertedRole.setValue(hackRole);

        if(!this->insertRows(0,1, QModelIndex()))
            qDebug() << "hackRole insert problems";

        //setData(this->index(0, QModelIndex(), QVariant(), Qt::EditRole);

        Employee hackEmployee = newEmployee(tr("Employee's Name"), tr("000000-0000"));

        this->iInsertedEmployee.setValue(hackEmployee);
        if(!this->insertRows(1,1, QModelIndex()))
            qDebug() << "hackEmployee insert problems";
        //setData(QModelIndex(), QVariant(), Qt::EditRole);

        Contract hackContract = newContract(hackRole.iRoleId, hackEmployee.iEmployeeId);

        this->iInsertedContract.setValue(hackContract);
        if(!this->insertRows(2,1, QModelIndex()))
            qDebug() << "hackContract insert problems";
        //setData(QModelIndex(), QVariant(), Qt::EditRole);


        Work hackWork = newWork(hackContract.iContractId,0,0,0);

        this->iInsertedWork.setValue(hackWork);
        if(!this->insertRows(3,1, QModelIndex()))
            qDebug() << "hackWork insert problems";
        //setData(QModelIndex(), QVariant(), Qt::EditRole);




}





int TableContractModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1 + 3 + 2 + 5 + 1; // see headerData
}



QVariant TableContractModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Entry Description");
            case 1:
                return tr("Months");
            case 2:
                return tr("Outcome");
            case 3:
                return tr("Hours");

            case 4:
                return tr("Social Security Number");
            case 5:
                return tr("Name");

            case 6:
                return tr("Description");
            case 7:
                return tr("Monthly Compensation");
            case 8:
                return tr("Target Outcome");
            case 9:
                return tr("Bonus \%");
            case 10:
                return tr("Hourly Compensation");

            case 11:
                return tr("Sallary");

            default:
                return QVariant();
        }
    }
    return QVariant();
}





QVariant TableContractModel::data(const QModelIndex &index, int role) const
{


    if (!index.isValid())
        return QVariant();

    if (index.row() >= this->rowCount(QModelIndex() )  || index.row() < 0)
        return QVariant();

    //be careful with the role...
    if (role == Qt::DisplayRole) {
        QVariant qVrowrole = rowType(index.row());
        if(!qVrowrole.isValid())
            return QVariant();
        int rowrole = qVrowrole.value<int>();

        // iRoles.size() + iEmployees.size() +iContracts.size() + iWorks.size();
        //TODO: UGLYYY!!! use lots of time to refactor !!!





        //        return 1 + 3 + 2 + 5 + 1;
        if(index.column() == 0){
            switch(rowrole){
                case 0: return tr("Role");
                case 1: return tr("Employee");
                case 2: return tr("Contract");
                case 3: return tr("Work");
                default: return QVariant();
            }
        }


        //iRoles.size() + iEmployees.size() + iContracts.size() + iWorks.size();
        QVariant qVrowtypeindex = rowTypeIndex(index.row());
        if(!qVrowtypeindex.isValid()){
            qDebug() << "qVrowtypeindex not valid";
            return false;
        }


        int rowtypeindex = qVrowtypeindex.value<int>();

        //if(rowtypeindex == 0)
        //    return QVariant(); //do not show the panichackrows




        if(rowrole == 0){ //role
            switch(index.column()){
                case 6: return iRoles[rowtypeindex].iDescription;
                case 7: return iRoles[rowtypeindex].iMonthlySallary;
                case 8: return iRoles[rowtypeindex].iTargetOutcome;
                case 9: return iRoles[rowtypeindex].iBonusMultiplier;
                case 10: return iRoles[rowtypeindex].iHourlySallary;
                case 11: return calculateSallary(
                            QVariant(),
                            QVariant(),
                            QVariant(this->iRoles[index.row()].iRoleId),
                            QVariant());
                default: return QVariant();
            }

        }

        if(rowrole == 1){ //employer
            switch(index.column()){
                case 4: return iEmployees[rowtypeindex].iSocialSecurityNumber;
                case 5: return iEmployees[rowtypeindex].iName;
                case 11: return calculateSallary(
                        QVariant(),
                        QVariant(),
                        QVariant(),
                        QVariant(iEmployees[rowtypeindex].iEmployeeId)
                        );
                default: return QVariant();
            }


        }

        if(rowrole == 2){ //contract
            Employee contractEmployee = iIdEmployees[iContracts[rowtypeindex].iEmployeeId];
            Role contractRole = iIdRoles[iContracts[rowtypeindex].iRoleId];
            switch(index.column()){
            case 4: return  contractEmployee.iSocialSecurityNumber;
            case 5: return  contractEmployee.iName;
            case 6: return  contractRole.iDescription;
            case 7: return  contractRole.iMonthlySallary;
            case 8: return  contractRole.iTargetOutcome;
            case 9: return  contractRole.iBonusMultiplier;
            case 10: return contractRole.iHourlySallary;
            case 11: return calculateSallary(
                    QVariant(),
                    QVariant(iContracts[rowtypeindex].iContractId),
                    QVariant(contractRole.iRoleId),
                    QVariant(contractEmployee.iEmployeeId)
                    );
            default: return QVariant();

            }

        }

        if(rowrole == 3){ //work
            Contract workContract = iIdContracts[iWorks[rowtypeindex].iContractId];
            Employee contractEmployee = iIdEmployees[workContract.iEmployeeId];
            Role contractRole = iIdRoles[workContract.iRoleId];

            switch(index.column()){
                case 1: return  iWorks[rowtypeindex].iMonths;
                case 2: return  iWorks[rowtypeindex].iRealizedOutcome;
                case 3: return  iWorks[rowtypeindex].iHours;
                case 4: return  contractEmployee.iSocialSecurityNumber;
                case 5: return  contractEmployee.iName;
                case 6: return  contractRole.iDescription;
                case 7: return  contractRole.iMonthlySallary;
                case 8: return  contractRole.iTargetOutcome;
                case 9: return  contractRole.iBonusMultiplier;
                case 10: return contractRole.iHourlySallary;
            case 11: return calculateSallary(
                    QVariant(iWorks[rowtypeindex].iWorkId),
                    QVariant(workContract.iContractId),
                    QVariant(contractRole.iRoleId),
                    QVariant(contractEmployee.iEmployeeId)
                    );
            default: return QVariant();

            }

        }



    }
    return QVariant();
}

//inserts empty things to datastructures, these are then modificated elsewhere
bool TableContractModel::insertRows(int position, int rows, const QModelIndex &index)
{

    Q_UNUSED(index);
    //TODO
    if(rows>1)
        return false;

    QVariant qVrowtype = rowType(position);
    if(!qVrowtype.isValid()){
        qDebug() << "qVrowtype not valid";
        return false;
    }
    int rowtype =  qVrowtype.value<int>();

    QVariant qVrowtypeindex = rowTypeIndex(position);
    if(!qVrowtypeindex.isValid()){
        qDebug() << "qVrowtypeindex not valid";
        return false;
    }

    int rowtypeindex = qVrowtypeindex.value<int>();

    qDebug() << "insertRows, type" << rowtype << "rowtypeindex" << rowtypeindex;

    beginInsertRows(QModelIndex(), position, position+rows-1);
    bool success;

    for (int row=0; row < rows; row++) {
        switch(rowtype){


        case 0: //role
            success = insertRole(this->iInsertedRole, rowtypeindex);
            if(success)
                this->iInsertedRole = QVariant();
            break;

        case 1: //employee
            success = this->insertEmployee(this->iInsertedEmployee, rowtypeindex);
            if(success)
                this->iInsertedEmployee = QVariant();
            break;

        case 2: //contract
            success = this->insertContract(this->iInsertedContract, rowtypeindex);
            if(success)
                this->iInsertedContract = QVariant();
            break;

        case 3: //work
           success = this->insertWork(this->iInsertedWork, rowtypeindex);
           if(success)
               this->iInsertedWork = QVariant();
           break;
        }
    }

    endInsertRows();

    qDebug() << "insertRows, success" << success;
    return success;
}








Qt::ItemFlags TableContractModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool TableContractModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    //TODO
    Q_UNUSED(value);
    //TODO
    Q_UNUSED(role);
    //TODO
    return true;
}


void TableContractModel::addRole(double aTargetOutcome, double iBonusMultiplier, double aMonthlySallary, double aHourlySallary, QString aDescription){
    Role hackRole = newRole(aTargetOutcome,iBonusMultiplier,aMonthlySallary,aHourlySallary,aDescription );
    this->iInsertedRole.setValue(hackRole);

    if(!this->insertRows(typeRow(0),1, QModelIndex()))
        qDebug() << "addRole insert problems";


}

void TableContractModel::addEmployee(QString aName, QString aSocialSecurityNumber){
    Employee hackEmployee = newEmployee(aName,aSocialSecurityNumber);

    this->iInsertedEmployee.setValue(hackEmployee);
    if(!this->insertRows(typeRow(1),1, QModelIndex()))
        qDebug() << "addEmployee insert problems";

}

void TableContractModel::addContract(int aRoleIndex, int aEmployeeIndex){
    QVariant roleindex = rowTypeIndex(aRoleIndex);
    qDebug() << "aRoleIndex" << aRoleIndex << "roleindex" << roleindex;
    if(!roleindex.isValid())
        return;
    qint64 roleId = this->iRoles[roleindex.value<int>()].iRoleId;
    if (roleId == 0)
        return;

    qDebug() << "roleId" << roleId;

    QVariant employeeindex = rowTypeIndex(aEmployeeIndex);

    qDebug() << "aEmployeeIndex" << aEmployeeIndex << "employeeindex" << employeeindex;
    if(!employeeindex.isValid())
        return;

    qint64 employeeId = this->iEmployees[employeeindex.value<int>()].iEmployeeId;
    if (employeeId == 0)
        return;
    qDebug() << "employeeId" << employeeId;


    Contract hackContract = newContract(roleId, employeeId);

    this->iInsertedContract.setValue(hackContract);
    if(!this->insertRows(typeRow(2),1, QModelIndex()))
        qDebug() << "addContract insert problems";
    //setData(QModelIndex(), QVariant(), Qt::EditRole);
}

void TableContractModel::addWork(int aContractIndex, double aHours, double aRealizedOutcome, double aMonths){

    QVariant contractindex = rowTypeIndex(aContractIndex);

    qDebug() << "aContractIndex" << aContractIndex << "contractindex" << contractindex;
    if(!contractindex.isValid())
        return;

    qint64 contractId = this->iContracts[contractindex.value<int>()].iContractId;
    qDebug() << "contractId" << contractId;
    if (contractId == 0)
        return;

    Work hackWork = newWork(contractId, aHours, aRealizedOutcome, aMonths);

    this->iInsertedWork.setValue(hackWork);
    if(!this->insertRows(typeRow(3),1, QModelIndex()))
        qDebug() << "addWork insert problems";
    //setData(QModelIndex(), QVariant(), Qt::EditRole);
}




QVariant TableContractModel::rowType(int row) const
{ // see rowCount and data


    int base = 0;


    //what if the lists are empty?
    //I'll just add some empty objects to the lists.. At least I learned what I should avoid
    //.. but this information is needed because of the way new rows
    //must be created by using insertRows. so I have to add things in a hard coded way.
    //fuck I say, fuck, FUCK!
    //see constructor, and do not touch to this!
    base += iRoles.size();
    //qDebug() << "rowType0" << row<< base;
    if (row < base || iRoles.size() == 0) return 0; //role
    base += iEmployees.size();
    //qDebug() << "rowType1" << row<< base;
    if (row < base || iEmployees.size() == 0) return 1; //employee
    base += iIdContracts.size();
    //qDebug() << "rowType2" << row<< base;
    if (row < base || iIdContracts.size() == 0) return 2; //contract
    base += iIdWorks.size();
    //qDebug() << "rowType3" << row<< base;
    if (row < base || iIdWorks.size() == 0) return 3; //work
    //qDebug() << "rowType invalid" << row<< base;

    return QVariant();
}

int TableContractModel::rowCount(const QModelIndex &parent) const //see rowType
{
    Q_UNUSED(parent);
    //own row for each of these
    return  iRoles.size() + iEmployees.size() + iContracts.size() + iWorks.size();
}

int TableContractModel::typeRow(int atype) const{
    qDebug() << "atype" << atype;
    switch(atype){
    case 0: return iRoles.size()-1;
    case 1: return iRoles.size() + iEmployees.size()-1;
    case 2: return iRoles.size() + iEmployees.size() + iContracts.size()-1;
    case 3: return iRoles.size() + iEmployees.size() + iContracts.size() + iWorks.size()-1;
    }
    return -1;
}


QVariant TableContractModel::rowTypeIndex(int row) const{ // see rowCount and data and DO NOT TOUCH
    int base = 0;

    if (row < base + iRoles.size() || iRoles.size() == 0) return row - base;
    base +=iRoles.size();

    if (row < base + iEmployees.size() || iEmployees.size() == 0) return row - base;
    base += iEmployees.size();

    if (row < base + iIdContracts.size() || iIdContracts.size() == 0) return row - base;
    base += iContracts.size();

    if (row < base + iIdWorks.size() || iIdWorks.size() == 0) return row - base;
    base += iWorks.size();

    return QVariant();
}


