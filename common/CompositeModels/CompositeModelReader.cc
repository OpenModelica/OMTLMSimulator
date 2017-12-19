/**
 * File: CompositeModelReader.cc
 *
 * Defines the CompositeModel class methods
 */

#include "CompositeModels/CompositeModelReader.h"
#include "Logging/TLMErrorLog.h"
#include "Interfaces/TLMInterface.h"
#include "double3.h"
#include "double33.h"
#include <string>
#include <sstream>
using std::string;

// ReadComponents method reads in Components (SubModels) definition from
// the XML file starting from the given xml node that should be "SubModels".
// Input: node - pointer to the "SubModels" element node 
//   - parent to all the SubModels
// Input/Output: TheModel - structure is updated in the model representation
void CompositeModelReader::ReadComponents(xmlNode *node, bool skipInterfaces=false, std::string singleModel="") {
    for(xmlNode* curNode = node->children; curNode; curNode = curNode->next) {
        if(   (XML_ELEMENT_NODE == curNode->type)
               // A SubModel Node found, read parameters
               && (strcmp("SubModel", (const char*)(curNode->name)) == 0)) {

            xmlNode* curAttrVal = FindAttributeByName(curNode, "Name");
            string Name((const char *)curAttrVal->content);

            if(skipInterfaces && singleModel != "" && Name != singleModel) {
                std::cout << "Skipping model " << Name << "\n";
                continue; //Don't load other models than the single model
            }

            TLMErrorLog::Info(string("-----  Processing SubModel  ----- "));
            TLMErrorLog::Info("Name: "+Name);

            curAttrVal = FindAttributeByName(curNode, "StartCommand");
            string StartCommand((const char*)curAttrVal->content);

            curAttrVal = FindAttributeByName(curNode, "ModelFile");
            string ModelFile((const char*)curAttrVal->content);

            // ExactStep means synchronous communication.
            // This means that the step size in the submodel is limited to the full
            // TLM time delay instead of half, since no interpolation is required.
            // It does not seem to work properly for some reason.
            curAttrVal = FindAttributeByName(curNode, "ExactStep", false);
            bool SolverMode = false;
            if(curAttrVal != NULL) {
                SolverMode = (curAttrVal->content[0] == '1');
                if(!SolverMode && (curAttrVal->content[0] == '1')) {
                    TLMErrorLog::FatalError("Unexpected value of ExactStep attribute. Must be 0 or 1.");
                }
            }

            curAttrVal = FindAttributeByName(curNode, "GeometryFile", false);
            string GeometryFile = "";
            if(curAttrVal != NULL) {
                GeometryFile = (const char*)curAttrVal->content;
            }

            // Now we're registering a proxy for the new component
            // and getting its ID
            int compID =
                    TheModel.RegisterTLMComponentProxy(Name,
                                                       StartCommand,
                                                       ModelFile,
                                                       SolverMode,
                                                       GeometryFile);

            TLMComponentProxy& cp = TheModel.GetTLMComponentProxy(compID);

            // Here we store the model location and orientation.
            double R[3] = {0.0, 0.0, 0.0};
            double A[9] = {1.0, 0.0, 0.0,
                           0.0, 1.0, 0.0,
                           0.0, 0.0, 1.0};

            ReadPositionAndOrientation(curNode, R, A);
            cp.SetInertialTranformation(R, A);

            //double scale = ReadDoubleAttribute(curNode, "UnitScale");
            //if(scale == 0.0) scale = 1.0;

            // Read the interface definitions (should be in the children nodes)
            if(!skipInterfaces) {
                ReadTLMInterfaceNodes(curNode, compID);
            }

            if(!skipInterfaces) {
                ReadComponentParameters(curNode, compID);
            }
        }
    }
}


// ReadTLMInterfaceNodes method reads in TLM interface definitions for a
// given SubModel XML node and its ID (ComponentID).
void CompositeModelReader::ReadTLMInterfaceNodes(xmlNode* node, int ComponentID) {
    for(xmlNode* curNode = node->children; curNode; curNode = curNode->next) {
        if(   (XML_ELEMENT_NODE == curNode->type)
               && (strcmp("InterfacePoint", (const char*)(curNode->name)) == 0)) {
            // For every InterfacePoint element that we find read its name

            xmlNode* curAttrVal = FindAttributeByName(curNode, "Name");
            string Name((const char*)curAttrVal->content);

            int Dimensions = 6;
            curAttrVal = FindAttributeByName(curNode, "Dimensions");
            if(curAttrVal) {
                Dimensions = atoi((const char*)curAttrVal->content);
            }

            string Causality = "Bidirectional";
            curAttrVal = FindAttributeByName(curNode, "Causality");
            if(curAttrVal) {
                Causality = (const char*)curAttrVal->content;
            }

            string Domain="Mechanical";
            curAttrVal = FindAttributeByName(curNode, "Domain");
            if(curAttrVal) {
                Domain = (const char*)curAttrVal->content;
            }

            //            InterfaceType type=Type3D;                                     //Default is 3D
            //            if(name.size() > 1 &&                                 //Temporary hack: if name of interface ends
            //               name[name.size()-2] == '1' &&                      //with "1D" it is a 1D connection
            //               name[name.size()-1] == 'D') {
            //                type = Type1D;
            //            }
            //            else if(name.size() > 3 &&                            //Temporary hack: if name of interface ends
            //               name[name.size()-4] == '1' &&                      //with "1DIN" it is a signal input interface
            //               name[name.size()-3] == 'D' &&
            //               name[name.size()-2] == 'I' &&
            //                name[name.size()-1] == 'N') {
            //                type = TypeInput;
            //            }
            //            else if(name.size() > 4 &&                            //Temporary hack: if name of interface ends
            //               name[name.size()-5] == '1' &&                      //with "1DOUT" it is a signal output interface
            //               name[name.size()-4] == 'D' &&
            //               name[name.size()-3] == 'O' &&
            //               name[name.size()-2] == 'U' &&
            //               name[name.size()-1] == 'T') {
            //                type = TypeOutput;
            //            }
            //            if(curAttrVal) {                                      //Now check for XML attribute
            //              type = ((const char*)curAttrVal->content);
            //            }

            int ipID = TheModel.RegisterTLMInterfaceProxy(ComponentID, Name, Dimensions, Causality, Domain);

            // Get/Set position and orientation if available in XML file.
            TLMInterfaceProxy& ip = TheModel.GetTLMInterfaceProxy(ipID);
            ReadPositionAndOrientation(curNode,
                                       ip.getTime0Data3D().Position,
                                       ip.getTime0Data3D().RotMatrix);
        }
    }
}

void CompositeModelReader::ReadComponentParameters(xmlNode *node, int ComponentID) {
    for(xmlNode* curNode = node->children; curNode; curNode = curNode->next) {
        if(   (XML_ELEMENT_NODE == curNode->type)
               && (strcmp("Parameter", (const char*)(curNode->name)) == 0)) {

            xmlNode* curAttrVal = FindAttributeByName(curNode, "Name");
            string Name((const char*)curAttrVal->content);

            curAttrVal = FindAttributeByName(curNode, "Value");
            string Value((const char*)curAttrVal->content);

            TheModel.RegisterComponentParameterProxy(ComponentID, Name, Value);
        }
    }
}


// ReadDoubleAttribute method reads a double value attribute, if applicable.
double CompositeModelReader::ReadDoubleAttribute(xmlNode* node, const char* attribute) {
    xmlNode* curAttrVal = FindAttributeByName(node, attribute, false);

    if(curAttrVal) {
        return atof((const char*)curAttrVal->content);
    }

    return 0.0;
}

// ReadVectorAttribute method reads a nodes 3D vector attribute if applicable.
// For instance, reads a position vector "x,y,z", that is, Position="0.0,1.0,-0.3"
void CompositeModelReader::ReadVectorAttribute(xmlNode* node, const char *attribute, double val[3]) {
    xmlNode* curAttrVal = FindAttributeByName(node, attribute, false);

    if(curAttrVal) {
        const std::string strContent = (const char*)curAttrVal->content;
        size_t c1 = strContent.find(',');
        size_t c2 = strContent.rfind(',');
        if(c1 != std::string::npos && c1 != std::string::npos && c1 != c2 && c1 > 0) {
            std::string strXPos = strContent.substr(0, c1);
            std::string strYPos = strContent.substr(c1+1, c2-c1-1);
            std::string strZPos = strContent.substr(c2+1);

            val[0] = atof(strXPos.c_str());
            val[1] = atof(strYPos.c_str());
            val[2] = atof(strZPos.c_str());
        }
        else {
            TLMErrorLog::FatalError("Wrong format in " + std::string(attribute) + " attribute: " + strContent + ", should be \"X,Y,Z\"");
            exit(1);
        }
    }
}

// ReadVectorAttribute method reads a nodes 3D vector attribute if applicable.
// For instance, reads a position vector "x,y,z", that is, Position="0.0,1.0,-0.3"
void CompositeModelReader::ReadPositionAndOrientation(xmlNode* node, double R[3], double A[9]) {
    double phi[3] = {0.0, 0.0, 0.0};

    ReadVectorAttribute(node, "Position", R);
    ReadVectorAttribute(node, "Angle321", phi);

    double33 A33 = A321(double3(phi[0],phi[1],phi[2]));

    for(int i=0; i<9; i++) {
        A[i] = A33(i/3+1, i%3+1);
    }
}

// ReadSimParams method reads in simulation parameters (Port, StartTime, StopTime)
// from XML-element node for a TLMConnection
void CompositeModelReader::ReadSimParams(xmlNode* node) {

    TLMErrorLog::Info(string("-----  Reading simulation parameters  ----- "));
    xmlNode* curAttrVal = FindAttributeByName(node, "ManagerPort", false);

    int Port = 11111; // Some default port.
    if(curAttrVal != NULL) {
        Port = atoi((const char*)curAttrVal->content);
    }

    curAttrVal = FindAttributeByName(node, "StartTime");
    double StartTime = atof((const char*)curAttrVal->content);

    curAttrVal = FindAttributeByName(node, "StopTime");
    double StopTime = atof((const char*)curAttrVal->content);

    if(StartTime >= StopTime) {
        TLMErrorLog::FatalError("StartTime must be smaller than StopTime, check your model!");
        exit(1);
    }

    double WriteTimeStep = (StopTime-StartTime)/1000.0;
    curAttrVal = FindAttributeByName(node, "WriteTimeStep", false);
    if(curAttrVal != 0) {
        WriteTimeStep = atof((const char*)curAttrVal->content);
    }

    //curAttrVal = FindAttributeByName(node, "SimInputFile");
    //std::string Infile = (const char*)curAttrVal->content;

    // Note, order is important here!
    TheModel.GetSimParams().Set(Port, StartTime, StopTime);
    TheModel.GetSimParams().SetWriteTimeStep(WriteTimeStep);

    TLMErrorLog::Info("StartTime     = "+TLMErrorLog::ToStdStr(StartTime)+" s");
    TLMErrorLog::Info("StopTime      = "+TLMErrorLog::ToStdStr(StopTime)+" s");
    TLMErrorLog::Info("WriteTimeStep = "+TLMErrorLog::ToStdStr(WriteTimeStep)+" s");
}


// FindChildByName is an utility function used for finding child elements by name
// for a given XML node. Used for looking up required sections in the XML document.
// Returns: xmlNode* giving address of the found node or NULL if an optional node
// is not found.
xmlNode* CompositeModelReader::FindChildByName(xmlNode* node, const char* name, bool required) {

    for(xmlNode* curNode = node->children; curNode; curNode = curNode->next) {
        if(   (XML_ELEMENT_NODE == curNode->type)
               && (strcmp(name, (const char*)(curNode->name))  == 0) ) {
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
xmlNode* CompositeModelReader::FindAttributeByName(xmlNode* node, const char* name, bool required) {

    for(xmlAttr* curAttr = node->properties; curAttr; curAttr = curAttr->next) {
        if(strcmp(name, (const char*)curAttr->name) == 0) {
            return curAttr->children;
        }
    }
    if(required) {
        TLMErrorLog::Warning(string("Cannot find attribute ") +  name);
    }
    return NULL;
}

// ReadTLMConnectionNode method processes an TLM connection definition in XML file.
// The definition is submitted as xmlNode* and is registered in TheModel as a 
// result of the method.
void CompositeModelReader::ReadTLMConnectionNode(xmlNode* node) {

    TLMErrorLog::Info(string("Reading definition for Connections "));
    if(node != 0) {
        for(xmlNode* curNode = node->children; curNode; curNode = curNode->next) {
            if(   (XML_ELEMENT_NODE == curNode->type)
                   && (strcmp("Connection", (const char*)curNode->name)  == 0) ) {

                TLMErrorLog::Info(string("-----  Processing Connection  ----- "));

                // Read connection attributes:
                xmlNode* curAttr = FindAttributeByName(curNode, "From");
                string AttrData((const char*)curAttr->content);
                TLMErrorLog::Info(string("From:") + AttrData);

                int fromID, toID;
                TLMConnectionParams conParam;

                fromID = TheModel.GetTLMInterfaceID(AttrData);
                if(fromID < 0) {
                    TLMErrorLog::FatalError(string("Could not find definition for interface ")
                                            + AttrData);
                }

                curAttr = FindAttributeByName(curNode, "To");

                AttrData = (const char*)curAttr->content;
                TLMErrorLog::Info(string("To:") + AttrData);
                toID = TheModel.GetTLMInterfaceID(AttrData);
                if(toID < 0) {
                    TLMErrorLog::FatalError(string("Could not find definition for interface ")
                                            + AttrData);
                }

                TLMInterfaceProxy& fromIfc = TheModel.GetTLMInterfaceProxy(fromID);
                TLMInterfaceProxy& toIfc = TheModel.GetTLMInterfaceProxy(toID);

                curAttr = FindAttributeByName(curNode, "Delay");
                conParam.Delay = atof((const char*)curAttr->content);

                TLMErrorLog::Info("Delay = "+TLMErrorLog::ToStdStr(conParam.Delay)+" s");

                if(fromIfc.GetCausality() == "Bidirectional") {
                    curAttr = FindAttributeByName(curNode, "Zf");
                    conParam.Zf = atof((const char*)curAttr->content);
                    TLMErrorLog::Info("Zf    = "+TLMErrorLog::ToStdStr(conParam.Zf));
                }

                if(fromIfc.GetCausality() == "Bidirectional" && fromIfc.GetDimensions() > 1) {
                    curAttr = FindAttributeByName(curNode, "Zfr",false);
                    if(curAttr) {
                        conParam.Zfr = atof((const char*)curAttr->content);
                    }
                    else {
                        TLMErrorLog::Warning(string("No impedance for rotation (Zfr) is defined, Zf will be used"));
                        conParam.Zfr = conParam.Zf;
                    }
                    TLMErrorLog::Info("Zf    = "+TLMErrorLog::ToStdStr(conParam.Zfr));
                }

                if(fromIfc.GetCausality() == "Bidirectional") {
                    curAttr = FindAttributeByName(curNode, "alpha",false);
                    if(curAttr) {
                        conParam.alpha =  atof((const char*)curAttr->content);
                    }
                    else {
                        TLMErrorLog::Warning(string("No damping coefficient (alpha) is defined, assume no damping"));
                        conParam.alpha = 0.0;
                    }
                    TLMErrorLog::Info("alpha = "+TLMErrorLog::ToStdStr(conParam.alpha));
                }

                int conID = TheModel.RegisterTLMConnection(fromID, toID, conParam);
                TLMConnection& con = TheModel.GetTLMConnection(conID);

                fromIfc.SetConnection(con);
                toIfc.SetConnection(con);
            }
        }
    }
    else {
        TLMErrorLog::Info(string("No connections found, continue anyway."));
    }

} // ReadTLMConnectionNode(xmlNode* node)


// ReadModel method processes input XML file and creates CompositeModel definition.
// Input: InputFile - input XML file name
// Input/Output: TheModel - model structure to be build.
void CompositeModelReader::ReadModel(std::string &InputFile, bool InterfaceRequestMode, std::string singleModel) {

    TLMErrorLog::Info("----------------------  Reading composite model  ---------------------- ");
    xmlDoc* doc = xmlParseFile(InputFile.c_str()); // open XML & parse it

    if(doc == NULL) {
        TLMErrorLog::FatalError(string("Could not parse input file ") + InputFile);
    }

    xmlNode *model_element = xmlDocGetRootElement(doc); // get root element

    TLMErrorLog::Info("XML file is parsed OK. Creating model.");

    xmlNode *components = FindChildByName(model_element, "SubModels");  //Don't load interfaces in interface request mode

    ReadComponents(components, InterfaceRequestMode, singleModel);

    xmlNode *connections = FindChildByName(model_element, "Connections", false); // We allow models without connections for interface request mode.

    //Don't load connections if interface request mode
    if(!InterfaceRequestMode) {
        ReadTLMConnectionNode(connections);
    }

    xmlNode *sim_params = FindChildByName(model_element, "SimulationParams");

    ReadSimParams(sim_params);

    TLMErrorLog::Info("----------------------  Composite model is read  ---------------------- ");

    // free the document and the global vars
    xmlFreeDoc(doc);
    xmlCleanupParser();
}
