# ***************************************************************************
# Copyright (c) 2013 Digi International Inc.,
# All rights not expressly granted are reserved.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
# ***************************************************************************
#
# S.A.R.E.S Framework - collections deque for SARES Object class

import collections      # Implements specialized container datatypes providing alternatives to dict,list,set,tuple
import re               # This module provides regular expression matching operations
import sre_constants    # Module to catch exceptions in re searchs ('For strings that contain ?, * or similar chars - normally for uboot chains')


class deque(collections.deque):


    ## Function to find the first occurrence on left of the pattern in the deque object
    #   @param self reference for this object
    #   @param pattern string containing the pattern to find
    #   @return index if the pattern was found, otherwise -1
    def find(self, pattern, flags=re.IGNORECASE):
        return self._findElem(pattern, flags)


    ## Function to find the first occurrence on right of the pattern in the deque object
    #   @param self reference for this object
    #   @param pattern string containing the pattern to find
    #   @return index if the pattern was found, otherwise -1
    def rfind(self, pattern, flags=re.IGNORECASE):
        return self._findElem(pattern, flags, reverse = True)


    ## Function to find all occurrences of the pattern in the deque object
    #   @param self reference for this object
    #   @param pattern string containing the pattern to find
    #   @return array containing the index of the occurrence
    def findAll(self, pattern, flags=re.IGNORECASE):
        return self._findElem(pattern, flags, findAll = True)




    def _findElem(self, pattern, flags, reverse = False, findAll = False):
        # Sanity check
        if not (isinstance(pattern,basestring)):
            # We raise the condition here for better traceability.
            raise Exception('Pattern %s %s is not a valid basestring' % (repr(pattern), type(pattern)))


        if (flags is None):
            # Set default value for flags
            flags = 0

        # Initialize index list matching with the pattern
        matchingList = []

        #* Get the index list for this deque object
        indexList = range(len(self))
        # reverse the index list to start from the end to the beginning
        if (reverse):
            indexList.reverse()

        #* Search the pattern, following the index list
        for eachIndex in indexList:
            # Get each line from the deque object
            line = self[eachIndex]

            # Looking for the pattern in the line
            if (self._isPatternInChain(pattern, line, flags)):

                if (findAll):
                    # In 'findAll' mode append each coincidence to the list
                    matchingList.append(eachIndex)
                else:
                    # In 'find' and 'rfind' return the first coincidence
                    return eachIndex

        #* Return the result of the search
        if (findAll):
            # In 'findAll' mode, return always a list, empty or with the coincidences
            return matchingList
        else:
            # In 'find' and 'rfind', return -1 if there is not a coincidence
            return -1




    def _isPatternInChain(self, pattern, line, flags):

        #** Search the pattern as regular expression
        try:
            if (re.search(pattern, line, flags=flags)):
                return True
        except (sre_constants.error):
            # Pattern is not a valid regular expression
            pass


        #** Search the pattern as literal
        if (flags == re.IGNORECASE):
            # Homogenize the pattern and line
            pattern = pattern.upper()
            line = line.upper()

        # Return True if pattern was found, otherwise False
        if (pattern in line):
            return True
        return False
