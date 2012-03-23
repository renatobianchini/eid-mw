/* ****************************************************************************

 * eID Middleware Project.
 * Copyright (C) 2008-2009 FedICT.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version
 * 3.0 as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, see
 * http://www.gnu.org/licenses/.

**************************************************************************** */
#include "eidlib.h"

#include "eidlibException.h"
#include "MWException.h"
#include "eidErrors.h"

#include "InternalUtil.h"

#include "APLDoc.h"
#include "APLCrypto.h"
#include "APLCardSIS.h"
#include "APLCardBeid.h"

//UNIQUE INDEX FOR RETRIEVING OBJECT
#define INCLUDE_OBJECT_PICTUREEID_DATA	1
#define INCLUDE_OBJECT_PICTUREEID_HASH	2
#define INCLUDE_OBJECT_INFOEID_SIGN		3

namespace eIDMW
{

/*****************************************************************************************
---------------------------------------- BEID_ByteArray --------------------------------------
*****************************************************************************************/
BEID_ByteArray::BEID_ByteArray():BEID_Object(NULL,new CByteArray())
{
	m_delimpl=true;
}
	
BEID_ByteArray::BEID_ByteArray(const unsigned char * pucData, unsigned long ulSize):BEID_Object(NULL,new CByteArray(pucData,ulSize))
{
	m_delimpl=true;
}

BEID_ByteArray::BEID_ByteArray(const BEID_ByteArray &bytearray):BEID_Object(bytearray.m_context,new CByteArray(*(CByteArray*)bytearray.m_impl))
{
	m_delimpl=true;
}

BEID_ByteArray::BEID_ByteArray(const SDK_Context *context,const CByteArray &impl):BEID_Object(context,(void *)&impl)
{
}

BEID_ByteArray::~BEID_ByteArray()
{
	if(m_delimpl)
	{
		CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
		delete pimpl;
		m_impl=NULL;
	}
}

BEID_ByteArray & BEID_ByteArray::operator=(const CByteArray &bytearray)
{
	if(m_delimpl)
	{
		CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
		delete pimpl;
		m_impl=NULL;
	}

	m_impl=new CByteArray(bytearray.GetBytes(),bytearray.Size());
	m_delimpl=true;

	return *this;
}

BEID_ByteArray & BEID_ByteArray::operator=(const BEID_ByteArray &bytearray)
{
	if(m_delimpl)
	{
		CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
		delete pimpl;
		m_impl=NULL;
	}

	m_impl=new CByteArray(bytearray.GetBytes(),bytearray.Size());
	m_delimpl=true;

	return *this;
}

unsigned long BEID_ByteArray::Size() const
{
	unsigned long out = 0;

	CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
	out = pimpl->Size();

	return out;
}

const unsigned char *BEID_ByteArray::GetBytes() const
{
	const unsigned char *out = NULL;

	CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
	out = pimpl->GetBytes();

	return out;
}

void BEID_ByteArray::Append(const unsigned char * pucData, unsigned long ulSize)
{
	if(!m_delimpl)	//If the CByteArray has not been created, it can't be changed
		throw BEID_ExBadUsage();

	BEGIN_TRY_CATCH

	CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
	pimpl->Append(pucData,ulSize);

	END_TRY_CATCH
}

void BEID_ByteArray::Append(const BEID_ByteArray &data)
{
	if(!m_delimpl)	//If the CByteArray has not been created, it can't be changed
		throw BEID_ExBadUsage();

	BEGIN_TRY_CATCH

	CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
	CByteArray *apl_data=static_cast<CByteArray *>(data.m_impl);
	pimpl->Append(*apl_data);

	END_TRY_CATCH
}

void BEID_ByteArray::Clear()
{
	if(!m_delimpl)	//If the CByteArray has not been created, it can't be changed
		throw BEID_ExBadUsage();

	BEGIN_TRY_CATCH

	CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
	pimpl->ClearContents();

	END_TRY_CATCH
}

bool BEID_ByteArray::Equals(const BEID_ByteArray &data) const
{
	bool out = false;

	CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
	CByteArray *data_pimpl=static_cast<CByteArray *>(data.m_impl);

	out = pimpl->Equals(*data_pimpl);

	return out;
}

bool BEID_ByteArray::writeToFile(const char * csFilePath)
{
	bool out=false;

	BEGIN_TRY_CATCH

	CByteArray *pimpl=static_cast<CByteArray *>(m_impl);
	out=APL_XMLDoc::writeToFile(*pimpl,csFilePath);

	END_TRY_CATCH

	return out;
}

/*****************************************************************************************
---------------------------------------- BEID_XMLDoc -------------------------------------------
*****************************************************************************************/
BEID_XMLDoc::BEID_XMLDoc(const SDK_Context *context,APL_XMLDoc *impl):BEID_Object(context,impl)
{
}

BEID_XMLDoc::~BEID_XMLDoc()
{
	if(m_delimpl && m_impl)
	{
		APL_XMLDoc *pimpl=static_cast<APL_XMLDoc *>(m_impl);
		delete pimpl;
		m_impl=NULL;
	}
}

bool BEID_XMLDoc::isAllowed()
{
	bool out;

	BEGIN_TRY_CATCH

	APL_XMLDoc *pimpl=static_cast<APL_XMLDoc *>(m_impl);

	out=pimpl->isAllowed();
	
	END_TRY_CATCH

	return out;
}

BEID_ByteArray BEID_XMLDoc::getXML()
{
	BEID_ByteArray out;

	BEGIN_TRY_CATCH

	APL_XMLDoc *pimpl=static_cast<APL_XMLDoc *>(m_impl);

	CByteArray result=pimpl->getXML();
	out.Append(result.GetBytes(),result.Size());
	
	END_TRY_CATCH

	return out;
}

BEID_ByteArray BEID_XMLDoc::getCSV()
{
	BEID_ByteArray out;

	BEGIN_TRY_CATCH

	APL_XMLDoc *pimpl=static_cast<APL_XMLDoc *>(m_impl);

	CByteArray result=pimpl->getCSV();
	out.Append(result.GetBytes(),result.Size());
	
	END_TRY_CATCH

	return out;
}

BEID_ByteArray BEID_XMLDoc::getTLV()
{
	BEID_ByteArray out;

	BEGIN_TRY_CATCH

	APL_XMLDoc *pimpl=static_cast<APL_XMLDoc *>(m_impl);

	CByteArray result=pimpl->getTLV();
	out.Append(result.GetBytes(),result.Size());
	
	END_TRY_CATCH

	return out;
}

bool BEID_XMLDoc::writeXmlToFile(const char * csFilePath)
{
	bool out = false;

	BEGIN_TRY_CATCH

	APL_XMLDoc *pimpl=static_cast<APL_XMLDoc *>(m_impl);
	out = pimpl->writeXmlToFile(csFilePath);
	
	END_TRY_CATCH

	return out;
}

bool BEID_XMLDoc::writeCsvToFile(const char * csFilePath)
{
	bool out = false;

	BEGIN_TRY_CATCH

	APL_XMLDoc *pimpl=static_cast<APL_XMLDoc *>(m_impl);
	out = pimpl->writeCsvToFile(csFilePath);
	
	END_TRY_CATCH

	return out;
}

bool BEID_XMLDoc::writeTlvToFile(const char * csFilePath)
{
	bool out = false;

	BEGIN_TRY_CATCH

	APL_XMLDoc *pimpl=static_cast<APL_XMLDoc *>(m_impl);
	out = pimpl->writeTlvToFile(csFilePath);
	
	END_TRY_CATCH

	return out;
}

/*****************************************************************************************
---------------------------------------- BEID_Biometric -------------------------------------------
*****************************************************************************************/
BEID_Biometric::BEID_Biometric(const SDK_Context *context,APL_Biometric *impl):BEID_XMLDoc(context,impl)
{
}

BEID_Biometric::~BEID_Biometric()
{
}

/*****************************************************************************************
---------------------------------------- BEID_Crypto -------------------------------------------
*****************************************************************************************/
BEID_Crypto::BEID_Crypto(const SDK_Context *context,APL_Crypto *impl):BEID_XMLDoc(context,impl)
{
}

BEID_Crypto::~BEID_Crypto()
{
}

/*****************************************************************************************
---------------------------------------- BEID_SisFullDoc -------------------------------------------
*****************************************************************************************/
BEID_SisFullDoc::BEID_SisFullDoc(const SDK_Context *context,APL_SisFullDoc *impl):BEID_XMLDoc(context,impl)
{
}

BEID_SisFullDoc::~BEID_SisFullDoc()
{
}

/*****************************************************************************************
---------------------------------------- BEID_EIdFullDoc -------------------------------------------
*****************************************************************************************/
BEID_EIdFullDoc::BEID_EIdFullDoc(const SDK_Context *context,APL_EIdFullDoc *impl):BEID_XMLDoc(context,impl)
{
}

BEID_EIdFullDoc::~BEID_EIdFullDoc()
{
}

/*****************************************************************************************
---------------------------------------- BEID_CardVersionInfo --------------------------------------
*****************************************************************************************/
BEID_CardVersionInfo::BEID_CardVersionInfo(const SDK_Context *context,APL_DocVersionInfo *impl):BEID_XMLDoc(context,impl)
{
}

BEID_CardVersionInfo::~BEID_CardVersionInfo()
{
}

const char *BEID_CardVersionInfo::getSerialNumber()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getSerialNumber();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getComponentCode()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getComponentCode();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getOsNumber()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getOsNumber();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getOsVersion()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getOsVersion();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getSoftmaskNumber()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getSoftmaskNumber();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getSoftmaskVersion()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getSoftmaskVersion();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getAppletVersion()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getAppletVersion();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getGlobalOsVersion()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getGlobalOsVersion();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getAppletInterfaceVersion()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getAppletInterfaceVersion();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getPKCS1Support()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getPKCS1Support();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getKeyExchangeVersion()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getKeyExchangeVersion();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getAppletLifeCycle()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getAppletLifeCicle();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getGraphicalPersonalisation()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getGraphicalPersonalisation();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getElectricalPersonalisation()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getElectricalPersonalisation();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_CardVersionInfo::getElectricalPersonalisationInterface()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);
	out = pimpl->getElectricalPersonalisationInterface();
	
	END_TRY_CATCH

	return out;
}

const BEID_ByteArray &BEID_CardVersionInfo::getSignature()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocVersionInfo *pimpl=static_cast<APL_DocVersionInfo *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_INFOEID_SIGN));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pbytearray=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_INFOEID_SIGN));
		//if(!pbytearray)
		//{
			out = new BEID_ByteArray(m_context,pimpl->getSignature());
			if(out)
				m_objects[INCLUDE_OBJECT_INFOEID_SIGN]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}
/*****************************************************************************************
---------------------------------------- SisId -------------------------------------------
*****************************************************************************************/
BEID_SisId::BEID_SisId(const SDK_Context *context,APL_DocSisId *impl):BEID_XMLDoc(context,impl)
{
}

BEID_SisId::~BEID_SisId()
{
}

const char *BEID_SisId::getName()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getName();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getSurname()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getSurname();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getInitials()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getInitials();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getGender()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getGender();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getDateOfBirth()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getDateOfBirth();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getSocialSecurityNumber()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getSocialSecurityNumber();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getLogicalNumber()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getLogicalNumber();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getDateOfIssue()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getDateOfIssue();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getValidityBeginDate()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getValidityBeginDate();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_SisId::getValidityEndDate()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocSisId *pimpl=static_cast<APL_DocSisId *>(m_impl);
	out = pimpl->getValidityEndDate();
	
	END_TRY_CATCH

	return out;
}



/*****************************************************************************************
---------------------------------------- BEID_EId ---------------------------------------------
*****************************************************************************************/
BEID_EId::BEID_EId(const SDK_Context *context,APL_DocEId *impl):BEID_XMLDoc(context,impl)
{
}

BEID_EId::~BEID_EId()
{
}

const char *BEID_EId::getDocumentVersion()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getDocumentVersion();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getDocumentType()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getDocumentType();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getFirstName()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getFirstName();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getFirstName1()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getFirstName1();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getFirstName2()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getFirstName2();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getSurname()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getSurname();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getGender()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getGender();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getDateOfBirth()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getDateOfBirth();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getLocationOfBirth()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getLocationOfBirth();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getNobility()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getNobility();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getNationality()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getNationality();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getNationalNumber()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getNationalNumber();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getDuplicata()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getDuplicata();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getSpecialOrganization()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getSpecialOrganization();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getMemberOfFamily()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getMemberOfFamily();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getLogicalNumber()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getLogicalNumber();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getChipNumber()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getChipNumber();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getValidityBeginDate()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getValidityBeginDate();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getValidityEndDate()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getValidityEndDate();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getIssuingMunicipality()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getIssuingMunicipality();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getAddressVersion()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getAddressVersion();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getStreet()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getStreet();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getZipCode()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getZipCode();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getMunicipality()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getMunicipality();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getCountry()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getCountry();
	
	END_TRY_CATCH

	return out;
}

const char *BEID_EId::getSpecialStatus()
{
	const char *out = NULL;

	BEGIN_TRY_CATCH

	APL_DocEId *pimpl=static_cast<APL_DocEId *>(m_impl);
	out = pimpl->getSpecialStatus();
	
	END_TRY_CATCH

	return out;
}


/*****************************************************************************************
---------------------------------------- BEID_Picture -----------------------------------------
*****************************************************************************************/
BEID_Picture::BEID_Picture(const SDK_Context *context,APL_PictureEid *impl):BEID_Biometric(context,impl)
{
}

BEID_Picture::~BEID_Picture()
{
}

const BEID_ByteArray& BEID_Picture::getData()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_PictureEid *ppicture=static_cast<APL_PictureEid *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_PICTUREEID_DATA));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pbytearray=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_PICTUREEID_DATA));
		//if(!pbytearray)
		//{
			out = new BEID_ByteArray(m_context,ppicture->getData());
			if(out)
				m_objects[INCLUDE_OBJECT_PICTUREEID_DATA]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_Picture::getHash()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_PictureEid *ppicture=static_cast<APL_PictureEid *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_PICTUREEID_HASH));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pbytearray=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_PICTUREEID_HASH));
		//if(!pbytearray)
		//{
			out = new BEID_ByteArray(m_context,ppicture->getHash());
			if(out)
				m_objects[INCLUDE_OBJECT_PICTUREEID_HASH]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}
	
}
