/****************************************************************************/
/*! \file   ProgramSettingsVerifier.h
 *
 *  \brief   ProgramSettingsVerifier class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-04-03
 *   $Author:  $ L. Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH. CN.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef DATAMANAGER_PROGRAMSETTINGSVERIFIER_H
#define DATAMANAGER_PROGRAMSETTINGSVERIFIER_H

#include <QString>
#include <QXmlStreamReader>

#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"

namespace DataManager {

#define MAX_SPEED_PROFILE   4        ///<  Definition/Declaration of macro MAX_SPEED_PROFILE
#define MAX_POSITION_VALUE  4500     ///<  Definition/Declaration of macro MAX_POSITION_VALUE
#define MAX_OFFSET_VALUE    500      ///<  Definition/Declaration of macro MAX_OFFSET_VALUE

/****************************************************************************/
/*!
 *  \brief  This class implements verifiers for Parameter
 */
/****************************************************************************/
class CProgramSettingsVerifier : public IVerifierInterface
{
public:
    /**
      * \brief constructor
    */
    CProgramSettingsVerifier();

    /**
     * \brief use concrete class for concrete verifier
     * \iparam p_ParameterList paramenter list
     * \return success status
    */
    bool VerifyData(CDataContainerBase* p_ParameterList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetSyncObject
     *
     *  \param p_ReadWriteLock = QReadWriteLock type parameter
     *
     *  \return from GetSyncObject
     */
    /****************************************************************************/
    bool GetSyncObject(QReadWriteLock* p_ReadWriteLock);

    /*! \todo complete implementation*/
    ErrorMap_t& GetErrors();

    /**
      * \brief reset errors
    */
    void ResetErrors();

    /**
      * \brief is local verifier
    */
    bool IsLocalVerifier();

    /****************************************************************************/
    /*!
     *  \brief  Destructor
     */
    /****************************************************************************/
    virtual ~CProgramSettingsVerifier() {}

private:
    ErrorMap_t m_ErrorsHash;           //!< To store Error Key and any arguments associated
};

} // namespace DataManager

#endif // DATAMANAGER_PROGRAMSETTINGSVERIFIER_H
