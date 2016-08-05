//  $Id: quad_set.cpp 5674 2010-07-08 23:03:45Z hikerstk $
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "tracks/quad_set.hpp"

#include <stdlib.h>

#include "io/file_manager.hpp"
#include "io/xml_node.hpp"
#include "utils/string_utils.hpp"

/** Constructor, loads the quad set from a file.
 *  \param filename filename to load.
 */
QuadSet::QuadSet(const std::string& filename) {
    load(filename);
}   // QuadSet

// -----------------------------------------------------------------------------
/** Destructor, frees all memory.
 */
QuadSet::~QuadSet()
{
    for(unsigned int i=0; i<m_all_quads.size(); i++)
    {
        delete m_all_quads[i];
    }
    m_all_quads.clear();
}   // ~QuadSet

// -----------------------------------------------------------------------------
/** This function interprets a point specification as an attribute in the 
    xml quadset file. It understands two different specifications:
    p1="n:p"      : get point p from square n (n, p integers)
    p1="p1,p2,p3" : make a 3d point out of these 3 floating point values
*/
void QuadSet::getPoint(const XMLNode *xml, const std::string &attribute_name, 
                       Vec3* result) const
{
    std::string s;
    xml->get(attribute_name, &s);
    int pos=s.find_first_of(":");
    if(pos>0)   // n:p specification
    {
        std::vector<std::string> l = StringUtils::split(s, ':');
        int n=atoi(l[0].c_str());
        int p=atoi(l[1].c_str());
        *result=(*m_all_quads[n])[p];
    } 
    else
    {
        xml->get(attribute_name, result);
    }

}   // getPoint 
// -----------------------------------------------------------------------------
void QuadSet::load(const std::string &filename) {
    m_min = Vec3( 99999,  99999,  99999);
    m_max = Vec3(-99999, -99999, -99999);

    XMLNode *xml = file_manager->createXMLTree(filename);
    if(!xml || xml->getName()!="quads") 
    {
        fprintf(stderr, "QuadSet '%s' not found.\n", filename.c_str());
        return;
        //FIXME exit(-1);
    }
    for(unsigned int i=0; i<xml->getNumNodes(); i++)
    {
        const XMLNode *xml_node = xml->getNode(i);
        if(xml_node->getName()!="quad")
        {
            printf("Unsupported node type '%s' found in '%s' - ignored.\n",
                xml_node->getName().c_str(), filename.c_str());
            continue;
        }
        Vec3 p0, p1, p2, p3;
        getPoint(xml_node, "p0", &p0);
        getPoint(xml_node, "p1", &p1);
        getPoint(xml_node, "p2", &p2);
        getPoint(xml_node, "p3", &p3);
        bool invisible=false;
        xml_node->get("invisible", &invisible);
        Quad* q=new Quad(p0,p1,p2,p3, invisible);
        m_all_quads.push_back(q);
        m_max.max(p0);m_max.max(p1);m_max.max(p2);m_max.max(p3);
        m_min.min(p0);m_min.min(p1);m_min.min(p2);m_min.min(p3);

    }
    delete xml;
}   // load

// -----------------------------------------------------------------------------