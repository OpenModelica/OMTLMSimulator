// This may look like C code, but it is really -*- C++ -*-

#ifndef MODELHOOK_H
#define MODELHOOK_H

#include "ModelNodeAddress.h"

#include "Bstring.h"


enum {
  DEFAULT,
  ONE2ONE,
  CYCLIC,
  CLIQUE
} ;


//! \class ModelHook
//!
//!
//!  Base class for the objects in the model containing the hook
//! functions.  The nodes in the model graph is of the class
//! ModelNode.  Every ModelNode instance in the
//! model graph contains a pointer to an instance of ModelHook.
//! The application programmer must instantiate this pointer so that it
//! points to an object in the model.
//!
//!  There are six hook functions (four for packing and two for
//! computation) that can be overridden in the derived class.  If they
//! are not overridden they return 0 as default.  If a hook function is
//! overridden it should return 1.
//!
//!  The hook funktions are:
//! <ul>
//! <li>  <b>pack_before_input</b> which should pack the input data needed
//!    to compute <b>compute_before</b>,
//! <li>  <b>pack_before_output</b> which packs the result of the
//!    <b>compute_before</b> computation,
//! <li>  <b>pack_after_output</b> which packs the result of the
//!    <b>compute_after</b> computation, 
//! <li>  <b>compute_before</b> which does computations before the lowest
//!     level and
//! <li>  <b>compute_after</b> which does computations after the lowest
//!      level.
//! </ul>
//!  The computations on the lowest level should be done in 
//!  <b>compute_before</b>.
//!
//! \author
//!  Tommy Persson


class ModelHook {
protected:

    // Bstring name;
    // String that is printed by the <m>print</m> method.

    ModelNodeAddress first_mna;

    int traverse_flag;

    // int child_connection_type;

public:

    //! Constructor.
    ModelHook ( ) {     
	traverse_flag = 0 ;  
    } 
  
    //! Destructor
    virtual ~ModelHook() {}

    //  virtual void set_name (const ModelNodeAddress & /* addr */,
    // const Bstring) {return;};

    //! Return the name.
    virtual Bstring get_name(const ModelNodeAddress & /* addr */) {return Bstring("Noname");}

    //! Return the name.
    virtual Bstring get_name() {return Bstring("Noname");}

    //! Hook function informing that a sequence of pack_from_application
    //! calles related to the specified slave will follow.
    virtual void prepare_pack_from_app(const ModelNodeAddress & /* addr */){}

    //! Hook function informing that a sequence of pack_to_application
    //! calles related to the specified slave will follow.
    virtual void prepare_pack_to_app(const ModelNodeAddress & /* addr */){}
  
    //! Hook function packing data from the solver that is used
    //! for the compute_before method.
    virtual int pack_from_application(const ModelNodeAddress & /* addr */) {return 0;}

    //! Hook function packing the result from the compute_before method.
    virtual int pack_downward_output(const ModelNodeAddress & /* parent */, const ModelNodeAddress & /* child */) {return 0;}
  
    //! Hook function packing the result for the compute_after method.
    virtual int pack_upward_output(const ModelNodeAddress & /* child */, const ModelNodeAddress & /* parent */) {return 0;}

    //! Hook function packing the result of the compute_after method.
    //! This is the data that should be sent directly to the master.
    virtual int pack_to_application(const ModelNodeAddress & /* addr */) {return 0;}


    //! Do the before computation.
    virtual int compute_downward(const ModelNodeAddress & /* addr */) {return 0;}

    //! Do the before computation.
    virtual int compute_downward_while(const ModelNodeAddress & /* addr */) {return 0;}

    //! Do the after computation.
    virtual int compute_upward(const ModelNodeAddress & /* addr */) {return 0;}

    //! 
    virtual int get_n_child(const ModelNodeAddress & /* addr */) {return -1;}

    //! 
    virtual int get_n_element(const ModelNodeAddress & /* addr */) {return 1;}

    //! Get the type of the child connection.
    virtual int get_child_connection_type(const ModelNodeAddress &, int /*i*/) { return DEFAULT; }

    //! 
    virtual ModelHook * get_child(const ModelNodeAddress & /* addr */, int /*i*/) {return 0;}

    //! 
    virtual ModelNodeAddress get_child_node_adress(const ModelNodeAddress & /* addr */, int /*i*/ ) {return ModelNodeAddress();}



    //! 
    virtual int get_n_parent(const ModelNodeAddress & /* addr */) {return -1;}

    //! probably not neccessary. Nordling want it for Linear algebra
    //! traversing.
    virtual ModelHook * get_parent(const ModelNodeAddress & /* addr */, int /*i*/) {return 0;}
  
//
//  virtual ModelNodeAddress get_parent_node_adress (const ModelNodeAddress &
//						    /* addr */, int i) 
//				   {return ModelNodeAddress();};
//  // probably not neccessary

    //! Set the first model node adress.
    void set_mna(const ModelNodeAddress & mna) { first_mna = mna; }

    //! Get the first model node adress.
    ModelNodeAddress get_mna() { return first_mna; }
  
    virtual ModelNodeAddress get_appl_mna() { return first_mna; }
  
    void set_traversed() { traverse_flag = 1; }
    void clear_traversed() { traverse_flag = 0; }

	//!    if (first_mna.get_level () < 0) 
	//!      return 0;
	//!    else 
	//! return 1;
    int traversed() {return traverse_flag;}

    //! Print the \a name .
    void print();

};

#endif
