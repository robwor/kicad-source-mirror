/*
* This program source code file is part of KiCad, a free EDA CAD application.
*
* Copyright (C) 2017 CERN
* @author Alejandro García Montoro <alejandro.garciamontoro@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <properties.h>

#include <wx/filename.h>
#include <memory>
#include <string>
#include <unordered_map>

#include <sch_sheet.h>
#include <sch_eagle_plugin.h>

#include <class_sch_screen.h>
#include <class_libentry.h>
#include <lib_circle.h>
#include <lib_rectangle.h>
#include <lib_polyline.h>

#include <eagle_parser.h>

using namespace std;

NODE_MAP mapChildren( wxXmlNode* aCurrentNode )
{
    // Map node_name -> node_pointer
    NODE_MAP nodesMap;

    // Loop through all children mapping them in nodesMap
    aCurrentNode = aCurrentNode->GetChildren();
    while( aCurrentNode )
    {
        // Create a new pair in the map
        //      key: current node name
        //      value: current node pointer
        nodesMap[aCurrentNode->GetName().ToStdString()] = aCurrentNode;

        // Get next child
        aCurrentNode = aCurrentNode->GetNext();
    }

    return nodesMap;
}


void kicadLayer( int aEagleLayer )
{
    /**
     * Layers in Kicad schematics are not actually layers, but abstract groups mainly used to
     * decide item colours.
     *
     * <layers>
     *     <layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
     *     <layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
     *     <layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
     *     <layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
     *     <layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
     *     <layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
     *     <layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
     *     <layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
     *     <layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
     * </layers>
     */

    switch( aEagleLayer )
    {
        case 90: break;
        case 91: break;
        case 92: break;
        case 93: break;
        case 94: break;
        case 95: break;
        case 96: break;
        case 97: break;
        case 98: break;
    }
}


SCH_EAGLE_PLUGIN::SCH_EAGLE_PLUGIN()
{
    m_rootSheet = nullptr;
}


SCH_EAGLE_PLUGIN::~SCH_EAGLE_PLUGIN()
{
}


const wxString SCH_EAGLE_PLUGIN::GetName() const
{
    return wxT( "EAGLE" );
}


const wxString SCH_EAGLE_PLUGIN::GetFileExtension() const
{
    return wxT( "sch" );
}


int SCH_EAGLE_PLUGIN::GetModifyHash() const
{
    return 0;
}


void SCH_EAGLE_PLUGIN::SaveLibrary( const wxString& aFileName, const PROPERTIES* aProperties )
{
}


SCH_SHEET* SCH_EAGLE_PLUGIN::Load( const wxString& aFileName, KIWAY* aKiway,
                                   SCH_SHEET* aAppendToMe, const PROPERTIES* aProperties )
{
    // TODO: Handle Kiway and uncomment next line.
    // wxASSERT( !aFileName || aKiway != NULL );

    // Load the document
    wxXmlDocument xmlDocument;
    wxFileName fn = aFileName;

    if( !xmlDocument.Load( fn.GetFullPath() ) )
        THROW_IO_ERROR( wxString::Format( _( "Unable to read file '%s'" ), fn.GetFullPath() ) );

    // Delete on exception, if I own m_rootSheet, according to aAppendToMe
    unique_ptr<SCH_SHEET> deleter( aAppendToMe ? nullptr : m_rootSheet );

    if( aAppendToMe )
    {
        m_rootSheet = aAppendToMe->GetRootSheet();
    }
    else
    {
        m_rootSheet = new SCH_SHEET();
        m_rootSheet->SetFileName( aFileName );
    }

    // TODO change to loadSheet, so it can handle multiple sheets
    if( !m_rootSheet->GetScreen() )
    {
        SCH_SCREEN* screen = new SCH_SCREEN( aKiway );
        screen->SetFileName( aFileName );
        m_rootSheet->SetScreen( screen );
    }

    // Retrieve the root as current node
    wxXmlNode* currentNode = xmlDocument.GetRoot();

    // If the attribute is found, store the Eagle version;
    // otherwise, store the dummy "0.0" version.
    m_version = currentNode->GetAttribute( "version", "0.0" );

    // Map all children into a readable dictionary
    NODE_MAP children = mapChildren( currentNode );

    // TODO: handle compatibility nodes
    // wxXmlNode* compatibility = children["compatibility"];

    // Load drawing
    loadDrawing( children["drawing"] );

    deleter.release();
    return m_rootSheet;
}


void SCH_EAGLE_PLUGIN::loadDrawing( wxXmlNode* aDrawingNode )
{
    // Map all children into a readable dictionary
    NODE_MAP drawingChildren = mapChildren( aDrawingNode );

    // Board nodes should not appear in .sch files
    // wxXmlNode* board = drawingChildren["board"]

    // TODO: handle grid nodes
    // wxXmlNode* grid = drawingChildren["grid"]

    // TODO: handle layers nodes
    // wxXmlNode* layers = drawingChildren["layers"]

    // TODO: handle library nodes
    // wxXmlNode* library = drawingChildren["library"]

    // TODO: handle settings nodes
    // wxXmlNode* settings = drawingChildren["settings"]

    // Load schematic
    loadSchematic( drawingChildren["schematic"] );
}


void SCH_EAGLE_PLUGIN::loadSchematic( wxXmlNode* aSchematicNode )
{
    // Map all children into a readable dictionary
    NODE_MAP schematicChildren = mapChildren( aSchematicNode );

    // TODO : handle classes nodes
    //wxXmlNode* classes = schematicChildren["classes"];

    // TODO : handle description nodes
    // wxXmlNode* description = schematicChildren["description"];

    // TODO : handle errors nodes
    // wxXmlNode* errors = schematicChildren["errors"];

    // TODO : handle modules nodes
    // wxXmlNode* modules = schematicChildren["modules"];

    // TODO : handle parts nodes
    // wxXmlNode* parts = schematicChildren["parts"];

    // TODO : handle variantdefs nodes
    // wxXmlNode* variantdefs = schematicChildren["variantdefs"];

    // TODO: handle attributes node
    // wxXmlNode* attributes = schematicChildren["attributes"];
    // Possible children: constant, display, font, layer, name, ratio, rot, size, value, x, y

    // Loop through all the libraries
    wxXmlNode* libraryNode = schematicChildren["libraries"]->GetChildren();

    while( libraryNode )
    {
        loadLibrary( libraryNode );
        libraryNode = libraryNode->GetNext();
    }

    // Loop through all the sheets
    wxXmlNode* sheetNode = schematicChildren["sheets"]->GetChildren();

    while( sheetNode )
    {
        loadSheet( sheetNode );
        sheetNode = sheetNode->GetNext();
    }
}


void SCH_EAGLE_PLUGIN::loadSheet( wxXmlNode* aSheetNode )
{
    // Map all children into a readable dictionary
    NODE_MAP sheetChildren = mapChildren( aSheetNode );

    // Loop through all busses
    // From the DTD: "Buses receive names which determine which signals they include.
    // A bus is a drawing object. It does not create any electrical connections.
    // These are always created by means of the nets and their names."
    wxXmlNode* busNode = getChildrenNodes( sheetChildren, "busses" );

    while( busNode )
    {
        // Get the bus name
        wxString busName = busNode->GetAttribute( "name" );

        // Load segments of this bus
        loadSegments( busNode );

        // Get next bus
        busNode = busNode->GetNext();
    }

    // Loop through all nets
    // From the DTD: "Net is an electrical connection in a schematic."
    wxXmlNode* netNode = getChildrenNodes( sheetChildren, "nets" );

    while( netNode )
    {
        // Get the net name and class
        wxString netName = netNode->GetAttribute( "name" );
        wxString netClass = netNode->GetAttribute( "class" );

        // Load segments of this net
        loadSegments( netNode );

        // Get next net
        netNode = netNode->GetNext();
    }

    // Loop through all instances
    wxXmlNode* instanceNode = getChildrenNodes( sheetChildren, "instances" );

    while( instanceNode )
    {
        loadInstance( instanceNode );
        instanceNode = instanceNode->GetNext();
    }

    // Loop through all moduleinsts
    wxXmlNode* moduleinstNode = getChildrenNodes( sheetChildren, "moduleinsts" );

    while( moduleinstNode )
    {
        loadModuleinst( moduleinstNode );
        moduleinstNode = moduleinstNode->GetNext();
    }

    // TODO: do something with the description
    // wxXmlNode* description = sheetChildren["description"];
    // wxString language = description->GetAttribute( "language", "en" ); // Defaults to "en"
    // wxString description = description->GetNodeContent();

    // TODO: do something with the plain
    wxXmlNode* plainNode = sheetChildren["plain"];

    while( plainNode )
    {
        loadSegments( plainNode );
        plainNode = plainNode->GetNext();
    }
}


void SCH_EAGLE_PLUGIN::loadSegments( wxXmlNode* aSegmentsNode )
{
    // Loop through all segments
    wxXmlNode* currentSegment = aSegmentsNode->GetChildren();
    SCH_SCREEN* screen = m_rootSheet->GetScreen();
    //wxCHECK( screen, [>void<] );

    while( currentSegment )
    {
        // Loop through all segment children
        wxXmlNode* segmentAttribute = currentSegment->GetChildren();

        while( segmentAttribute )
        {
            wxString nodeName = segmentAttribute->GetName();

            if( nodeName == "junction" )
            {
                // TODO: handle junctions attributes
                segmentAttribute->GetAttribute( "x" );
                segmentAttribute->GetAttribute( "y" );
            }
            else if( nodeName == "label" )
            {
                // TODO: handle labels attributes
                segmentAttribute->GetAttribute( "x" );      // REQUIRED
                segmentAttribute->GetAttribute( "y" );      // REQUIRED
                segmentAttribute->GetAttribute( "size" );   // REQUIRED
                segmentAttribute->GetAttribute( "layer" );  // REQUIRED
                segmentAttribute->GetAttribute( "font" );   // Defaults to "proportional"
                segmentAttribute->GetAttribute( "ratio" );  // Defaults to "8"
                segmentAttribute->GetAttribute( "rot" );    // Defaults to "R0"
                segmentAttribute->GetAttribute( "xref" );   // Defaults to "no"
            }
            else if( nodeName == "pinref" )
            {
                // TODO: handle pinref attributes
                segmentAttribute->GetAttribute( "gate" );   // REQUIRED
                segmentAttribute->GetAttribute( "part" );   // REQUIRED
                segmentAttribute->GetAttribute( "pin" );    // REQUIRED
            }
            else if( nodeName == "portref" )
            {
                // TODO: handle portref attributes
                segmentAttribute->GetAttribute( "moduleinst" ); // REQUIRED
                segmentAttribute->GetAttribute( "port" );       // REQUIRED
            }
            else if( nodeName == "wire" )
            {
                screen->Append( loadWire( segmentAttribute ) );
            }
            else if( nodeName == "segment" )
            {
                loadSegments( segmentAttribute );
            }
            else if( nodeName == "text" )
            {
                // TODO
                //loadSegments( segmentAttribute );
            }
            else // DEFAULT
            {
                // TODO uncomment
                //THROW_IO_ERROR( wxString::Format( _( "XML node '%s' unknown" ), nodeName ) );
            }

            // Get next segment attribute
            segmentAttribute = segmentAttribute->GetNext();
        }

        currentSegment = currentSegment->GetNext();
    }
}


SCH_LINE* SCH_EAGLE_PLUGIN::loadWire( wxXmlNode* aWireNode )
{
    std::unique_ptr<SCH_LINE> wire( new SCH_LINE );

    auto ewire = EWIRE( aWireNode );

    // TODO: layer map?
    // wire->SetLayer( layerMap( layer ) );

    // if( strCompare( "Wire", line, &line ) )
         wire->SetLayer( LAYER_WIRE );
    // else if( strCompare( "Bus", line, &line ) )
    //     wire->SetLayer( LAYER_BUS );
    // else if( strCompare( "Notes", line, &line ) )
    //     wire->SetLayer( LAYER_NOTES );
    // else
    //     SCH_PARSE_ERROR( "invalid line type", aReader, line );

    wxPoint begin, end;

    begin.x = ewire.x1;
    begin.y = ewire.y1;
    end.x   = ewire.x2;
    end.y   = ewire.y2;

    wire->SetStartPoint( begin );
    wire->SetEndPoint( end );

    return wire.release();
}


void SCH_EAGLE_PLUGIN::loadInstance( wxXmlNode* aInstanceNode )
{
}


void SCH_EAGLE_PLUGIN::loadModuleinst( wxXmlNode* aModuleinstNode )
{
}


void SCH_EAGLE_PLUGIN::loadLibrary( wxXmlNode* aLibraryNode )
{
    // Read the library name
    wxString libName = aLibraryNode->GetAttribute( "name" );

    // Query all children and map them into a readable dictionary
    NODE_MAP libraryChildren = mapChildren( aLibraryNode );

    // TODO: Do something with the description
    // wxXmlNode* libraryChildren["description"];

    // Loop through the devicesets and load each of them
    // wxXmlNode* devicesetNode = libraryChildren["devicesets"]->GetChildren();
    // while( devicesetNode )
    // {
    //     loadDeviceset( devicesetNode );
    //     devicesetNode = devicesetNode->GetNext();
    // }

    // Loop through the packages and load each of them
    // wxXmlNode* packageNode = libraryChildren["packages"]->GetChildren();
    // while( packageNode )
    // {
    //     loadPackage( packageNode );
    //     packageNode = packageNode->GetNext();
    // }

    // Loop through the symbols and load each of them
    wxXmlNode* symbolNode = libraryChildren["symbols"]->GetChildren();

    while( symbolNode )
    {
        loadSymbol( symbolNode );
        symbolNode = symbolNode->GetNext();
    }
}


LIB_PART* SCH_EAGLE_PLUGIN::loadSymbol( wxXmlNode* aSymbolNode )
{
    // Create a new part with the symbol name
    wxString symbolName = aSymbolNode->GetAttribute( "name" );
    std::unique_ptr<LIB_PART> part( new LIB_PART( symbolName ) );

    wxXmlNode* currentNode = aSymbolNode->GetChildren();

    while( currentNode )
    {
        wxString nodeName = currentNode->GetName();

        if( nodeName == "description" )
        {
            // TODO
            //wxASSERT_MSG( false, "'description' nodes are not implemented yet" );
        }
        else if( nodeName == "dimension" )
        {
            // TODO
            //wxASSERT_MSG( false, "'description' nodes are not implemented yet" );
        }
        else if( nodeName == "frame" )
        {
        }
        else if( nodeName == "circle" )
        {
            part->AddDrawItem( loadCircle( part.get(), currentNode ) );
        }
        else if( nodeName == "pin" )
        {
            // part->AddDrawItem( loadPin( part, currentNode ) );
        }
        else if( nodeName == "polygon" )
        {
            //part->AddDrawItem( loadPolygon( part.get(), currentNode ) );
        }
        else if( nodeName == "rectangle" )
        {
            part->AddDrawItem( loadRectangle( part.get(), currentNode ) );
        }
        else if( nodeName == "text" )
        {
            // part->AddDrawItem( loadText( part, currentNode ) );
        }
        else if( nodeName == "wire" )
        {
            // part->AddDrawItem( loadPolyline( part, currentNode ) );
        }

        currentNode = currentNode->GetNext();
    }

    return part.release();
}


LIB_CIRCLE* SCH_EAGLE_PLUGIN::loadCircle( LIB_PART* aPart, wxXmlNode* aCircleNode )
{
    // Parse the circle properties
    ECIRCLE c( aCircleNode );

    unique_ptr<LIB_CIRCLE> circle( new LIB_CIRCLE( aPart ) );

    circle->SetPosition( wxPoint( c.x, c.y ) );
    circle->SetRadius( c.radius );
    circle->SetWidth( c.width );

    return circle.release();
}


LIB_RECTANGLE* SCH_EAGLE_PLUGIN::loadRectangle( LIB_PART* aPart, wxXmlNode* aRectNode )
{
    ERECT rect( aRectNode );

    unique_ptr<LIB_RECTANGLE> rectangle( new LIB_RECTANGLE( aPart ) );

    rectangle->SetPosition( wxPoint( rect.x1, rect.y1 ) );
    rectangle->SetEnd( wxPoint( rect.x2, rect.y2 ) );

    // TODO: Manage rotation

    return rectangle.release();
}


LIB_POLYLINE* SCH_EAGLE_PLUGIN::loadPolyLine( LIB_PART* aPart, wxXmlNode* aRectNode )
{
    std::unique_ptr< LIB_POLYLINE > polyLine( new LIB_POLYLINE( aPart ) );

    /*int points = parseInt( aReader, line, &line );
    polyLine->SetUnit( parseInt( aReader, line, &line ) );
    polyLine->SetConvert( parseInt( aReader, line, &line ) );
    polyLine->SetWidth( parseInt( aReader, line, &line ) );

    wxPoint pt;

    for( int i = 0; i < points; i++ )
    {
        pt.x = parseInt( aReader, line, &line );
        pt.y = parseInt( aReader, line, &line );
        polyLine->AddPoint( pt );
    }

    if( *line != 0 )
        polyLine->SetFillMode( parseFillMode( aReader, line, &line ) );*/

    return polyLine.release();
}


void SCH_EAGLE_PLUGIN::Save( const wxString& aFileName, SCH_SCREEN* aSchematic, KIWAY* aKiway,
                             const PROPERTIES* aProperties )
{
}


size_t SCH_EAGLE_PLUGIN::GetSymbolLibCount( const wxString&   aLibraryPath,
                                            const PROPERTIES* aProperties )
{
    return 0;
}


void SCH_EAGLE_PLUGIN::EnumerateSymbolLib( wxArrayString&    aAliasNameList,
                                           const wxString&   aLibraryPath,
                                           const PROPERTIES* aProperties )
{
}


LIB_ALIAS* SCH_EAGLE_PLUGIN::LoadSymbol( const wxString& aLibraryPath, const wxString& aSymbolName,
                                         const PROPERTIES* aProperties )
{
    return nullptr;
}


void SCH_EAGLE_PLUGIN::SaveSymbol( const wxString& aLibraryPath, const LIB_PART* aSymbol,
                                   const PROPERTIES* aProperties )
{
}


void SCH_EAGLE_PLUGIN::DeleteAlias( const wxString& aLibraryPath, const wxString& aAliasName,
                                    const PROPERTIES* aProperties )
{
}


void SCH_EAGLE_PLUGIN::DeleteSymbol( const wxString& aLibraryPath, const wxString& aAliasName,
                                     const PROPERTIES* aProperties )
{
}


void SCH_EAGLE_PLUGIN::CreateSymbolLib( const wxString&   aLibraryPath,
                                        const PROPERTIES* aProperties )
{
}


bool SCH_EAGLE_PLUGIN::DeleteSymbolLib( const wxString&   aLibraryPath,
                                        const PROPERTIES* aProperties )
{
    return false;
}


bool SCH_EAGLE_PLUGIN::IsSymbolLibWritable( const wxString& aLibraryPath )
{
    return false;
}


void SCH_EAGLE_PLUGIN::SymbolLibOptions( PROPERTIES* aListToAppendTo ) const
{
}

// approved
// attribute
// circle
// clearance
// connect
// contactref
// description
// dimension
// frame
// gate
// grid
// hole
// junction
// label
// layer
// note
// pad
// param
// pin
// pinref
// port
// portref
// rectangle
// setting
// smd
// textvariant
// variantdef
// vertex
// via
// wire
