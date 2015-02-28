/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2009 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *  This file is part of OpenMesh.                                           *
 *                                                                           *
 *  OpenMesh is free software: you can redistribute it and/or modify         * 
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenMesh is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenMesh.  If not,                                    *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/ 

/*===========================================================================*\
 *                                                                           *             
 *   $Revision: 213 $                                                         *
 *   $Date: 2009-10-16 14:46:28 +0200 (Fr, 16 Okt 2009) $                   *
 *                                                                           *
\*===========================================================================*/

/** \file config.h
 *  \todo Move content to config.hh and include it to be compatible with old
 *  source.
 */

//=============================================================================

#ifndef OPENMESH_CONFIG_H
#define OPENMESH_CONFIG_H

//=============================================================================

#include <assert.h>
#include <OpenMesh/Core/System/compiler.hh>

// ----------------------------------------------------------------------------

#define OM_VERSION 0x10000

// only defined, if it is a beta version
#define OM_VERSION_BETA 4

#define OM_GET_VER ((OM_VERSION && 0xf0000) >> 16)
#define OM_GET_MAJ ((OM_VERSION && 0x0ff00) >> 8)
#define OM_GET_MIN  (OM_VERSION && 0x000ff)

#ifdef WIN32
#  ifdef min
#    pragma message("Detected min macro! OpenMesh does not compiled with min/max macros active! Please add a define NOMINMAX to your compiler flags !")
#    error min macro active 
#  endif
#  ifdef max
#    pragma message("Detected max macro! OpenMesh does not compiled with min/max macros active! Please add a define NOMINMAX to your compiler flags !")
#    error max macro active 
#  endif
#endif

typedef unsigned int uint;
//=============================================================================
#endif // OPENMESH_CONFIG_H defined
//=============================================================================
