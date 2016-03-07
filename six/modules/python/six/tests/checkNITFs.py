#!/usr/bin/env python

#
# =========================================================================
# This file is part of six.sicd-python
# =========================================================================
#
# (C) Copyright 2004 - 2016, MDA Information Systems LLC
#
# six.sicd-python is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; If not,
# see <http://www.gnu.org/licenses/>.
#

import glob
import os
import utils

from subprocess import call

binDir = os.path.join(utils.installPath(), 'bin')

def roundTrippedName(pathname):
    return os.path.basename(pathname.replace('.nitf', '_rt.nitf'))

def roundTripSix(pathname):
    return call([utils.executableName(os.path.join(binDir, 'round_trip_six')),
              pathname, roundTrippedName(pathname)]) == 0

def validate(pathname):
    check_valid_six = utils.executableName(os.path.join(binDir,
            'check_valid_six'))
    
    return (call([check_valid_six, pathname], stdout=open(os.devnull, 'w')) and
            call([check_valid_six, roundTrippedName(pathname)],
            stdout=open(os.devnull, 'w'))) == 0


def run():
    regressionDir = os.path.join(utils.findSixHome(), 'regression_files')
    result = True
    for pathname in glob.iglob(os.path.join(regressionDir, '*', '*.nitf')):
        result = result and roundTripSix(pathname) and validate(pathname)

    return result
