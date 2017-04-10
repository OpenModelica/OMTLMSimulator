//! \file: MetaModelReader.h
//!
//! Defines the MetaModelReader class which provides an interface
//! for reading in the meta-model information from xml file
//!
#ifndef MetaModelReader_h
#define MetaModelReader_h

#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>

#include "MetaModels/MetaModel.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

//! Class MetaModelReader is responsible for reading meta model definition from
//! an XML file and passing the information to the MetaModel classes
//! to create the internal representation of the model.
class MetaModelReader {

    //! The model object to be filled in during reading
    MetaModel& TheModel;

    //! ReadComponents method reads in Components (SubModels) definition from
    //! the XML file starting from the given xml node that should be "SubModels".
    //! Input: \param node - pointer to the "SubModels" element node
    //!   - parent to all the SubModels
    //! Input/Output: TheModel - structure is updated in the model representation
    void ReadComponents(xmlNode *node, bool skipInterfaces, std::string singleModel);

    //! ReadTLMInterfaceNodes method reads in TLM interface definitions for a
    //! given SubModel XML node and its ID (ComponentID).
    void ReadTLMInterfaceNodes(xmlNode* node, int ComponentID);

    void ReadTLMParameters(xmlNode* node, int ComponentID);


    //! ReadSimParams method reads in simulation parameters (Port, StartTime, StopTime)
    //! from XML-element node for a TLMConnection
    void ReadSimParams(xmlNode* node);

    //! ReadVectorAttribute method reads a nodes 3D vector attribute, if applicable.
    //! For instance, reads a position vector "x,y,z", that is, Position="0.0,1.0,-0.3".
    //! \param node The current XML node that might have an attribute of the given name "attribute"
    //! \param attribute The name of the attribute, for instance, "Position"
    //! \param pos The 3D vector that contains the result, that is, the 3D vector read from the XML node.
    //!            This field will be unchanged if the attribute is not found.
    void ReadVectorAttribute(xmlNode* node, const char* attribute, double pos[3]);

    //! ReadDoubleAttribute method reads a double value attribute, if applicable.
    //! \param node The current XML node that might have an attribute of the given name "attribute"
    //! \param attribute The name of the attribute, for instance, "Position"
    //! \return The result, that is, the double value read from the XML node.
    //!         Returns 0.0 if the attribute is not found.
    double ReadDoubleAttribute(xmlNode* node, const char *attribute);

    //! ReadPositionAndOrientation method reads position and orientation (phi angles) from the meta-model XML file.
    //! Orientation 3x3 matrix A is created from the phi angles.
    //! \param node The current XML node that contains the location and orientation attributes.
    //! \param R    The position vector, output.
    //! \param A    The 3x3 orientation matrix, output.
    void ReadPositionAndOrientation(xmlNode* node, double R[3], double A[9]);

    //! FindChildByName is an utility function used for finding child elements by name
    //! for a given XML node. Used for looking up required sections in the XML document.
    //! Returns: xmlNode* giving address of the found node or NULL if an optional node
    //! is not found.
    xmlNode* FindChildByName(xmlNode* node, const char* name, bool required = true);

    //! FindAttributeByName is an utility function for finding node attributes by name
    //! for a given XML element node. Used for looking up required attributes while
    //! building the Model structure.
    //! Returns: xmlNode* providing address of the found attribute or NULL
    xmlNode* FindAttributeByName(xmlNode* node, const char* name, bool required = true);


    //! ReadTLMConnectionNode method processes an TLM connection definition in XML file.
    //! The definition is submitted as xmlNode* and is registered in TheModel as a
    //! result of the method.
    void ReadTLMConnectionNode(xmlNode* node);

public:

    //! Constructor
    MetaModelReader(MetaModel& model) : TheModel(model) {}


    //! ReadModel method processes input XML file and creates MetaModel definition.
    //! Input: InputFile - input XML file name
    //! Input/Output: TheModel - model structure to be build.
    void ReadModel(std::string& InputFile, bool SkipConnections=false, std::string singleModel="");

};



#endif
