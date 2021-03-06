/* =========================================================================
 * This file is part of six.sidd-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * six.sidd-c++ is free software; you can redistribute it and/or modify
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

#include <six/sidd/DerivedXMLControl.h>
#include <six/sidd/DerivedData.h>
#include <six/sidd/DerivedXMLParser100.h>
#include <six/sidd/DerivedXMLParser200.h>

namespace
{
std::string normalizeVersion(const std::string& version)
{
    std::vector<std::string> versionParts;
    six::XMLControl::splitVersion(version, versionParts);
    if (versionParts.size() != 3)
    {
        throw except::Exception(
            Ctxt("Unsupported SIDD Version: " + version));
    }
    return str::join(versionParts, "");
}
}

namespace six
{
namespace sidd
{
DerivedXMLControl::DerivedXMLControl(logging::Logger* log, bool ownLog) :
    XMLControl(log, ownLog)
{
}

Data* DerivedXMLControl::fromXMLImpl(const xml::lite::Document* doc)
{
    return getParser(getVersionFromURI(doc))->fromXML(doc);
}

xml::lite::Document* DerivedXMLControl::toXMLImpl(const Data* data)
{
    if (data->getDataType() != DataType::DERIVED)
    {
        throw except::Exception(Ctxt("Data must be SIDD"));
    }

    const DerivedData* const sidd(reinterpret_cast<const DerivedData*>(data));
    return getParser(data->getVersion())->toXML(sidd);
}

std::auto_ptr<DerivedXMLParser>
DerivedXMLControl::getParser(const std::string& version) const
{
    std::auto_ptr<DerivedXMLParser> parser;

    const std::string normalizedVersion = normalizeVersion(version);

    // six.sidd only currently supports --
    //   SIDD 1.0.0
    //   SIDD 2.0.0
    if (normalizedVersion == "100")
    {
        parser.reset(new DerivedXMLParser100(mLog));
    }
    else if (normalizedVersion == "200")
    {
        parser.reset(new DerivedXMLParser200(mLog));
    }
    else if (normalizedVersion == "110")
    {
        throw except::Exception(Ctxt(
            "SIDD Version 1.1.0 does not exist. "
            "Did you mean 2.0.0 instead?"
        ));
    }
    else
    {
        throw except::Exception(
            Ctxt("Unsupported SIDD Version: " + version));
    }

    return parser;
}
}
}
