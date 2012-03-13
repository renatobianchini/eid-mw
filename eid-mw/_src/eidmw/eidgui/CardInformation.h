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

#ifndef _CARDINFORMATION_H_
#define _CARDINFORMATION_H_

#include <QtGui>
#include "eidlib.h"
#include "eidlibException.h"

using namespace eIDMW;

typedef QMap<QString,QString> tFieldMap;
//**************************************************
// Card information
// This class contains all the card specific data
//**************************************************
class CardInfo
{
public:

#define CHIP_NUMBER			"chip_number"
#define CARD_NUMBER			"card_number"
#define CARD_TYPE			"card_type"
#define CARD_DATEOFISSUE	"card_dateofissue"
#define CARD_PLACEOFISSUE	"card_placeofissue"
#define CARD_ISSUING		"card_issuing"
#define CARD_VALIDFROM		"card_validfrom"
#define CARD_VALIDUNTIL		"card_validuntil"

enum eCARD_SUBTYPE
{
	  NON_EUROPEAN_A = 11
	, NON_EUROPEAN_B		// 12
	, NON_EUROPEAN_C		// 13
	, NON_EUROPEAN_D		// 14
	, EUROPEAN_E			// 15
	, EUROPEAN_E_PLUS		// 16
	, EUROPEAN_F			// 17
	, EUROPEAN_F_PLUS		// 18
	, EUROPEAN_UNKNOWN		// 19
};

enum eCARD_BASETYPE
{
	  BASETYPE_BELGIUM = 1
	, BASETYPE_KIDS	   = 6
	, BASETYPE_FOREIGNER = 11
	, BASETYPE_SIS = 99
};

	//----------------------------------------------
	// ctor
	//----------------------------------------------
	CardInfo( void )
	{
	}
	//----------------------------------------------
	// Reset: reset all the members to default values
	//----------------------------------------------
	void Reset( void )
	{
		m_Fields.clear();
	}
	//----------------------------------------------
	// Retrieve the card specific data 
	//----------------------------------------------
	bool RetrieveData(BEID_EIDCard& Card)
	{
		bool bRetVal = false;
		try
		{
			BEID_EId& beid_eid	= Card.getID();

			m_Fields[CARD_TYPE]			= QString::fromUtf8(beid_eid.getDocumentType());
			m_Fields[CHIP_NUMBER]		= QString::fromUtf8(beid_eid.getChipNumber());
			m_Fields[CARD_NUMBER]		= QString::fromUtf8(beid_eid.getLogicalNumber());
			m_Fields[CARD_PLACEOFISSUE]	= QString::fromUtf8(beid_eid.getIssuingMunicipality());
			m_Fields[CARD_VALIDFROM]	= QString::fromUtf8(beid_eid.getValidityBeginDate());
			m_Fields[CARD_VALIDUNTIL]	= QString::fromUtf8(beid_eid.getValidityEndDate());
		}
		catch (BEID_ExNotAllowByUser& e)
		{
			e = e;
			// ok fine, we don't read the personal information if not allowed
		}

// 		qDebug() << "CardInfo::RetrieveData()";
// 		for (tFieldMap::iterator it=m_Fields.begin(); it!=m_Fields.end(); it++)
// 		{
// 			qDebug() << "[" << it.key() << "] = " << "Value: " << it.value();
// 		}

		bRetVal = true;
		return bRetVal;

	}
	//----------------------------------------------
	// Retrieve the card specific data 
	//----------------------------------------------
	bool RetrieveData(BEID_SISCard& Card)
	{
		bool bRetVal = false;
		BEID_SisId& beid_sisid	= Card.getID();

		m_Fields[CARD_TYPE]			= "99";
		m_Fields[CARD_NUMBER]		= QString::fromUtf8(beid_sisid.getLogicalNumber());
		m_Fields[CARD_VALIDFROM]	= QString::fromUtf8(beid_sisid.getValidityBeginDate());
		m_Fields[CARD_VALIDUNTIL]	= QString::fromUtf8(beid_sisid.getValidityEndDate());

// 		qDebug() << "CardInfo::RetrieveData()";
// 		for (tFieldMap::iterator it=m_Fields.begin(); it!=m_Fields.end(); it++)
// 		{
// 			qDebug() << "[" << it.key() << "] = " << "Value: " << it.value();
// 		}
		bRetVal = true;
		return bRetVal;
	}

	//----------------------------------------------
	// get reference to all fields
	//----------------------------------------------
	tFieldMap& getFields( void )
	{
		return m_Fields;
	}
	BEID_CardType getType( void )
	{
		QString cardType = m_Fields[CARD_TYPE];
		BEID_CardType cType = BEID_CARDTYPE_UNKNOWN;
		if( cardType.size() > 0 )
		{
			int iCardType = cardType.toInt();
			switch(iCardType)
			{
			case 1:
				cType = BEID_CARDTYPE_EID;
				break;
			case 6:
				cType = BEID_CARDTYPE_KIDS;
				break;
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
				cType = BEID_CARDTYPE_FOREIGNER;
				break;
			case 99:
				cType = BEID_CARDTYPE_SIS;
				break;
			default:
				break;
			}
		}
		return cType;
	}
	eCARD_SUBTYPE getSubType( void )
	{
		QString cardType = m_Fields[CARD_TYPE];
		eCARD_SUBTYPE subType = EUROPEAN_UNKNOWN;
		if( cardType.size() > 0 )
		{
			int iCardType = cardType.toInt();
			switch(iCardType)
			{
			case 1:
			case 6:
			case 99:
				break;
			case 11:
				subType = NON_EUROPEAN_A;
				break;
			case 12:
				subType = NON_EUROPEAN_B;
				break;
			case 13:
				subType = NON_EUROPEAN_C;
				break;
			case 14:
				subType = NON_EUROPEAN_D;
				break;
			case 15:
				subType = EUROPEAN_E;
				break;
			case 16:
				subType = EUROPEAN_E_PLUS;
				break;
			case 17:
				subType = EUROPEAN_F;
				break;
			case 18:
				subType = EUROPEAN_F_PLUS;
				break;
			default:
				break;
			}
		}
		return subType;
	}
	static QString formatCardNumber(QString const& card_number, BEID_CardType type)
	{
		QString formatted="";

		if (card_number.length()==0)
		{
			return formatted;
		}

		switch ( type )
		{
		case BEID_CARDTYPE_EID:
		case BEID_CARDTYPE_KIDS:
			formatted += card_number.mid(0,3);
			formatted += "-";
			formatted += card_number.mid(3,7);
			formatted += "-";
			formatted += card_number.mid(10,2);
			break;
		case BEID_CARDTYPE_FOREIGNER:
			formatted += card_number.mid(0,1);
			formatted += " ";
			formatted += card_number.mid(1,7);
			formatted += " ";
			formatted += card_number.mid(8,2);
			break;
		case BEID_CARDTYPE_SIS:
			formatted = card_number;
			break;
		default:
			break;
		}
		return formatted;
	}
private:
	tFieldMap m_Fields;

};

//**************************************************
// Address information
// This class contains all the card specific data
//**************************************************
class AddressInfo
{
public:
#define ADDRESS_STREET		"address_street"
#define ADDRESS_ZIPCODE		"address_zipcode"
#define	ADDRESS_CITY		"address_city"	
#define ADDRESS_COUNTRY		"address_country"

	//----------------------------------------------
	// ctor
	//----------------------------------------------
	AddressInfo( void )
	{
	}

	//----------------------------------------------
	// dtor
	//----------------------------------------------
	virtual ~AddressInfo( void )
	{
	}

	//----------------------------------------------
	// Reset data to default values
	//----------------------------------------------
	void Reset( void )
	{
		m_Fields.clear();
	}

	//----------------------------------------------
	// Retrieve data from EID card
	//----------------------------------------------
	bool RetrieveData(BEID_EIDCard& Card)
	{
		bool		bRetVal = false;
		BEID_EId&	beid_eid= Card.getID();
		
		m_Fields[ADDRESS_STREET]	= QString::fromUtf8(beid_eid.getStreet());
		m_Fields[ADDRESS_ZIPCODE]	= QString::fromUtf8(beid_eid.getZipCode());
		m_Fields[ADDRESS_CITY]		= QString::fromUtf8(beid_eid.getMunicipality());
		m_Fields[ADDRESS_COUNTRY]	= QString::fromUtf8(beid_eid.getCountry());

// 		qDebug() << "AddressInfo::RetrieveData()";
// 		for (tFieldMap::iterator it=m_Fields.begin(); it!=m_Fields.end(); it++)
// 		{
// 			qDebug() << "[" << it.key() << "] = " << "Value: " << it.value();
// 		}

		return bRetVal;
	};
	//----------------------------------------------
	// get reference to all fields
	//----------------------------------------------
	tFieldMap& getFields( void )
	{
		return m_Fields;
	}

private:
	tFieldMap m_Fields;
};
//**************************************************
// Person relatives information
// This class contains all the persons relatives specific data
//**************************************************
class RelativesInfo
{
public:
//#define 
	//----------------------------------------------
	// ctor
	//----------------------------------------------
	RelativesInfo( void )
	{
	}

	//----------------------------------------------
	// dtor
	//----------------------------------------------
	virtual ~RelativesInfo( void )
	{
	}

	//----------------------------------------------
	// Reset to default values
	//----------------------------------------------
	void Reset( void )
	{
		m_Fields.clear();
	}

	//----------------------------------------------
	// retrieve data from EID card
	//----------------------------------------------
	bool RetrieveData( BEID_EIDCard& Card )
	{
		Card.getType();
		bool bRetVal = false;

		bRetVal = true;
		return bRetVal;
	}

	//----------------------------------------------
	// get reference to all fields
	//----------------------------------------------
	tFieldMap& getFields( void )
	{
		return m_Fields;
	}

private:
	tFieldMap m_Fields;
};

//**************************************************
// Person extra information
// This class contains all persons additional data
//**************************************************
class PersonExtraInfo
{
public:
#define SOCIALSECURITYNUMBER "socialsecuritynumber"
#define SPECIALSTATUS		 "special status"

	//----------------------------------------------
	// ctor
	//----------------------------------------------
	PersonExtraInfo( void )
	{
	}

	//----------------------------------------------
	// dtor
	//----------------------------------------------
	virtual ~PersonExtraInfo( void )
	{
	}

	//----------------------------------------------
	// Reset to default values
	//----------------------------------------------
	void Reset( void )
	{
		m_Fields.clear();
	}

	//----------------------------------------------
	// Retrieve data from EID card
	//----------------------------------------------
	bool RetrieveData( BEID_EIDCard& Card )
	{
		bool	  bRetVal  = false;
		BEID_EId& beid_eid = Card.getID();

		m_Fields[SPECIALSTATUS] = beid_eid.getSpecialStatus();

// 		qDebug() << "PersonExtraInfo::RetrieveData()";
// 		for (tFieldMap::iterator it=m_Fields.begin(); it!=m_Fields.end(); it++)
// 		{
// 			qDebug() << "[" << it.key() << "] = " << "Value: " << it.value();
// 		}

		return bRetVal;
	}

	//----------------------------------------------
	// Retrieve data from SIS card
	//----------------------------------------------
	bool RetrieveData( BEID_SISCard& Card )
	{
		bool bRetVal = false;
		BEID_SisId& beid_SisId = Card.getID();

		m_Fields[SOCIALSECURITYNUMBER] = QString::fromUtf8(beid_SisId.getSocialSecurityNumber());


// 		qDebug() << "PersonExtraInfo::RetrieveData()";
// 		for (tFieldMap::iterator it=m_Fields.begin(); it!=m_Fields.end(); it++)
// 		{
// 			qDebug() << "[" << it.key() << "] = " << "Value: " << it.value();
// 		}
		return bRetVal;
	}

	//----------------------------------------------
	// get reference to all fields
	//----------------------------------------------
	tFieldMap& getFields( void )
	{
		return m_Fields;
	}
private:
/*
	QString m_SocialSecurityNumber;
*/
	tFieldMap m_Fields;
};

//**************************************************
// Miscellaneous information
// This class contains all card miscellaneous data
//**************************************************
class MiscInfo
{
public:
#define DUPLICATA			"duplicata"
#define SPECIALORGANIZATION "special organization"
#define MEMBEROFFAMILY		"member of family"
	//----------------------------------------------
	// ctor
	//----------------------------------------------
	MiscInfo( void )
	{
	}

	//----------------------------------------------
	// dtor
	//----------------------------------------------
	virtual ~MiscInfo( void )
	{
	}

	//----------------------------------------------
	// Reset to default values
	//----------------------------------------------
	void Reset( void )
	{
		m_Fields.clear();
	}

	//----------------------------------------------
	// Retrieve data from EID card
	//----------------------------------------------
	bool RetrieveData( BEID_EIDCard& Card )
	{
		bool	  bRetVal  = false;
		BEID_EId& beid_eid = Card.getID();

		m_Fields[DUPLICATA]			  = beid_eid.getDuplicata();
		m_Fields[SPECIALORGANIZATION] = beid_eid.getSpecialOrganization();
		m_Fields[MEMBEROFFAMILY]	  = beid_eid.getMemberOfFamily();

		bRetVal = true;
		return bRetVal;
	}

	//----------------------------------------------
	// Retrieve data from SIS card
	//----------------------------------------------
	bool RetrieveData( BEID_SISCard& Card )
	{
		Card.getType();
		bool bRetVal = false;

		bRetVal = true;
		return bRetVal;
	}

	//----------------------------------------------
	// get reference to all fields
	//----------------------------------------------
	tFieldMap& getFields( void )
	{
		return m_Fields;
	}
private:
	tFieldMap m_Fields;
};

//**************************************************
// Person biometric information
// This class contains all biometric data
//**************************************************
class BiometricInfo
{
public:
	//----------------------------------------------
	// ctor
	//----------------------------------------------
	BiometricInfo( void )
	{
	}

	//----------------------------------------------
	// dtor
	//----------------------------------------------
	virtual ~BiometricInfo( void )
	{
	}

	//----------------------------------------------
	// Reset the biometric data
	//----------------------------------------------
	void Reset( void )
	{
		m_pPictureData.clear();
	}
	//----------------------------------------------
	// retrieve data from EID card
	//----------------------------------------------
	bool RetrieveData( BEID_EIDCard& Card	)
	{
		bool					bRetVal = false;
		BEID_Picture&			Picture = Card.getPicture();
		const unsigned char*	PictureData = Picture.getData().GetBytes();

// 		qDebug() << "BiometricInfo::RetrieveData()";
// 		qDebug() << "Retrieving picture...";
		m_pPictureData = QByteArray((char*)PictureData,Picture.getData().Size());

		bRetVal = true;
		return bRetVal;
	}

public:
	QByteArray m_pPictureData;			//!< picture on card
};
//**************************************************
// Person information
// This class contains all the person specific data
//**************************************************
class PersonInfo
{
public:
#define NAME			"name"			
#define FIRSTNAME		"firstname"
#define BIRTHPLACE		"birthplace"	
#define BIRTHDATE		"birthdate"	
#define SEX				"sex"			
#define TITLE			"title"		
#define NATIONALITY		"nationality"	
#define NATIONALNUMBER	"nationalnumber"

#define INITIALS		"initials"

	//----------------------------------------------
	// ctor
	//----------------------------------------------
	PersonInfo( void )
	{
	}

	//----------------------------------------------
	// dtor
	//----------------------------------------------
	virtual ~PersonInfo( void )
	{
	}

	//----------------------------------------------
	// reset all data to default
	//----------------------------------------------
	void Reset( void )
	{
		m_Fields.clear();
		m_AddressInfo.Reset();
		m_RelativesInfo.Reset();
		m_PersonExtraInfo.Reset();
		m_BiometricInfo.Reset();
	}

	//----------------------------------------------
	// retrieve data from EID card
	//----------------------------------------------
	bool RetrieveData(BEID_EIDCard& Card)
	{
		bool bRetVal = false;
		//m_imgPicture=QPixmap("IDFace.jpg");

		BEID_EId& beid_eid = Card.getID();

		m_Fields[NAME]			= QString::fromUtf8(beid_eid.getSurname());
		m_Fields[FIRSTNAME]		= QString::fromUtf8(beid_eid.getFirstName());
		m_Fields[NATIONALITY]	= QString::fromUtf8(beid_eid.getNationality());
		m_Fields[NATIONALNUMBER]= QString::fromUtf8(beid_eid.getNationalNumber());
		m_Fields[BIRTHPLACE]	= QString::fromUtf8(beid_eid.getLocationOfBirth());
		m_Fields[BIRTHDATE]		= QString::fromUtf8(beid_eid.getDateOfBirth());
		m_Fields[SEX]			= QString::fromUtf8(beid_eid.getGender());
		m_Fields[TITLE]			= QString::fromUtf8(beid_eid.getNobility());

// 		qDebug() << "PersonInfo::RetrieveData()";
// 		for (tFieldMap::iterator it=m_Fields.begin(); it!=m_Fields.end(); it++)
// 		{
// 			qDebug() << "[" << it.key() << "] = " << "Value: " << it.value();
// 		}

		m_AddressInfo.RetrieveData(Card);
		m_RelativesInfo.RetrieveData(Card);
		m_PersonExtraInfo.RetrieveData(Card);
		m_BiometricInfo.RetrieveData(Card);

		return bRetVal;

	}

	//----------------------------------------------
	// retrieve data from SIS card
	//----------------------------------------------
	bool RetrieveData(BEID_SISCard& Card)
	{
		BEID_SisId& beid_SisId = Card.getID();

		m_Fields[NAME]      = QString::fromUtf8(beid_SisId.getSurname()); 
		m_Fields[FIRSTNAME] = QString::fromUtf8(beid_SisId.getName()); 
		m_Fields[INITIALS]  = QString::fromUtf8(beid_SisId.getInitials()); 
		m_Fields[BIRTHDATE] = QString::fromUtf8(beid_SisId.getDateOfBirth()); 
		m_Fields[SEX]	    = QString::fromUtf8(beid_SisId.getGender()); 

// 		qDebug() << "PersonInfo::RetrieveData()";
// 		for (tFieldMap::iterator it=m_Fields.begin(); it!=m_Fields.end(); it++)
// 		{
// 			qDebug() << "[" << it.key() << "] = " << "Value: " << it.value();
// 		}

		m_PersonExtraInfo.RetrieveData(Card);

		return true;
	}

	//----------------------------------------------
	// get reference to all fields
	//----------------------------------------------
	tFieldMap& getFields( void )
	{
		return m_Fields;
	}

	bool isMale( void )
	{
		bool bRetVal = false;

		QString sex = m_Fields[SEX];
		if ( sex != "" )
		{
			if (sex == "1")		bRetVal=true;
			if (sex == "M")		bRetVal=true;
			if (sex == "Male")	bRetVal=true;
		}
		return bRetVal;
	}

	bool isFemale( void )
	{
		bool bRetVal = false;

		QString sex = m_Fields[SEX];
		if ( sex != "" )
		{
			if (sex == "2")	bRetVal=true;
			if (sex == "F")	bRetVal=true;
			if (sex == "V")	bRetVal=true;
			if (sex == "W")	bRetVal=true;
		}
		return bRetVal;
	}
	static QString formatNationalNumber( QString const& number, BEID_CardType type /*, CardInfo::eCARD_SUBTYPE subType*/)
	{
		QString formatted="";

		if (number.length()==0)
		{
			return formatted;
		}

		switch ( type )
		{
		case BEID_CARDTYPE_EID:
		case BEID_CARDTYPE_KIDS:
		case BEID_CARDTYPE_FOREIGNER:
			formatted += number.mid(0,2);
			formatted += ".";
			formatted += number.mid(2,2);
			formatted += ".";
			formatted += number.mid(4,2);
			formatted += "-";
			formatted += number.mid(6,3);
			formatted += ".";
			formatted += number.mid(9,2);
			break;
			break;
		case BEID_CARDTYPE_SIS:
			formatted = number;
			break;
		default:
			break;
		}
		return formatted;
	}

public:
	AddressInfo		m_AddressInfo;			//!< address info
	RelativesInfo	m_RelativesInfo;		//!< person relatives info
	PersonExtraInfo	m_PersonExtraInfo;		//!< person extra info
	BiometricInfo	m_BiometricInfo;		//!< person biometric data

private:
	tFieldMap m_Fields;
};
//**************************************************
// Certificate info on the card
//**************************************************
class CertifInfo
{
public:
	CertifInfo( void )
		: m_pCertificates(NULL)
	{
	}
	virtual ~CertifInfo( void )
	{
	}
	bool RetrieveData( BEID_EIDCard& card )
	{
		m_pCertificates = &card.getCertificates();
		if (m_pCertificates)
		{
			return true;
		}
		return false;
	}
	BEID_Certificates* getCertificates( void )
	{
		return m_pCertificates;
	}
private:
	BEID_Certificates* m_pCertificates;
};
//**************************************************
// Pins info on the card
//**************************************************
class PinsInfo
{
public:
	PinsInfo( void )
	{
	}
	virtual ~PinsInfo( void )
	{
	}
};
//**************************************************
// Base class containing the card information
// The data for the cards is spread over several classes to separate
// the type of data
// - CardInfo	: data of the card itself
// - PersonInfo : data about the person
// - AddressInfo: address data of the person
// - RelativesInfo: info about the persons relatives
// - PersonExtraInfo: additional data on the person
// - MiscInfo	: miscellaneous info
//**************************************************
class CardInformation
{
public:
	//----------------------------------------------
	// ctor
	//----------------------------------------------
	CardInformation( void );

	//----------------------------------------------
	// dtor
	//----------------------------------------------
	virtual ~CardInformation( void );

	//----------------------------------------------
	// Reset to default values
	//----------------------------------------------
	void Reset( void );

	//----------------------------------------------
	// Load the data from an EID card
	//----------------------------------------------
	bool LoadData(BEID_EIDCard&	   Card, QString const& cardReader );

	//----------------------------------------------
	// Load the data from a memory card
	//----------------------------------------------
	bool LoadData(BEID_MemoryCard& Card, QString const& cardReader);

	bool isDataLoaded() {return (m_pCard!=NULL);} //return m_bDataLoaded;}

private:
	//----------------------------------------------
	// retrieve data from EID card
	//----------------------------------------------
	bool RetrieveData( BEID_EIDCard& Card )
	{
		bool bRetVal = false;
		try
		{
			bRetVal |= m_CardInfo.RetrieveData(Card);
			bRetVal |= m_PersonInfo.RetrieveData(Card);
			bRetVal |= m_MiscInfo.RetrieveData(Card);
		}
		catch (BEID_ExNotAllowByUser& e)
		{
			e = e;
			// ok fine, we don't read the personal information if not allowed
		}

//		bRetVal |= m_PinsInfo.RetrieveData(pCard);
		bRetVal |= m_CertifInfo.RetrieveData(Card);
		m_pCard = &Card;

		return bRetVal;

	}

	//----------------------------------------------
	// retrieve data from SIS card
	//----------------------------------------------
	bool RetrieveData( BEID_SISCard& Card )
	{
		bool bRetVal = false;

		if(!m_CardInfo.RetrieveData(Card))
		{
			return false;
		}
		if (!m_PersonInfo.RetrieveData(Card))
		{
			return false;
		}
		if (!m_MiscInfo.RetrieveData(Card))
		{
			return false;
		}
		
		m_pCard = &Card;
		bRetVal = true;

		return bRetVal;
	}

public:
	CardInfo		m_CardInfo;				//!< card specific data
	PersonInfo		m_PersonInfo;			//!< person info
	MiscInfo		m_MiscInfo;				//!< miscellaneous info

	PinsInfo		m_PinsInfo;				//!< info on all the pins
	CertifInfo		m_CertifInfo;			//!< info on the certificates

	BEID_Card*		m_pCard;				//!< pointer to the card this data belongs to
	//std::string		m_cardReader;			//!< cardreader from which we read the data
	QString		m_cardReader;			//!< cardreader from which we read the data
};

#endif
