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
#include "InternalUtil.h"
#include "MWException.h"
#include "eidErrors.h"

#include "APLCard.h"
#include "APLCardBeid.h"
#include "APLCardSIS.h"
#include "APLCrypto.h"
#include "APLCertif.h"

#include "ByteArray.h"

//UNIQUE INDEX FOR RETRIEVING OBJECT
#define INCLUDE_OBJECT_DOCEID			1
#define INCLUDE_OBJECT_PICTUREEID		2
#define INCLUDE_OBJECT_PINS				3
#define INCLUDE_OBJECT_CERTIFICATES		4
#define INCLUDE_OBJECT_DOCINFO			5
#define INCLUDE_OBJECT_FULLDOC			6
#define INCLUDE_OBJECT_CHALLENGE		7
#define INCLUDE_OBJECT_RESPONSE			8

#define INCLUDE_OBJECT_DOCSIS			11

#define INCLUDE_OBJECT_RAWDATA_ID			21
#define INCLUDE_OBJECT_RAWDATA_ID_SIG		22
#define INCLUDE_OBJECT_RAWDATA_ADDR			23
#define INCLUDE_OBJECT_RAWDATA_ADDR_SIG		24
#define INCLUDE_OBJECT_RAWDATA_PICTURE		25
#define INCLUDE_OBJECT_RAWDATA_CARD_INFO	26
#define INCLUDE_OBJECT_RAWDATA_TOKEN_INFO	27
#define INCLUDE_OBJECT_RAWDATA_CERT_RRN		28
#define INCLUDE_OBJECT_RAWDATA_CHALLENGE	29
#define INCLUDE_OBJECT_RAWDATA_RESPONSE		30

namespace eIDMW
{

/*****************************************************************************************
---------------------------------------- Card --------------------------------------------
*****************************************************************************************/
BEID_Card::BEID_Card(const SDK_Context *context,APL_Card *impl):BEID_Object(context,impl)
{
}

BEID_Card::~BEID_Card()
{
}

BEID_CardType BEID_Card::getType()
{
	BEID_CardType out = BEID_CARDTYPE_UNKNOWN;
	BEGIN_TRY_CATCH

	APL_Card *pcard=static_cast<APL_Card *>(m_impl);
	out = ConvertCardType(pcard->getType());
	
	END_TRY_CATCH

	return out;
}

BEID_ByteArray BEID_Card::sendAPDU(const BEID_ByteArray& cmd)
{
	BEID_ByteArray out;

	BEGIN_TRY_CATCH

	APL_Card *pcard=static_cast<APL_Card *>(m_impl);

	CByteArray apdu(cmd.GetBytes(),cmd.Size());
	CByteArray result=pcard->sendAPDU(apdu);
	out.Append(result.GetBytes(),result.Size());

	END_TRY_CATCH

	return out;
}

BEID_ByteArray BEID_Card::readFile(const char *fileID, unsigned long  ulOffset, unsigned long  ulMaxLength)
{
	BEID_ByteArray out;

	BEGIN_TRY_CATCH

	APL_Card *pcard=static_cast<APL_Card *>(m_impl);

	CByteArray oData;

	pcard->readFile(fileID, oData, ulOffset,ulMaxLength);

	out.Append(oData.GetBytes(),oData.Size());

	END_TRY_CATCH

	return out;
}

bool BEID_Card::writeFile(const char *fileID,const BEID_ByteArray& oData,unsigned long ulOffset)
{
	bool out = false;
	BEGIN_TRY_CATCH

	APL_Card *pcard=static_cast<APL_Card *>(m_impl);

	CByteArray bytearray(oData.GetBytes(),oData.Size());
	out = pcard->writeFile(fileID,bytearray,ulOffset);
	
	END_TRY_CATCH
	return out;
}

/*****************************************************************************************
---------------------------------------- BEID_MemoryCard --------------------------------------
*****************************************************************************************/
BEID_MemoryCard::BEID_MemoryCard(const SDK_Context *context,APL_Card *impl):BEID_Card(context,impl)
{
}

BEID_MemoryCard::~BEID_MemoryCard()
{
}

/*****************************************************************************************
---------------------------------------- BEID_SmartCard --------------------------------------
*****************************************************************************************/
BEID_SmartCard::BEID_SmartCard(const SDK_Context *context,APL_Card *impl):BEID_Card(context,impl)
{
}

BEID_SmartCard::~BEID_SmartCard()
{
}

void BEID_SmartCard::selectApplication(const BEID_ByteArray &applicationId)
{
	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	CByteArray bytearray(applicationId.GetBytes(),applicationId.Size());

	pcard->selectApplication(bytearray);
	
	END_TRY_CATCH
}

BEID_ByteArray BEID_SmartCard::sendAPDU(const BEID_ByteArray& cmd,BEID_Pin *pin,const char *csPinCode)
{
	BEID_ByteArray out;

	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	APL_Pin *pimplPin=NULL;

	if(pin)
		pimplPin=pcard->getPins()->getPinByNumber(pin->getIndex());

	CByteArray apdu(cmd.GetBytes(),cmd.Size());

	CByteArray result=pcard->sendAPDU(apdu,pimplPin,csPinCode);

	out.Append(result.GetBytes(),result.Size());
	
	END_TRY_CATCH

	return out;
}

long BEID_SmartCard::readFile(const char *fileID, BEID_ByteArray &in,BEID_Pin *pin,const char *csPinCode)
{
	long out = 0;	

	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	CByteArray bytearray;

	APL_Pin *pimplPin=NULL;

	if(pin)
		pimplPin=pcard->getPins()->getPinByNumber(pin->getIndex());

	out=pcard->readFile(fileID,bytearray,pimplPin,csPinCode);
	in=bytearray;
	
	END_TRY_CATCH

	return out;
}

bool BEID_SmartCard::writeFile(const char *fileID,const BEID_ByteArray &baOut,BEID_Pin *pin,const char *csPinCode)
{
	bool out = false;	

	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	CByteArray bytearray(baOut.GetBytes(),baOut.Size());

	APL_Pin *pimplPin=NULL;

	if(pin)
		pimplPin=pcard->getPins()->getPinByNumber(pin->getIndex());

	out=pcard->writeFile(fileID,bytearray,pimplPin,csPinCode);
	
	END_TRY_CATCH

	return out;
}

unsigned long BEID_SmartCard::pinCount() 
{ 
	unsigned long out = 0;

	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);
	out = pcard->pinCount();
	
	END_TRY_CATCH

	return out;
}

BEID_Pins& BEID_SmartCard::getPins()
{
	BEID_Pins *out = NULL;

	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	out=dynamic_cast<BEID_Pins *>(getObject(INCLUDE_OBJECT_PINS));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//ppins=dynamic_cast<BEID_Pins *>(getObject(INCLUDE_OBJECT_PINS));
		//if(!ppins)
		//{
			out = new BEID_Pins(m_context,pcard->getPins());
			if(out)
				m_objects[INCLUDE_OBJECT_PINS]=out;
			else
				throw BEID_ExUnknown();
		//}
	}

	END_TRY_CATCH

	return *out;
}

unsigned long BEID_SmartCard::certificateCount()
{
	unsigned long out = 0;
	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);
	out = pcard->certificateCount();
	
	END_TRY_CATCH
	
	return out;
}

BEID_Certificates& BEID_SmartCard::getCertificates()
{
	BEID_Certificates *out = NULL;

	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	out = dynamic_cast<BEID_Certificates *>(getObject(INCLUDE_OBJECT_CERTIFICATES));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pcerts=dynamic_cast<BEID_Certificates *>(getObject(INCLUDE_OBJECT_CERTIFICATES));
		//if(!pcerts)
		//{
			out = new BEID_Certificates(m_context,pcard->getCertificates());
			if(out)
				m_objects[INCLUDE_OBJECT_CERTIFICATES]=out;
			else
				throw BEID_ExUnknown();
		//}
	}

	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_SmartCard::getChallenge(bool bForceNewInit)
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	if(bForceNewInit)
	{
		delObject(INCLUDE_OBJECT_CHALLENGE);
		delObject(INCLUDE_OBJECT_RESPONSE);
	}

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_CHALLENGE));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_CHALLENGE));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getChallenge(bForceNewInit));
			if(out)
				m_objects[INCLUDE_OBJECT_CHALLENGE]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_SmartCard::getChallengeResponse()
{
	BEID_ByteArray *out = NULL;
	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RESPONSE));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RESPONSE));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getChallengeResponse());
			if(out)
				m_objects[INCLUDE_OBJECT_RESPONSE]=out;
			else
				throw BEID_ExUnknown();
		//}
	}

	END_TRY_CATCH

	return *out;
}

bool BEID_SmartCard::verifyChallengeResponse(const BEID_ByteArray &challenge, const BEID_ByteArray &response) const
{
	bool out = false;

	BEGIN_TRY_CATCH

	APL_SmartCard *pcard=static_cast<APL_SmartCard *>(m_impl);

	CByteArray baChallenge(challenge.GetBytes(),challenge.Size());
	CByteArray baResponse(response.GetBytes(),response.Size());

	out = pcard->verifyChallengeResponse(baChallenge,baResponse);
	
	END_TRY_CATCH

	return out;
}

/*****************************************************************************************
---------------------------------------- BEID_SISCard -----------------------------------------
*****************************************************************************************/
BEID_SISCard::BEID_SISCard(const SDK_Context *context,APL_Card *impl):BEID_MemoryCard(context,impl)
{
}

BEID_SISCard::~BEID_SISCard()
{
}

BEID_XMLDoc& BEID_SISCard::getDocument(BEID_DocumentType type)
{
	switch(type)
	{
	case BEID_DOCTYPE_FULL:
		return getFullDoc();
	case BEID_DOCTYPE_ID:
		return getID();
	default:
		throw BEID_ExDocTypeUnknown();
	}
}

BEID_SisFullDoc& BEID_SISCard::getFullDoc()
{
	BEID_SisFullDoc *out = NULL;

	BEGIN_TRY_CATCH

	APL_SISCard *pcard=static_cast<APL_SISCard *>(m_impl);

	out = dynamic_cast<BEID_SisFullDoc *>(getObject(INCLUDE_OBJECT_FULLDOC));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pdoc=dynamic_cast<BEID_SisFullDoc *>(getObject(INCLUDE_OBJECT_FULLDOC));
		//if(!pdoc)
		//{
			out = new BEID_SisFullDoc(m_context,&pcard->getFullDoc());
			if(out)
				m_objects[INCLUDE_OBJECT_FULLDOC]=out;
			else
				throw BEID_ExUnknown();
		//}
	}

	END_TRY_CATCH

	return *out;
}

BEID_SisId& BEID_SISCard::getID()
{
	BEID_SisId *out = NULL;

	BEGIN_TRY_CATCH

	APL_SISCard *pcard=static_cast<APL_SISCard *>(m_impl);

	out = dynamic_cast<BEID_SisId *>(getObject(INCLUDE_OBJECT_DOCSIS));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pdoc=dynamic_cast<BEID_SisId *>(getObject(INCLUDE_OBJECT_DOCSIS));
		//if(!pdoc)
		//{
			out = new BEID_SisId(m_context,&pcard->getID());
			if(out)
				m_objects[INCLUDE_OBJECT_DOCSIS]=out;
			else
				throw BEID_ExUnknown();
		//}
	}

	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_SISCard::getRawData(BEID_RawDataType type)
{
	switch(type)
	{
	case BEID_RAWDATA_ID:
		return getRawData_Id();
	default:
		throw BEID_ExFileTypeUnknown();
	}
}

const BEID_ByteArray& BEID_SISCard::getRawData_Id()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_SISCard *pcard=static_cast<APL_SISCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ID));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ID));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_Id());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_ID]=out;
			else
				throw BEID_ExUnknown();
		//}
	}

	END_TRY_CATCH

	return *out;
}

/*****************************************************************************************
---------------------------------------- BEID_EIDCard -----------------------------------------
*****************************************************************************************/
BEID_EIDCard::BEID_EIDCard(const SDK_Context *context,APL_Card *impl):BEID_SmartCard(context,impl)
{
}

BEID_EIDCard::~BEID_EIDCard()
{
}

bool BEID_EIDCard::isApplicationAllowed()
{
	bool out = true;

	try														
	{
		out = APL_EIDCard::isApplicationAllowed();
	}
	catch(CMWException &e)
	{									
		e.GetError();				    
		throw BEID_Exception::THROWException(e);	
	}				

	return out;
}

bool BEID_EIDCard::isTestCard()
{
	bool out = true;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);
	out = pcard->isTestCard();
	
	END_TRY_CATCH

	return out;
}

bool BEID_EIDCard::getAllowTestCard()
{
	bool out = false;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);
	out =  pcard->getAllowTestCard();
	
	END_TRY_CATCH

	return out;
}

void BEID_EIDCard::setAllowTestCard(bool allow)
{
	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);
	pcard->setAllowTestCard(allow);
	
	END_TRY_CATCH
}

BEID_CertifStatus BEID_EIDCard::getDataStatus()
{
	BEID_CertifStatus out = BEID_CERTIF_STATUS_UNKNOWN;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);
	out =  ConvertCertStatus(pcard->getDataStatus());
	
	END_TRY_CATCH

	return out;
}

BEID_XMLDoc& BEID_EIDCard::getDocument(BEID_DocumentType type)
{
	switch(type)
	{
	case BEID_DOCTYPE_FULL:
		return getFullDoc();
	case BEID_DOCTYPE_ID:
		return getID();
	case BEID_DOCTYPE_PICTURE:
		return getPicture();
	case BEID_DOCTYPE_INFO:
		return getVersionInfo();
	case BEID_DOCTYPE_PINS:
		return getPins();
	case BEID_DOCTYPE_CERTIFICATES:
		return getCertificates();
	default:
		throw BEID_ExDocTypeUnknown();
	}
}

BEID_EIdFullDoc& BEID_EIDCard::getFullDoc()
{
	BEID_EIdFullDoc *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_EIdFullDoc *>(getObject(INCLUDE_OBJECT_FULLDOC));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pdoc=dynamic_cast<BEID_EIdFullDoc *>(getObject(INCLUDE_OBJECT_FULLDOC));
		//if(!pdoc)
		//{
			out = new BEID_EIdFullDoc(m_context,&pcard->getFullDoc());
			if(out)
				m_objects[INCLUDE_OBJECT_FULLDOC]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

BEID_EId& BEID_EIDCard::getID()
{
	BEID_EId *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out=dynamic_cast<BEID_EId *>(getObject(INCLUDE_OBJECT_DOCEID));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pdoc=dynamic_cast<BEID_EId *>(getObject(INCLUDE_OBJECT_DOCEID));
		//if(!pdoc)
		//{
			out = new BEID_EId(m_context,&pcard->getID());
			if(out)
				m_objects[INCLUDE_OBJECT_DOCEID]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

BEID_Picture& BEID_EIDCard::getPicture()
{
	BEID_Picture *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_Picture *>(getObject(INCLUDE_OBJECT_PICTUREEID));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//ppicture=dynamic_cast<BEID_Picture *>(getObject(INCLUDE_OBJECT_PICTUREEID));
		//if(!ppicture)
		//{
			out = new BEID_Picture(m_context,&pcard->getPicture());
			if(out)
				m_objects[INCLUDE_OBJECT_PICTUREEID]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

BEID_CardVersionInfo& BEID_EIDCard::getVersionInfo()
{
	BEID_CardVersionInfo *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_CardVersionInfo *>(getObject(INCLUDE_OBJECT_DOCINFO));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//pdoc=dynamic_cast<BEID_CardVersionInfo *>(getObject(INCLUDE_OBJECT_DOCINFO));
		//if(!pdoc)
		//{
			out = new BEID_CardVersionInfo(m_context,&pcard->getDocInfo());
			if(out)
				m_objects[INCLUDE_OBJECT_DOCINFO]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

BEID_Certificate &BEID_EIDCard::getCert(BEID_CertifType type)
{
	return getCertificates().getCert(type);
}

BEID_Certificate &BEID_EIDCard::getRrn()
{
	return getCertificates().getCert(BEID_CERTIF_TYPE_RRN);
}

BEID_Certificate &BEID_EIDCard::getRoot()
{
	return getCertificates().getCert(BEID_CERTIF_TYPE_ROOT);
}

BEID_Certificate &BEID_EIDCard::getCA()
{
	return getCertificates().getCert(BEID_CERTIF_TYPE_CA);
}

BEID_Certificate &BEID_EIDCard::getAuthentication()
{
	return getCertificates().getCert(BEID_CERTIF_TYPE_AUTHENTICATION);
}

BEID_Certificate &BEID_EIDCard::getSignature()
{
	return getCertificates().getCert(BEID_CERTIF_TYPE_SIGNATURE);
}

const BEID_ByteArray& BEID_EIDCard::getRawData(BEID_RawDataType type)
{	
	switch(type)
	{
	case BEID_RAWDATA_ID:
		return getRawData_Id();
	case BEID_RAWDATA_ID_SIG:
		return getRawData_IdSig();
	case BEID_RAWDATA_ADDR:
		return getRawData_Addr();
	case BEID_RAWDATA_ADDR_SIG:
		return getRawData_AddrSig();
	case BEID_RAWDATA_PICTURE:
		return getRawData_Picture();
	case BEID_RAWDATA_CARD_INFO:
		return getRawData_CardInfo();
	case BEID_RAWDATA_TOKEN_INFO:
		return getRawData_TokenInfo();
	case BEID_RAWDATA_CERT_RRN:
		return getRawData_CertRRN();
	case BEID_RAWDATA_CHALLENGE:
		return getRawData_Challenge();
	case BEID_RAWDATA_RESPONSE:
		return getRawData_Response();
	default:
		throw BEID_ExFileTypeUnknown();
	}
}

const BEID_ByteArray& BEID_EIDCard::getRawData_Id()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ID));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ID));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_Id());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_ID]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_IdSig()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ID_SIG));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ID_SIG));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_IdSig());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_ID_SIG]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_Addr()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ADDR));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ADDR));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_Addr());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_ADDR]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_AddrSig()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ADDR_SIG));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_ADDR_SIG));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_AddrSig());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_ADDR_SIG]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_Picture()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_PICTURE));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_PICTURE));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_Picture());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_PICTURE]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_CardInfo()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_CARD_INFO));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_CARD_INFO));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_CardInfo());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_CARD_INFO]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_TokenInfo()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_TOKEN_INFO));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_TOKEN_INFO));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_TokenInfo());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_TOKEN_INFO]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_CertRRN()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_CERT_RRN));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_CERT_RRN));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_CertRRN());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_CERT_RRN]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_Challenge()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_CHALLENGE));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_CHALLENGE));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_Challenge());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_CHALLENGE]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

const BEID_ByteArray& BEID_EIDCard::getRawData_Response()
{
	BEID_ByteArray *out = NULL;

	BEGIN_TRY_CATCH

	APL_EIDCard *pcard=static_cast<APL_EIDCard *>(m_impl);

	out = dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_RESPONSE));

	if(!out)
	{
		//CAutoMutex autoMutex(m_mutex);

		//praw=dynamic_cast<BEID_ByteArray *>(getObject(INCLUDE_OBJECT_RAWDATA_RESPONSE));
		//if(!praw)
		//{
			out = new BEID_ByteArray(m_context,pcard->getRawData_Response());
			if(out)
				m_objects[INCLUDE_OBJECT_RAWDATA_RESPONSE]=out;
			else
				throw BEID_ExUnknown();
		//}
	}
	
	END_TRY_CATCH

	return *out;
}

/*****************************************************************************************
---------------------------------------- BEID_KidsCard -----------------------------------------
*****************************************************************************************/
BEID_KidsCard::BEID_KidsCard(const SDK_Context *context,APL_Card *impl):BEID_EIDCard(context,impl)
{
}

BEID_KidsCard::~BEID_KidsCard()
{
}

/*****************************************************************************************
---------------------------------------- BEID_ForeignerCard -----------------------------------------
*****************************************************************************************/
BEID_ForeignerCard::BEID_ForeignerCard(const SDK_Context *context,APL_Card *impl):BEID_EIDCard(context,impl)
{
}

BEID_ForeignerCard::~BEID_ForeignerCard()
{
}

}
