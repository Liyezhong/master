/****************************************************************************/
/*! \file ServiceDataManager/Include/ModuleDataList.h
 *
 *  \brief Definition file for class CModuleDataList.
 *  Reads all Module information from XML, stores in a container (QHash) and also writes
 *  data from container to XML.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-12-20
 *  $Author:    $ Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERVICE_DATAMANAGER_MODULEDATALIST_H
#define SERVICE_DATAMANAGER_MODULEDATALIST_H

#include <QString>
#include <QHash>
#include <QReadWriteLock>
#include <QDateTime>
#include <QIODevice>
#include <QList>

#include "ServiceDataManager/Include/Module.h"
#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "DataManager/Helper/Include/Types.h"
#include "Global/Include/Translator.h"

//lint -e429

namespace ServiceDataManager
{

typedef QHash<QString, CModule*> ListofModules_t; //!< Hash table for list of modules

typedef QList<QString> ListofOrderedModules_t; //!< List for module names

/****************************************************************************/
/*!
 *  \brief  This class implements Module List
 *  Reading all Module information from XML and FunctionStoring in a Container
 */
/****************************************************************************/
class CModuleDataList : public DataManager::CDataContainerBase
{
    friend class CInstrumentHistory;

    QString m_InstrumentName;   //!< name of the Instrument
    QString m_ModuleTimeStamp;  //!< Time Stamp of the Module
    bool m_DataVerificationMode; //!< Verification mode flag , verify the Container
    ListofModules_t m_ModuleList;   //!< Module List
    ListofOrderedModules_t m_ListofModules; //!< List of Module Names
    QString m_FileName; //!< XML file name

    QReadWriteLock *mp_ReadWriteLock; //!< Synchronization for data access
    ErrorMap_t m_ErrorMap;      //!< Event List for GUI and for logging purpose. This member is not copied when using copy constructor/Assignment operator

    bool SerializeContent(QIODevice& IODevice, bool CompleteData);
    bool DeserializeContent(QIODevice& IODevice, bool CompleteData);

    bool SerializeContentXML(QXmlStreamWriter& XmlStreamWriter, bool CompleteData);
    bool DeserializeContentXML(QXmlStreamReader& XmlStreamReader, bool CompleteData);

    /****************************************************************************/
    /*!
     *  \brief  To read module info from xml
     *  \iparam XmlStreamReader = Xml reader to read the XML contents
     *  \iparam CompleteData = bool type if true writes Complete data of object
     *  \return true on success, false on failure
     */
    /****************************************************************************/
    bool ReadModules(QXmlStreamReader& XmlStreamReader, bool CompleteData);

    friend QDataStream& operator <<(QDataStream& OutDataStream, const CModuleDataList&  ModuleList);
    friend QDataStream& operator >>(QDataStream& InDataStream, CModuleDataList& ModuleList);

    QStringList GetModuleIdList() const;
    void AddModuleWithoutVerification(CModule const* p_Module);

public:
    CModuleDataList();
    CModuleDataList(QString TimeStamp);
    CModuleDataList(const CModuleDataList&);    //!< Copy Constructor
    ~CModuleDataList();
    void CopyFromOther(const CModuleDataList &ListofModules);
    CModuleDataList& operator=(const CModuleDataList&); //!< Assignment Operator Overloading

    /********************************************* QReadWriteLock* mp_ReadWriteLock;   ///< Synchronisation for data access
*******************************/
    /*!
     *  \brief returns the  Data container type
     *  \return Data container type
     */
    /****************************************************************************/
    DataContainerType_t GetDataContainerType() {return INSTRUMENTHISTORY;}

    /****************************************************************************/
    /*!
     *  \brief  To set Instrument Name
     *  \iparam value = Name to set
     */
    /****************************************************************************/
    void SetInstrumentName(const QString value) { m_InstrumentName = value; }

    /****************************************************************************/
    /*!
     *  \brief Returns the name of the Instrument
     *  \return Instrument name
     */
    /****************************************************************************/
    QString GetInstrumentName() { return m_InstrumentName; }

    /****************************************************************************/
    /*!
     *  \brief  To set module timestamp
     *  \iparam value = Timestamp to set
     */
    /****************************************************************************/
    void SetModuleTimeStamp(const QString value) { m_ModuleTimeStamp = value; }

    /****************************************************************************/
    /*!
     *  \brief Returns the module time stamp
     *  \return Time stamp
     */
    /****************************************************************************/
    QString GetModuleTimeStamp() { return m_ModuleTimeStamp; }

    /****************************************************************************/
    /*!
     *  \brief  To get number of modules in modulelist
     *  \return Number of modules
     */
    /****************************************************************************/
    int GetNumberofModules() { return m_ModuleList.count(); }

    /****************************************************************************/
    /*!
     *  \brief  To get module info using module name
     *  \iparam ModuleName = name of the Module
     *  \return Module info
     */
    /****************************************************************************/
    CModule* GetModule(const QString& ModuleName) const {
            return m_ModuleList.value(ModuleName, NULL);
    }

    /****************************************************************************/
    /*!
     *  \brief  To get module info using Index
     *  \iparam Index
     *  \return Module info
     */
    /****************************************************************************/
    CModule* GetModule(const unsigned int Index); // uses order index

    /****************************************************************************/
    /*!
     *  \brief  To get module info using ModuleName
     *  \iparam ModuleName = name of the Module
     *  \iparam Module = Module Object
     *  \return Module info
     */
    /****************************************************************************/
    bool GetModule(const QString& ModuleName, CModule& Module);

    /****************************************************************************/
    /*!
     *  \brief  To read XML file
     *  \iparam FileName
     *  \return true on success, false on failure
     */
    /****************************************************************************/
    bool ReadFile(const QString& FileName);

    /****************************************************************************/
    /*!
     *  \brief  To writes data to container
     *  \iparam p_Module = Module Object
     *  \return true on success, false on failure
     */
    /****************************************************************************/
    bool AddModule(CModule const* p_Module);

    /****************************************************************************/
    /*!
     *  \brief  Deletes the Module
     *  \iparam Index
     *  \return true - delete success , false - delete failure
     */
    /****************************************************************************/
    bool DeleteModule(const unsigned int Index);

    /****************************************************************************/
    /*!
     *  \brief  Deletes the Module
     *  \iparam ModuleName
     *  \return true - delete success , false - delete failure
     */
    /****************************************************************************/
    bool DeleteModule(const QString& ModuleName);

    /****************************************************************************/
    /*!
     *  \brief  Deletes all the Modules in the list
     *  \return true - delete success , false - delete failure
     */
    /****************************************************************************/
    bool DeleteAllModules();

    /****************************************************************************/
    /*!
     *  \brief Sets the XML file name
     *  \iparam Value = Filename to set
     */
    /****************************************************************************/
    void SetFileName(const QString Value) { m_FileName = Value; }

    /****************************************************************************/
    /*!
     *  \brief Returns the filename of the xml file read
     *  \return File name
     */
    /****************************************************************************/
    QString GetFilename() {return m_FileName;}

    /****************************************************************************/
    /*!
     *  \brief Sets verification mode
     *  \iparam value = true-verification on, false - verification off
     */
    /****************************************************************************/
    void SetDataVerificationMode(const bool value) { m_DataVerificationMode = value; }

    /****************************************************************************/
    /*!
     *  \brief Retrieves the status of Verification
     *  \return true-verification on , false-verification off
     */
    /****************************************************************************/
    bool GetDataVerificationMode() { return m_DataVerificationMode; }

    /****************************************************************************/
    /*!
     *  \brief  To update module info to XML
     *  \iparam p_Module = Object of Module
     *  \return true on success, false on failure
     */
    /****************************************************************************/
    bool UpdateModule(CModule const* p_Module);  // content of p_Module will be copied  => delete outside!

};

}   // namespace ServiceDataManager

#endif // SERVICE_DATAMANAGER_MODULEDATALIST_H
