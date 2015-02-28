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
 *   $Revision: 583 $                                                         *
 *   $Date: 2012-05-08 17:05:10 +0200 (Di, 08 Mai 2012) $                   *
 *                                                                           *
\*===========================================================================*/


//== INCLUDES =================================================================


// STL
#include <map>

#include <float.h>
#include <fstream>

// OpenMesh
#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/IO/reader/STLReader.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/IO/importer/BaseImporter.hh>


//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=== INSTANCIATE =============================================================


// register the STLReader singleton with MeshReader
_STLReader_  __STLReaderInstance;
_STLReader_&  STLReader() { return __STLReaderInstance; }


//=== IMPLEMENTATION ==========================================================


_STLReader_::
_STLReader_()
  : eps_(FLT_MIN)
{
  IOManager().register_module(this);
}


//-----------------------------------------------------------------------------


bool
_STLReader_::
read(const std::string& _filename, BaseImporter& _bi, Options& _opt)
{
  bool result = false;

  STL_Type file_type = NONE;

  if ( check_extension( _filename, "stla" ) )
  {
    file_type = STLA;
  }

  else if ( check_extension( _filename, "stlb" ) )
  {
    file_type = STLB;
  }

  else if ( check_extension( _filename, "stl" ) )
  {
    file_type = check_stl_type(_filename);
  }


  switch (file_type)
  {
    case STLA:
    {
      result = read_stla(_filename, _bi);
      _opt -= Options::Binary;
      break;
    }

    case STLB:
    {
      result = read_stlb(_filename, _bi);
      _opt += Options::Binary;
      break;
    }

    default:
    {
      result = false;
      break;
    }
  }


  return result;
}

bool
_STLReader_::read(std::istream& _is,
		 BaseImporter& _bi,
		 Options& _opt)
{

    omerr() << "[OMReader] : STL Streams are not supported " << std::endl;

  return false;
}


//-----------------------------------------------------------------------------


#ifndef DOXY_IGNORE_THIS

class CmpVec
{
public:

  CmpVec(float _eps=FLT_MIN) : eps_(_eps) {}

  bool operator()( const Vec3f& _v0, const Vec3f& _v1 ) const
  {
    if (fabs(_v0[0] - _v1[0]) <= eps_)
    {
      if (fabs(_v0[1] - _v1[1]) <= eps_)
      {
	return (_v0[2] < _v1[2] - eps_);
      }
      else return (_v0[1] < _v1[1] - eps_);
    }
    else return (_v0[0] < _v1[0] - eps_);
  }

private:
  float eps_;
};

#endif


//-----------------------------------------------------------------------------

void trimStdString( std::string& _string) {
  // Trim Both leading and trailing spaces

  size_t start = _string.find_first_not_of(" \t\r\n");
  size_t end   = _string.find_last_not_of(" \t\r\n");

  if(( std::string::npos == start ) || ( std::string::npos == end))
    _string = "";
  else
    _string = _string.substr( start, end-start+1 );
}

//-----------------------------------------------------------------------------

bool
_STLReader_::
read_stla(const std::string& _filename, BaseImporter& _bi) const
{
  omlog() << "[STLReader] : read ascii file\n";

  std::fstream in( _filename.c_str(), std::ios_base::in );

  if (!in)
  {
    omerr() << "[STLReader] : cannot not open file "
	  << _filename
	  << std::endl;
    return false;
  }


  unsigned int               i;
  OpenMesh::Vec3f            v;
  OpenMesh::Vec3f            n;
  unsigned int               cur_idx(0);
  BaseImporter::VHandles     vhandles;

  CmpVec comp(eps_);
  std::map<Vec3f, VertexHandle, CmpVec>            vMap(comp);
  std::map<Vec3f, VertexHandle, CmpVec>::iterator  vMapIt;

  std::string line;

  bool normal = false;

  while( in && !in.eof() ) {

    // Get one line
    std::getline(in,line);
    if ( in.bad() ){
      omerr() << "  Warning! Could not read file properly!\n";
      in.close();
      return false;
    }

    // Trim Both leading and trailing spaces
    trimStdString(line);

    // Normal found?
    if (line.find("facet normal") != std::string::npos) {
      std::stringstream strstream(line);

      std::string garbage;

      // facet
      strstream >> garbage;

      // normal
      strstream >> garbage;

      strstream >> n[0];
      strstream >> n[1];
      strstream >> n[2];

      normal = true;
    }

    // Detected a triangle
    if ( (line.find("outer") != std::string::npos) ||  (line.find("OUTER") != std::string::npos ) ) {

      vhandles.clear();

      for (i=0; i<3; ++i) {
        // Get one vertex
        std::getline(in,line);
        trimStdString(line);

        std::stringstream strstream(line);

        std::string garbage;
        strstream >> garbage;

        strstream >> v[0];
        strstream >> v[1];
        strstream >> v[2];

        // has vector been referenced before?
        if ((vMapIt=vMap.find(v)) == vMap.end())
        {
          // No : add vertex and remember idx/vector mapping
          _bi.add_vertex(v);
          vhandles.push_back(VertexHandle(cur_idx));
          vMap[v] = VertexHandle(cur_idx++);
        }
        else
          // Yes : get index from map
          vhandles.push_back(vMapIt->second);

      }

      // Add face only if it is not degenerated
      if ((vhandles[0] != vhandles[1]) &&
          (vhandles[0] != vhandles[2]) &&
          (vhandles[1] != vhandles[2])) {


        FaceHandle fh = _bi.add_face(vhandles);

      }

      normal = false;
    }
  }

  if (in)
    in.close();

  return true;
}


//-----------------------------------------------------------------------------


bool
_STLReader_::
read_stlb(const std::string& _filename, BaseImporter& _bi) const
{
  omlog() << "[STLReader] : read binary file\n";

  FILE*  in = fopen(_filename.c_str(), "rb");
  if (!in)
  {
    omerr() << "[STLReader] : cannot not open file "
	  << _filename
	  << std::endl;
    return false;
  }


  char                       dummy[100];
  bool                       swapFlag;
  unsigned int               i, nT;
  OpenMesh::Vec3f            v;
  unsigned int               cur_idx(0);
  BaseImporter::VHandles     vhandles;

  std::map<Vec3f, VertexHandle, CmpVec>  vMap;
  std::map<Vec3f, VertexHandle, CmpVec>::iterator vMapIt;


  // check size of types
  if ((sizeof(float) != 4) || (sizeof(int) != 4)) {
    omerr() << "[STLReader] : wrong type size\n";
    return false;
  }

  // determine endian mode
  union { unsigned int i; unsigned char c[4]; } endian_test;
  endian_test.i = 1;
  swapFlag = (endian_test.c[3] == 1);

  // read number of triangles
  fread(dummy, 1, 80, in);
  nT = read_int(in, swapFlag);

  // read triangles
  while (nT)
  {
    vhandles.clear();

    // skip triangle normal
    fread(dummy, 1, 12, in);

    // triangle's vertices
    for (i=0; i<3; ++i)
    {
      v[0] = read_float(in, swapFlag);
      v[1] = read_float(in, swapFlag);
      v[2] = read_float(in, swapFlag);

      // has vector been referenced before?
      if ((vMapIt=vMap.find(v)) == vMap.end())
      {
	// No : add vertex and remember idx/vector mapping
	_bi.add_vertex(v);
	vhandles.push_back(VertexHandle(cur_idx));
	vMap[v] = VertexHandle(cur_idx++);
      }
      else
	// Yes : get index from map
	vhandles.push_back(vMapIt->second);
    }


    // Add face only if it is not degenerated
    if ((vhandles[0] != vhandles[1]) &&
	(vhandles[0] != vhandles[2]) &&
	(vhandles[1] != vhandles[2]))
      _bi.add_face(vhandles);

    fread(dummy, 1, 2, in);
    --nT;
  }

  fclose(in);

  return true;
}


//-----------------------------------------------------------------------------


_STLReader_::STL_Type
_STLReader_::
check_stl_type(const std::string& _filename) const
{
  // assume it's binary stl, then file size is known from #triangles
  // if size matches, it's really binary


  // open file
  FILE* in = fopen(_filename.c_str(), "rb");
  if (!in) return NONE;


  // determine endian mode
  union { unsigned int i; unsigned char c[4]; } endian_test;
  endian_test.i = 1;
  bool swapFlag = (endian_test.c[3] == 1);


  // read number of triangles
  char dummy[100];
  fread(dummy, 1, 80, in);
  size_t nT = read_int(in, swapFlag);


  // compute file size from nT
  size_t binary_size = 84 + nT*50;


  // get actual file size
  size_t file_size(0);
  rewind(in);
  while (!feof(in))
    file_size += fread(dummy, 1, 100, in);
  fclose(in);


  // if sizes match -> it's STLB
  return (binary_size == file_size ? STLB : STLA);
}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
