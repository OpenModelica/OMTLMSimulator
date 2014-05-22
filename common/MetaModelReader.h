//! \file: MetaModelReader.h
//! 
//! Defines the MetaModelReader class which provides an interface
//! for reading in the meta-model information from xml file
//!
#ifndef MetaModelReader_h
#define MetaModelReader_h

#include <cstdio>
#include <string>

#include "MetaModel.h"

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
    void ReadComponents(xmlNode *node);

    //! ReadTLMInterfaceNodes method reads in TLM interface definitions for a
    //! given SubModel XML node and its ID (ComponentID).
    void ReadTLMInterfaceNodes(xmlNode* node, int ComponentID);


    //! ReadSimParams method reads in simulation parameters (Port, StartTime, StopTime)
    //! from XML-element node for a TLMConnection
    void ReadSimParams(xmlNode* node);


    //! FindChildByName is an utility function used for finding child elements by name
    //! for a given XML node. Used for looking up required sections in the XML document.
    //! Returns: xmlNode* giving address of the found node or NULL if an optional node
    //! is not found.
    xmlNode* FindChildByName(xmlNode* node, char* name, bool required = true);

    //! FindAttributeByName is an utility function for finding node attributes by name
    //! for a given XML element node. Used for looking up required attributes while
    //! building the Model structure.
    //! Returns: xmlNode* providing address of the found attribute or NULL
    xmlNode* FindAttributeByName(xmlNode* node, char* name, bool required = true);


    //! ReadTLMConnectionNode method processes an TLM connection definition in XML file.
    //! The definition is submitted as xmlNode* and is registered in TheModel as a 
    //! result of the method.
    void ReadTLMConnectionNode(xmlNode* node);

 public:
    
    //! Constructor
    MetaModelReader(MetaModel& model) : TheModel(model) {};


    //! ReadModel method processes input XML file and creates MetaModel definition.
    //! Input: InputFile - input XML file name
    //! Input/Output: TheModel - model structure to be build.
    void ReadModel(std::string& InputFile);
 
};



#endif
