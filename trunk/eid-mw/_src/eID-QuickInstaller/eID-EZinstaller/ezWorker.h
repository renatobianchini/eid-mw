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
#ifndef __EZWORKER_INCL__
#define __EZWORKER_INCL__

#pragma once
#include <QDomDocument>
#include <QDomNode>
#include <QProcess>


#include "Platform.h"
#ifdef WIN32
#else
#include <errno.h>
#define ERROR_FILE_NOT_FOUND ENOENT
#define ERROR_ACCESS_DENIED EPERM
#endif

#include "wildcards.h"

#include <string>
#include <sstream>
#include <vector>

using namespace std;

class ezWorker
{

public:
	ezWorker(void);
	virtual ~ezWorker(void);
	


	QDomDocument xml_in;
	QDomDocument xml_out;

    string ezWorker::TextFromNode(QDomNode theNode);
	string ezWorker::NodeAsString(QDomNode theNode);
	string ezWorker::inttostr(const int getal);
	bool ezWorker::WildCardFit(string searchprofile, string stringToCheck);
	string ezWorker::GetExtraInfoItem(string theXml, string attrib);
	string ezWorker::AttribTextFromNode(QDomNode theNode, string attrib);
	QDomNode ezWorker::CreateTextNode(const string tag, const string val);
    QDomNode ezWorker::CreateAttribTextNode(const string tag, const string val, const string AttribName, const string Attribval);
    QDomNode ezWorker::CreateInfoTextNode(const string tag, const string val);
	string ezWorker::GetNamedItem(QDomNode theNode, string tag);
	string ezWorker::GetNamedItem(const string theXml, string tag);
	QDomNode ezWorker::xmlToNode(const string theXml);
	string ezWorker::GetLastErrorText(DWORD errorCode);
    std::string ezWorker::toHEX(std::vector<BYTE>& b);
};

#endif __EZWORKER_INCL__
