/* =========================================================================
 * This file is part of cphd-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2019, MDA Information Systems LLC
 *
 * cphd-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not,
 * see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __CPHD_CPHD_XML_CONTROL_H__
#define __CPHD_CPHD_XML_CONTROL_H__

#include <memory>
#include <unordered_map>
#include <logging/Logger.h>
#include <xml/lite/Element.h>
#include <xml/lite/Document.h>
#include <cphd/CPHDXMLParser.h>
#include <cphd/Types.h>

namespace cphd
{
/*!
 *  \class CPHDXMLControl
 *
 *  \brief This class converts a Metadata object into a CPHD XML
 *  Document Object Model (DOM) and vice-versa.
 */
class CPHDXMLControl
{
public:
    /*!
     *  \func CPHDXMLControl
     *  \brief Default constructor
     *
     *  \param log provide logger object
     *  \param ownLog flag indicates if log should be deleted
     */
    CPHDXMLControl(logging::Logger* log = nullptr, bool ownLog = false);

    //! Destructor
    virtual ~CPHDXMLControl();

    /*
     *  \func setLogger
     *  \brief Handles setting logger based on log provided and ownLog flag
     *
     *  \param log provide logger object
     *  \param ownLog flag indicates if log should be deleted
     */
    void setLogger(logging::Logger* log, bool ownLog = false);

    /*!
     *  \func toXMLString
     *
     *  \brief Convert metadata to XML string
     *  Calls toXML
     *  \return XML String
     */
    virtual std::string toXMLString(
            const Metadata& metadata,
            const std::vector<std::string>& schemaPaths = std::vector<std::string>(),
            bool prettyPrint = false);

    /*!
     *  \func toXML
     *
     *  \brief Convert metadata to XML document object
     *
     *  \param metadata Valid CPHD metadata object
     *  \param schemaPaths Vector of XML Schema for validation
     *  \return pointer to xml Document object
     */
    virtual std::auto_ptr<xml::lite::Document> toXML(
            const Metadata& metadata,
            const std::vector<std::string>& schemaPaths = std::vector<std::string>());
    virtual std::unique_ptr<xml::lite::Document> toXML(std::nullptr_t,
            const Metadata& metadata,
            const std::vector<std::string>& schemaPaths = std::vector<std::string>());

    /*!
     *  \func fromXML
     *
     *  \brief Parse XML string to Metadata object
     *
     *  \param xmlString Valid cphd XML string
     *  \param schemaPaths Vector of XML Schema for validation
     *
     *  \return pointer to metadata object
     */
    virtual std::auto_ptr<Metadata> fromXML(
            const std::string& xmlString,
            const std::vector<std::string>& schemaPaths = std::vector<std::string>());
    virtual std::unique_ptr<Metadata> fromXML(std::nullptr_t,
            const std::string& xmlString,
            const std::vector<std::string>& schemaPaths = std::vector<std::string>());

    /*!
     *  \func fromXML
     *
     *  \brief Parse XML document to Metadata object
     *
     *  \param doc XML document object of CPHD
     *  \param schemaPaths Vector of XML Schema for validation
     *
     *  \return pointer to metadata object
     */
    virtual std::auto_ptr<Metadata> fromXML(
            const xml::lite::Document* doc,
            const std::vector<std::string>& schemaPaths = std::vector<std::string>());
    virtual std::unique_ptr<Metadata> fromXML(std::nullptr_t,
            const xml::lite::Document* doc,
            const std::vector<std::string>& schemaPaths = std::vector<std::string>());

protected:
    logging::Logger *mLog;
    bool mOwnLog;

private:
    //! \return Hardcoded version to uri mapping
    static std::unordered_map<std::string, std::string> getVersionUriMap();

    /*!
     *  This function takes in a Metadata object and converts
     *  it to a new-allocated XML DOM.
     *
     *  \param data A Metadata object
     *  \return An XML DOM
     */
    virtual std::auto_ptr<xml::lite::Document> toXMLImpl(const Metadata& metadata);
    virtual std::unique_ptr<xml::lite::Document> toXMLImpl(std::nullptr_t, const Metadata& metadata);

    /*!
     *  Function takes a DOM Document* node and creates a new-allocated
     *  ComplexData* populated by the DOM.
     *
     *  \param doc An XML document pointer
     *  \return A Metadata object
     */
    virtual std::auto_ptr<Metadata> fromXMLImpl(const xml::lite::Document* doc);
    virtual std::unique_ptr<Metadata> fromXMLImpl(std::nullptr_t, const xml::lite::Document* doc);

    /*
     *  \Function creates a new parser to parse XML or document
     *
     *  \param uri A string specifying CPHD uri
     */
    std::auto_ptr<CPHDXMLParser> getParser(const std::string& uri) const;
    std::unique_ptr<CPHDXMLParser> getParser(std::nullptr_t, const std::string& uri) const;

    // Given the URI get associated version
    std::string uriToVersion(const std::string& uri) const;
};
}

#endif
