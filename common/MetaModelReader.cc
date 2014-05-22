/**
 * File: MetaModelReader.cc
 * 
 * Defines the MetaModelReader class methods
 */

#include "MetaModelReader.h"
#include "TLMErrorLog.h"
#include <string>
using std::string;

// ReadComponents method reads in Components (SubModels) definition from
// the XML file starting from the given xml node that should be "SubModels".
// Input: node - pointer to the "SubModels" element node 
//   - parent to all the SubModels
// Input/Output: TheModel - structure is updated in the model representation
void MetaModelReader::ReadComponents(xmlNode *node) {
    for(xmlNode* curNode = node->children; curNode; curNode = curNode->next) {
	if(    (XML_ELEMENT_NODE == curNode->type)  
	       // A SubModel Node found, read parameters
	       && (strcmp("SubModel", (const char*)(curNode->name)) == 0)) {
		
	    xmlNode* curAttrVal = FindAttributeByName(curNode, "Name");
	    string Name((const char *)curAttrVal->content);

	    TLMErrorLog::Log(string("Processing SubModel ") + Name);

	    curAttrVal = FindAttributeByName(curNode, "StartCommand");
	    string StartCommand((const char*)curAttrVal->content);

            curAttrVal = FindAttributeByName(curNode, "ModelFile");
            string ModelFile((const char*)curAttrVal->content);

	    curAttrVal = FindAttributeByName(curNode, "ExactStep");
	    bool SolverMode = (curAttrVal->content[0] == '1');
		
	    if(!SolverMode && (curAttrVal->content[0] == '1')) {
		TLMErrorLog::FatalError("Unexpected value of ExactStep attribute."
			   " Must be 0 or 1.");
	    }

	    // Now we're registering a proxy for the new component 
	    // and getting its ID
	    int compID = 
		TheModel.RegisterTLMComponentProxy(Name, 
						   StartCommand,
                                                   ModelFile,
						   SolverMode);

	    // Read the interface definitions (should be in the children nodes)
	    ReadTLMInterfaceNodes(curNode, compID);
	}
    }
}


// ReadTLMInterfaceNodes method reads in TLM interface definitions for a
// given SubModel XML node and its ID (ComponentID).
void MetaModelReader::ReadTLMInterfaceNodes(xmlNode* node, int ComponentID) {
    for(xmlNode* curNode = node->children; curNode; curNode = curNode->next) {
	if(    (XML_ELEMENT_NODE == curNode->type)
	       && (strcmp("InterfacePoint", (const char*)(curNode->name)) == 0)) { 
	    // For every InterfacePoint element that we find read its name

	    xmlNode* curAttrVal = FindAttributeByName(curNode, "Name");

	    string name((const char*)curAttrVal->content);

	    TLMErrorLog::Log(string("Registering TLM interface ") + name);
	    TheModel.RegisterTLMInterfaceProxy(ComponentID, name);
	}
    }
}


// ReadSimParams method reads in simulation parameters (Port, StartTime, StopTime)
// from XML-element node for a TLMConnection
void MetaModelReader::ReadSimParams(xmlNode* node) {

    xmlNode* curAttrVal = FindAttributeByName(node, "ManagerPort");
    int Port = atoi((const char*)curAttrVal->content);
	
    curAttrVal = FindAttributeByName(node, "StartTime");
    double StartTime = atof((const char*)curAttrVal->content);

    curAttrVal = FindAttributeByName(node, "StopTime");
    double StopTime = atof((const char*)curAttrVal->content);

    //curAttrVal = FindAttributeByName(node, "SimInputFile");
    //std::string Infile = (const char*)curAttrVal->content;

    TheModel.GetSimParams().Set(Port, StartTime, StopTime);
}


// FindChildByName is an utility function used for finding child elements by name
// for a given XML node. Used for looking up required sections in the XML document.
// Returns: xmlNode* giving address of the found node or NULL if an optional node
// is not found.
xmlNode* MetaModelReader::FindChildByName(xmlNode* node, char* name, bool required ){

    for(xmlNode* curNode = node->children ; curNode; curNode = curNode->next) {
	if(    (XML_ELEMENT_NODE == curNode->type)
	       && (strcmp(name, (const char*)(curNode->name))  == 0)  ) {
	    return curNode;
	}
    }

    if(required) {
	TLMErrorLog::FatalError(string("Cannot find required XML node ") + name);
    }
    return NULL;
}


// FindAttributeByName is an utility function for finding node attributes by name
// for a given XML element node. Used for looking up required attributes while
// building the Model structure.
// Returns: xmlNode* providing address of the found attribute or NULL
xmlNode* MetaModelReader::FindAttributeByName(xmlNode* node, char* name, bool required ){

    for(xmlAttr* curAttr = node->properties; curAttr; curAttr = curAttr->next) {
	if(strcmp(name, (const char*)curAttr->name) == 0) {
	    return curAttr->children;
	}
    }
    if(required) {
	TLMErrorLog::FatalError(string("Cannot find attribute ") +  name);
    }
    return NULL;
}

// ReadTLMConnectionNode method processes an TLM connection definition in XML file.
// The definition is submitted as xmlNode* and is registered in TheModel as a 
// result of the method.
void MetaModelReader::ReadTLMConnectionNode(xmlNode* node) {

    TLMErrorLog::Log(string("Reading definition for Connections "));

    for(xmlNode* curNode = node->children; curNode; curNode = curNode->next) {
	if(    (XML_ELEMENT_NODE == curNode->type)
	       && (strcmp("Connection", (const char*)curNode->name)  == 0)  ) {

	    TLMErrorLog::Log(string("Processing Connection: "));

	    // Read connection attributes:
	    xmlNode* curAttr = FindAttributeByName(curNode, "From");
	    string AttrData((const char*)curAttr->content);
	    TLMErrorLog::Log(string("From:") + AttrData);

	    int fromID, toID;
	    TLMConnectionParams conParam;

	    fromID = TheModel.GetTLMInterfaceID( AttrData );
	    if(fromID < 0) {
		TLMErrorLog::FatalError(string("Could not find definition for interface ") 
			   + AttrData);
	    }

	    curAttr = FindAttributeByName(curNode, "To");

	    AttrData = (const char*)curAttr->content;
	    TLMErrorLog::Log(string("To:") + AttrData);
	    toID = TheModel.GetTLMInterfaceID( AttrData );
	    if(toID < 0) {
		TLMErrorLog::FatalError(string("Could not find definition for interface ") 
			   + AttrData);
	    }

	    curAttr = FindAttributeByName(curNode, "Delay");
	    conParam.Delay = atof((const char*)curAttr->content);

	    curAttr = FindAttributeByName(curNode, "Zf");
	    conParam.Zf = atof((const char*)curAttr->content);

	    curAttr = FindAttributeByName(curNode, "Zfr",false);
	    if(curAttr) {
		conParam.Zfr = atof((const char*)curAttr->content);
	    }
	    else {
		TLMErrorLog::Warning(string("No impedance for rotation (Zfr) is defined, Zf will be used"));
		conParam.Zfr = conParam.Zf;
	    }


	    curAttr = FindAttributeByName(curNode, "alpha",false);
	    if(curAttr) {
		conParam.alpha =  atof((const char*)curAttr->content);
	    }
	    else {
		TLMErrorLog::Warning(string("No damping coefficient (alpha) is defined, assume no damping"));
		conParam.alpha = 0.0;
	    }
		    
	    TLMInterfaceProxy& fromIfc = TheModel.GetTLMInterfaceProxy(fromID);
	    TLMInterfaceProxy& toIfc = TheModel.GetTLMInterfaceProxy(toID);
	    int conID = TheModel.RegisterTLMConnection(fromID, toID, conParam);
	    TLMConnection& con = TheModel.GetTLMConnection(conID);

	    fromIfc.SetConnection(con);
	    toIfc.SetConnection(con);
	}
			       
    }
	
} // ReadTLMConnectionNode(xmlNode* node)


// ReadModel method processes input XML file and creates MetaModel definition.
// Input: InputFile - input XML file name
// Input/Output: TheModel - model structure to be build.
void MetaModelReader::ReadModel(string& InputFile) {
	
    xmlDoc* doc = xmlParseFile(InputFile.c_str()); // open XML & parse it
	
    if (doc == NULL) {
	TLMErrorLog::FatalError(string("Could not parse input file ") + InputFile);
    }

    xmlNode *model_element = xmlDocGetRootElement(doc); // get root element

    TLMErrorLog::Log("XML file is parsed OK. Creating model.");

    xmlNode *components = FindChildByName(model_element, "SubModels");

    ReadComponents(components);

    xmlNode *connections = FindChildByName(model_element, "Connections");

    ReadTLMConnectionNode(connections);

    xmlNode *sim_params = FindChildByName(model_element, "SimulationParams");

    ReadSimParams(sim_params);

    TLMErrorLog::Log(" ----------------------  Meta-model is read  ----------------------");
	
    // free the document and the global vars
    xmlFreeDoc(doc);
    xmlCleanupParser();
}
