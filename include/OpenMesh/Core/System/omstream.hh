/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen      *
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
 *   $Revision: 566 $                                                         *
 *   $Date: 2012-03-23 18:00:57 +0100 (Fr, 23 Mär 2012) $                   *
 *                                                                           *
\*===========================================================================*/

//=============================================================================
//
//  OpenMesh streams: omlog, omout, omerr
//
//=============================================================================

#ifndef OPENMESH_OMSTREAMS_HH
#define OPENMESH_OMSTREAMS_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/mostream.hh>


//== CLASS DEFINITION =========================================================

/** \file omstream.hh
    This file provides the streams omlog, omout, and omerr.
*/

/** \name stream replacements
    These stream provide replacements for clog, cout, and cerr. They have
    the advantage that they can easily be multiplexed.
    \see OpenMesh::mostream
*/
//@{
OpenMesh::mostream& omlog();
OpenMesh::mostream& omout();
OpenMesh::mostream& omerr();
//@}

//=============================================================================
#endif // OPENMESH_OMSTREAMS_HH defined
//=============================================================================