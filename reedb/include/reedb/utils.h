/* libreedb - utils.h
 *
 * A few utilities for the public header files of libreedb. Are exposed
 * to the developer because they might also be useful for their
 * application.
 *
 * (c) 2015-2016    			Lonely Robot.
 * Authors:						Katharina 'spacekookie' Sabel
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * -------------------------------------------
 *
 */

#ifndef REEDB_UTILS_H
#define REEDB_UTILS_H

/** Auto ACCESS, GETTER and SETTER macros to generate functions on the fly*/
#define DATA(type, var) \
private: \
    type var; \
public: \
    type get_##var() \
    {\
       return this->var; \
    }\
   void set_##var(type val) \
    {\
       this->var = val; \
    }

#define GET(type, var) \
private: \
    type var; \
public: \
    type get_##var() \
    {\
       return this->var; \
    }

#define SET(type, var) \
private: \
    type var; \
public: \
    void set_##var(type val) \
    {\
       this->var = val; \
    }



#endif //REEDB_UTILS_H
