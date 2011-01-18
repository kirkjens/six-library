/* =========================================================================
 * This file is part of NITRO
 * =========================================================================
 *
 * (C) Copyright 2004 - 2010, General Dynamics - Advanced Information Systems
 *
 * NITRO is free software; you can redistribute it and/or modify
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
 * License along with this program; if not,
 * see <http://www.gnu.org/licenses/>.
 *
 */

#include "nitf/Utils.h"

NITFAPI(void) nitf_Utils_decimalToGeographic(double decimal,
                                             int *degrees,
                                             int *minutes,
                                             double *seconds)
{
    double remainder;
    *degrees = (int)decimal;
    remainder = fabs( decimal - (double)*degrees ) * 60.0;
    *minutes = (int)remainder;
    *seconds = fabs(remainder - (double)*minutes) * 60.0;

}

NITFAPI(double) nitf_Utils_geographicToDecimal(int degrees,
                                               int minutes,
                                               double seconds)
{
    double decimal = fabs(degrees);
    decimal += ((double)minutes/60.0);
    decimal += (seconds/3600.0);

    if (degrees < 0)
    {
        decimal *= -1;
    }

    return decimal;
}

NITFAPI(NITF_BOOL) nitf_Utils_parseGeographicString(char* dms,
                                                    int* degrees,
                                                    int* minutes,
                                                    double* seconds,
                                                    nitf_Error* error)
{
    int degreeOffset = 0;
    int len = strlen(dms);
    char dir;

    char d[4];
    char m[3];
    char s[3];

    /* ddmmssX or dddmmssY */
    if (len == 7)
    {
        degreeOffset = 2;
    }
    else if (len == 8)
    {
        degreeOffset = 3;
    }
    else
    {
        nitf_Error_initf(error, NITF_CTXT, NITF_ERR_INVALID_PARAMETER,
                         "Invalid decimal string: %s. Should be ddmmssX or dddmmssY",
                         dms);
        return NITF_FAILURE;
    }
    dir = dms[len - 1];
    if (dir != 'N' && dir != 'S' && dir != 'E' && dir != 'W')
    {
        nitf_Error_initf(error, NITF_CTXT, NITF_ERR_INVALID_PARAMETER,
                         "Invalid direction: %s. Should be [NSEW]",
                         dms);
        return NITF_FAILURE;
    }

    /* Now replace all spaces */
    nitf_Utils_replace(dms, ' ', '0');


    /* Now get the corners out as geographic coords */
    d[degreeOffset] = 0;
    memcpy(d, dms, degreeOffset);

    memcpy(m, &dms[degreeOffset], 2);
    m[2] = 0;

    memcpy(s, &dms[degreeOffset+2], 2);
    s[2] = 0;

    *degrees = NITF_ATO32(d);
    *minutes = NITF_ATO32(m);
    *seconds = (double)NITF_ATO32(s);

    if ((degreeOffset == 2 && dir == 'S') ||
        (degreeOffset == 3 && dir == 'W'))
    {
        *degrees *= -1;
    }

    return NITF_SUCCESS;
}

NITFAPI(char) nitf_Utils_cornersTypeAsCoordRep(nitf_CornersType type)
{
    char cornerRep = ' ';

    switch (type)
    {
    case NITF_CORNERS_UTM:
        cornerRep = 'U';
        break;

    case NITF_CORNERS_UTM_UPS_S:
        cornerRep = 'S';
        break;

    case NITF_CORNERS_UTM_UPS_N:
        cornerRep = 'N';
        break;

    case NITF_CORNERS_GEO:
        cornerRep = 'G';
        break;

    case NITF_CORNERS_DECIMAL:
        cornerRep = 'D';
        break;
    default:
        break;
    }
    return cornerRep;
}

NITFPROT(void) nitf_Utils_geographicLatToCharArray(int degrees,
                                                   int minutes,
                                                   double seconds,
                                                   char *buffer7)
{
    int wtf = 0;
    char dir = 'N';
    if (degrees < 0)
    {
        dir = 'S';
        degrees *= -1;
    }
    NITF_SNPRINTF(buffer7, 8, "%02d%02d%02d%c",
             degrees, minutes, (int)(seconds + 0.5), dir);
}

NITFPROT(void) nitf_Utils_geographicLonToCharArray(int degrees,
                                                   int minutes,
                                                   double seconds,
                                                   char *buffer8)
{
    char dir = 'E';
    int wtf = 0;
    if (degrees < 0)
    {
        dir = 'W';
        degrees *= -1;
    }
    NITF_SNPRINTF(buffer8, 9, "%03d%02d%02d%c",
                  degrees, minutes, (int)(seconds + 0.5), dir);

}

NITFPROT(void) nitf_Utils_decimalLatToCharArray(double decimal,
                                                char *buffer7)
{
    NITF_SNPRINTF(buffer7, 8, "%+07.3f", decimal);
}

NITFPROT(void) nitf_Utils_decimalLonToCharArray(double decimal,
                                                char *buffer8)
{
    NITF_SNPRINTF(buffer8, 9, "%+08.3f", decimal);
}

NITFPROT(void) nitf_Utils_decimalLatToGeoCharArray(double decimal,
                                                   char *buffer7)
{
    int d, m;
    double s;

    nitf_Utils_decimalToGeographic(decimal, &d, &m, &s);
    nitf_Utils_geographicLatToCharArray(d, m, s, buffer7);
}

NITFPROT(void) nitf_Utils_decimalLonToGeoCharArray(double decimal,
                                                  char *buffer8)
{

    int d, m;
    double s;

    nitf_Utils_decimalToGeographic(decimal, &d, &m, &s);
    nitf_Utils_geographicLonToCharArray(d, m, s, buffer8);
}

