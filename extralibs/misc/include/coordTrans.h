#ifndef _COORDTRANS_H
#define _COORDTRANS_H

#include "double3Vec.h"
// For inlines
#include "double33Mat.h"
// For inlines

namespace tlmMisc
{

  class double33Mat;
  class double3Vec;
 
  //! The notation and functions are explained in the report D. Fritzson, "Notation and Vector algebra", 2015.
  //!   Located at /home/Beast/IntegrationRep/doc/Reports/Theory/NotationAndVectorAlgebra/
  //! This report refers to
  //! [1] C. Fuhrer, P. Nordling, D. Fritzson, "Cardan Angles versus Euler Parameters for Simulation of Rolling Bearings", 1997.
  //!   Located at /home/Beast/IntegrationRep/doc/Reports/Theory/CardanAnglesVersusEulerParameter/
  //! [4] B. Graf, "Quaternions And Dynamics", 2007.
  //!   Located at /home/Beast/IntegrationRep/doc/Reports/Theory/QuaternionsAndDynamics/
  //! Note, that in [1] due to a typo a factor 1/2 is missing in equation 3.16 and 3.17 .
  //! Note, that in [4] a sequence [q_0, q_1, q_2, q_3] is used for quaternions instead of [q_1, q_2, q_3, q_4] used here,
  //! where q_4 corresponds to q_0. Comparing the equations with below, the equations in [4] need to be rearranged accordingly.



  //!
  //! Declarations of functions
  //!
  //! Here we declare functions used for coordinate transformations etc.
  //!


  //! Validate the Euler parameters with a tolerance of 1.0e-6
  int validate_euler(double q1,double q2,double q3,double q4) ;

  //! Validate the Euler parameters with a tolerance of 1.0e-15
  int validate_euler2(double q1,double q2,double q3,double q4) ;

  //! Validate the Euler parameters with a tolerance of 1.0e-6
  int validate_euler(double* q) ;

  //! Create a transformation matrix from base system (c1) to local system (c2), i.e., p^(c2) = A_c2/c1 * p^(c1)
  //! We use Euler parameters.
  //! q_i = sin(theta/2)*u_i i=1,2,3, where theta is a single rotation around an axis u = [u_1, u_2, u_3] (u is a unit direction vector).
  //! q_4 = cos(theta/2)
  //! We have also that q_1 + q_2 + q_3 + q_4 = 1.
  const double33Mat Aeuler(const double q1,
			const double q2,
			const double q3,
			const double q4) ;


  //! Create an inverse transformation matrix from base system to local system.
  //! We use Euler parameters.
  const double33Mat invAeuler(const double q1,
			   const double q2,
			   const double q3,
			   const double q4) ;

  //! Transformation from Euler parameters to rotation angles.
  //! We use 3,2,1 rotation scheme. The angles will be cyclic due to arcsin function.
  const double3Vec EulerTophi321(const double q1,
			      const double q2,
			      const double q3,
			      const double q4) ;


  //! Transformation from rotation angles to Euler parameters.
  //! We use 3,2,1 rotation scheme.
  void phi321ToEuler(const double3Vec& phi,
		     double& q1,
		     double& q2,
		     double& q3,
		     double& q4) ;


  //! Calculate Euler parameters derivatives with respect to the phi321 angles for given orientation.
  void phi321ToEulerDer(const double3Vec& phi,
			double3Vec& dq1_dphi,
			double3Vec& dq2_dphi,
			double3Vec& dq3_dphi,
			double3Vec& dq4_dphi);


  //! Transformation from rotation angles to  Euler parameters.
  //! We use 1,2,3 rotation scheme.
  void phi123ToEuler(const double3Vec& phi,
		     double& q1,
		     double& q2,
		     double& q3,
		     double& q4) ;

  //! Calculate Euler parameters derivatives with respect to the phi123 angles for given orientation.
  void phi123ToEulerDer(const double3Vec& phi,
			double3Vec& dq1_dphi,
			double3Vec& dq2_dphi,
			double3Vec& dq3_dphi,
			double3Vec& dq4_dphi);

  //! Transformation from rotation angles to  Euler parameters.
  //! phiSequence 0 -> A321
  //! phiSequence 1 -> A123
  void phiToEuler(const double3Vec& phi,
		  const int phiSequence,
		  double& q1,
		  double& q2,
		  double& q3,
		  double& q4) ;

  //! The conversion from a rotation matrix to
  //! Euler parameters (quaternions of length 1) is unique
  //! up to a sign \f$\{+1,-1\}\f$. That is, \f$(q_1,q_2,q_3,q_4)\f$ represents
  //! the same rotation matrix as \f$(-q_1,-q_2,-q_3,-q_4)\f$.
  //! This implementation is done so that \f$q_1\f$ is always non-negative.
  //!
  //! \par Theory
  //!
  //! For further information, see Chapter 2 in 
  //! "Lie Groups: An Introduction Through Linear Groups" by
  //! Wulf Rossmann. Especially Lemma 2A on page 34.
  //!
  //! The Euler parameters \f$q\f$ is an element in the 3-sphear
  //! \f$S^3 = \{ q \in \mathbf{R}^4 ; ||q||=1 \} \f$. This space
  //! is isomorphic to 
  //! \f$SU(2) = \{ B \in GL(2,\mathbf{C}) ; B^* B = 1 \} \f$, where
  //! \f$B = [\alpha,-\bar\beta;\beta,\bar\alpha]\f$. 
  //! An isomorphism is given by 
  //! \f$(\alpha,\beta) \mapsto 
  //! (\Re(\beta),\Im(\beta),\Im(\alpha),\Re(\alpha))=(q_1,q_2,q_3,q_4)\f$.
  //!
  //! A surjective map \f$ \phi: S^3 \ni q \mapsto A \in SO(3)\f$ is given by \f[
  //! A = \left[ \begin{array}{ccc}
  //! q_1^2-q_2^2-q_3^2+q_4^2 & 2(q_1 q_2+q_3 q_4) & 2(q_1 q_3-q_2 q_4)
  //! \\ 2(q_1q_2-q_3q_4) & -q_1^2+q_2^2-q_3^2+q_4^2 & 2(q_2q_3+q_1q_4)
  //! \\ 2(q_1 q_3+q_2q_4) & 2(q_2q_3-q_1q_4) & -q_1^2-q_2^2+q_3^2+q_4^2
  //! \end{array} \right] \f]
  //! (Notice from the formula that the sign of \f$q\f$ does not matter.)
  //!
  //! The corresponding injective map \f$ \psi : SO(3) \ni A \mapsto q \in S^3\f$ 
  //! fulfills \f[ \begin{array}{c}
  //! |q_1| = \sqrt{1+A_{11}-A_{22}-A_{33}}/2
  //! \\ |q_2| = \sqrt{1-A_{11}+A_{22}-A_{33}}/2
  //! \\ |q_3| = \sqrt{1-A_{11}-A_{22}+A_{33}}/2
  //! \\ |q_4| = \sqrt{1+A_{11}+A_{22}+A_{33}}/2
  //! \end{array} \f]
  //! The signs follows by combining off diagonal elements in the
  //! formula for \f$q \mapsto A\f$ and the choice that 
  //! \f$q_1 \geq 0 \f$ (see the actual implementation).
  //!
  //! Notice that \f$\phi \circ \psi = Id_{SO(3)}\f$ whereas \f$\psi \circ \phi = \pm Id_{S^3}\f$.
  void AToEuler(const double33Mat& A,
		double& q1,
		double& q2,
		double& q3,
		double& q4);


  //! Calculate the rotation angles from a matrix, 3,2,1 rotation scheme.
  const double3Vec ATophi321(const double33Mat& A);

  //! Calculate the rotation angles from a matrix, 1,2,3 rotation scheme.
  const double3Vec ATophi123(const double33Mat& A);

  //! Calculate the rotation angles from a matrix.
  //! phiSequence 0 -> A321
  //! phiSequence 1 -> A123
  const double3Vec ATophi(const double33Mat& A, const int phiSequence);


  //! Calculates the time derivate of the Euler parameters q1 ... q4.
  //! The Euler parameters and it's derivatives describe the rotation of the local system (c2) relative the base system (c1).
  //! The angular speed c2_Omega_c1_c1, is the speed of the local system (c2) relative base system (c1) and expressed in base system (c1).
  //! The vector components {x,y,z} corresponds to index {1,2,3}.
  //! If omega is dimensionless, then qdot is that also.
  void qdot(const double q1,
	    const double q2,
	    const double q3,
	    const double q4,
	    const double c2_Omega_c1_c1_1, 
	    const double c2_Omega_c1_c1_2, 
	    const double c2_Omega_c1_c1_3,
	    double& q1dot,
	    double& q2dot,
	    double& q3dot,
	    double& q4dot) ;

  //! Calculates the time derivate of the Euler parameters q1 ... q4.
  //! The Euler parameters and it's derivatives describe the rotation of the local system (c2) relative the base system (c1).
  //! The angular speed c2_Omega_c1_c1, is the speed of the local system (c2) relative base system (c1) and expressed in base system (c1).
  //! The vector components {x,y,z} corresponds to index {1,2,3}.
  //! If omega is dimensionless, then qdot is that also.
  //! Same as qdot(..) above but no validation in debug mode.
  void qdot_unsafe(const double q1,
		   const double q2,
		   const double q3,
		   const double q4,
		   const double c2_Omega_c1_c1_1, 
		   const double c2_Omega_c1_c1_2, 
		   const double c2_Omega_c1_c1_3,
		   double& q1dot,
		   double& q2dot,
		   double& q3dot,
		   double& q4dot) ;

  //! Calculates the time derivate of the Euler parameters q1 ... q4.
  //! The Euler parameters and it's derivatives describe the rotation of the local system (c2) relative the base system (c1).
  //! The angular speed c2_Omega_c1_c2, is the speed of the local system (c2) relative base system (c1) and expressed in local system (c2).
  //! The vector components {x,y,z} corresponds to index {1,2,3}.
  //! Same as qdot(..) above but with Omega expressed in local system (c2).
  void qdotL(const double q1,
	     const double q2,
	     const double q3,
	     const double q4,           
	     const double c2_Omega_c1_c2_1, 
	     const double c2_Omega_c1_c2_2, 
	     const double c2_Omega_c1_c2_3,
	     double& q1dot,
	     double& q2dot,
	     double& q3dot,
	     double& q4dot);

  //! Calculates the second time derivate of the Euler parameters q1 ... q4.
  //! The Euler parameters, its first and second time derivatives describe the rotation of the local system (c2) relative to the base system (c1),
  //! expressed in the base system (c1).
  //! The angular speed c2_Omega_c1_c1, is the speed of the local system (c2) relative base system (c1), expressed in base system (c1).
  //! The angular acceleration c2_vOmega_c1_c1 is the angular acceleation of the local system (c2) relative base system (c1) and expressed in base system (c1).
  //! The vector components {x,y,z} corresponds to index {1,2,3}.
  //! If omega is dimensionless, then qdot and qdotdot are dimensionless as well.
  void qdotdot(const double q1,
	       const double q2,
	       const double q3,
	       const double q4,
	       const double q1dot,
	       const double q2dot,
	       const double q3dot,
	       const double q4dot,
	       const double c2_Omega_c1_c1_1,
	       const double c2_Omega_c1_c1_2,
	       const double c2_Omega_c1_c1_3,
	       const double c2_vOmega_c1_c1_1,
	       const double c2_vOmega_c1_c1_2,
	       const double c2_vOmega_c1_c1_3,
	       double& q1dotdot,
	       double& q2dotdot,
	       double& q3dotdot,
	       double& q4dotdot);

  //! Calculates the second time derivate of the Euler parameters q1 ... q4.
  //! The Euler parameters, its first and second time derivatives describe the rotation of the local system (c2) relative to the base system (c1),
  //! expressed in the local system (c2).
  //! The angular speed c2_Omega_c1_c2, is the speed of the local system (c2) relative base system (c1), expressed in base system (c2).
  //! The angular acceleration c2_vOmega_c1_c2 is the angular acceleation of the local system (c2) relative base system (c1) and expressed in base system (c2).
  //! The vector components {x,y,z} corresponds to index {1,2,3}.
  //! If omega is dimensionless, then qdot and qdotdot are dimensionless as well.
  //! Same as qdotdot(..) above but with Omega expressed in local system (c2).
  void qdotdotL(const double q1,
		const double q2,
		const double q3,
		const double q4,
		const double q1dot,
		const double q2dot,
		const double q3dot,
		const double q4dot,
		const double c2_Omega_c1_c2_1,
		const double c2_Omega_c1_c2_2,
		const double c2_Omega_c1_c2_3,
		const double c2_vOmega_c1_c2_1,
		const double c2_vOmega_c1_c2_2,
		const double c2_vOmega_c1_c2_3,
		double& q1dotdot,
		double& q2dotdot,
		double& q3dotdot,
		double& q4dotdot);

#if 0 
  // Not used. 
  //! Calculates Omega_{c2/c1}^{(c1)} from q_{c2/c1} and q_dot_{c2/c1}. 
  //! Omega = (T^{-1}(q) q_dot
  void omega_q_qdot(const double q[4], const double qdot[4], double c2_Omega_c1_c1[3]);
#endif

  //! Calculates the transpose of the omega_q_qdot ( T^{-T} [a;b;c] )
  //! Used in SubJac.cc 
  //! Need to be documented and verified.
  void omega_q_qdot_T(const double q1 ,const double q2,const double q3,const double q4,
		      const double a,  const double b, const double c, 
		      double &out1, double &out2, double &out3, double &out4);

#if 0 
  // Not used. Need to be documented and verified before use. 
  //! Forms the transformation matrix T in q_dot = T omega
  void FormT(double T[4][3], const double q[4]);

  void FormS(double S[4][4], const double3& omega);

  //! Computes  d(T(q_dot)/dq_dot T^{-1} q_dot + T(Q_dot) T ^{-1}
  void FormSS(double SS[4][4], const double q[4], const double qdot[4]);
#endif

  //! Create a symmetric matrix 
  //! The vector components {x,y,z} corresponds to index {1,2,3}.
  inline const double33Mat Asym(const double axx, 
			     const double axy, 
			     const double axz,
			     const double ayy, 
			     const double ayz, 
			     const double azz);

  //! Create the inverse to a symmetric matrix.
  //! The vector components {x,y,z} corresponds to index {1,2,3}.
  const double33Mat invAsym(const double xx, 
			 const double xy, 
			 const double xz,
			 const double yy, 
			 const double yz, 
			 const double zz);

  //! Create a transformation matrix from base system (c1) to local system (c2), i.e., A_c2/c1.
  //! p^(c2) = A_c2/c1 * p^(c1)
  //! We use 1,2,3 rotation scheme.
  const double33Mat A123(const double3Vec& phi);

  //! Create an inverse transformation matrix from base system to local system.
  //! We use 1,2,3 rotation scheme.
  const double33Mat invA123(const double3Vec& phi);

  //! Create a transformation matrix from base system (c1) to local system (c2), i.e., A_c2/c1.
  //! p^(c2) = A_c2/c1 * p^(c1)
  //! We use 3,2,1 rotation scheme.
  const double33Mat A321(const double3Vec& phi);

  //! Create an inverse (transposed) transformation matrix from base system to local system.
  //! We use 3,2,1 rotation scheme.
  const double33Mat invA321(const double3Vec& phi);

  //! Create a transformation matrix from base system (c1) to local system (c2), i.e., A_c2/c1.
  //! p^(c2) = A_c2/c1 * p^(c1)
  //! phiSequence 0 -> A321
  //! phiSequence 1 -> A123
  const double33Mat A(const double3Vec& phi, const int phiSequence);

  //! Create a transformation matrix from base system (c1) to local system (c2), i.e., A_c2/c1.
  //! p^(c2) = A_c2/c1 * p^(c1)
  //! We use only rotation around the axis number 3 (z-axis).
  const double33Mat A3(const double phi_3);

  //! Create a transformation matrix from base system (c1) to local system (c2), i.e., A_c2/c1.
  //! p^(c2) = A_c2/c1 * p^(c1)
  //! We use only rotation around the axis number 2 (y-axis).
  const double33Mat A2(const double phi_2);

  //! Create a transformation matrix from base system (c1) to local system (c2), i.e., A_c2/c1.
  //! p^(c2) = A_c2/c1 * p^(c1)
  //! We use only rotation around the axis number 1 (x-axis).
  const double33Mat A1(const double phi_1);

  //! Create an "U" transformation matrix.
  //! We use 1,2,3 rotation scheme.
  const double33Mat U123(const double3Vec& phi);

  //! Create an "U" transformation matrix.
  //! We use 3,2,1 rotation scheme.
  const double33Mat U321(const double3Vec& phi);

  //! Create an "H" transformation matrix.
  //! We use 3,2,1 rotation scheme.
  const double33Mat H321(const double3Vec& phi);

  //! Create an "H" transformation matrix.
  //! We use 1,2,3 rotation scheme.
  const double33Mat H123(const double3Vec& phi);

  //! Create an inverse "U" transformation matrix.
  //! We use 1,2,3 rotation scheme.
  const double33Mat invU123(const double3Vec& phi);

  //! Create an inverse "H" transformation matrix.
  //! We use 1,2,3 rotation scheme.
  const double33Mat invH123(const double3Vec& phi);

  //! Create an inverse "H" transformation matrix.
  const double33Mat invH(const double3Vec& phi, const int phiSequence);

  //! Create an inverse "H" transformation matrix.
  //! We use 3,2,1 rotation scheme.
  const double33Mat invH321(const double3Vec& phi);

  //! Create an inverse "U" transformation matrix.
  //! We use 3,2,1 rotation scheme.
  const double33Mat invU321(const double3Vec& phi);


  //! Create a "vU" transformation matrix.
  //! We use 1,2,3 rotation scheme.
  const double33Mat vU123(const double3Vec& phi,
		       const double3Vec& vphi);

  //! Create a "vH" transformation matrix.
  //! We use 1,2,3 rotation scheme.
  const double33Mat vH123(const double3Vec& phi,
		       const double3Vec& vphi);

  //! Create a "vH" transformation matrix.
  //! We use 3,2,1 rotation scheme.
  const double33Mat vH321(const double3Vec& phi,
		       const double3Vec& vphi);

  //! Create a "vU" transformation matrix.
  //! We use 3,2,1 rotation scheme.
  const double33Mat vU321(const double3Vec& phi,
		       const double3Vec& vphi);

  //! Angular speed of the local system relative the base system, i.e. "Omega[cBase]".
  //! The vector is expressed in the local system and we use 1,2,3 rotation scheme, i.e.
  //! "Omega[cBase][this] = U[cBase] . vphi[cBase]"
  const double3Vec Omega_U123(const double3Vec& phi,
			   const double3Vec& vphi);

  //! Angular speed of the local system relative the base system, i.e. "Omega[cBase]".
  //! The vector is expressed in the base system and we use 1,2,3 rotation scheme, i.e.
  //! "Omega[cBase][cBase] = H[cBase] . vphi[cBase]"
  const double3Vec Omega_H123(const double3Vec& phi,
			   const double3Vec& vphi);

  //! Angular acc. of the local system relative the base system. The vector 
  //! is expressed in the local system and we use 1,2,3 rotation scheme. We have: 
  //! "vOmega[cBase][cBase,this] =  vOmega[cBase][this,this] =
  //! vU[cBase] . vphi[cBase] + U[cBase] . aphi[cBase]"
  const double3Vec vOmega_U123(const double3Vec& phi,
			    const double3Vec& vphi,
			    const double3Vec& aphi);

  //! Angular acc. of the local system relative the base system. The vector 
  //! is expressed in the base system and we use 1,2,3 rotation scheme. We have: 
  //! "vOmega[cBase][cBase,cBase] =  vOmega[cBase][this,cBase] =
  //! vH[cBase] . vphi[cBase] + H[cBase] . aphi[cBase]"
  const double3Vec vOmega_H123(const double3Vec& phi,
			    const double3Vec& vphi,
			    const double3Vec& aphi);

  //! Angular speed of the local system relative the base system, i.e. "Omega[cBase]".
  //! The vector is expressed in the base system and we use 3,2,1 rotation scheme, i.e.
  //! "Omega[cBase][cBase] = H[cBase] . vphi[cBase]"
  const double3Vec Omega_H321(const double3Vec& phi,
			   const double3Vec& vphi);

  //! Angular speed of the local system relative the base system, i.e. "Omega[cBase]".
  //! The vector is expressed in the local system and we use 3,2,1 rotation scheme, i.e.
  //! "Omega[cBase][this] = U[cBase] . vphi[cBase]"
  const double3Vec Omega_U321(const double3Vec& phi,
			   const double3Vec& vphi);


  //! Angular acc. of the local system relative the base system. The vector 
  //! is expressed in the local system and we use 3,2,1 rotation scheme. We have: 
  //! "vOmega[cBase][cBase,this] =  vOmega[cBase][this,this] =
  //! vU[cBase] . vphi[cBase] + U[cBase] . aphi[cBase]"
  const double3Vec vOmega_U321(const double3Vec& phi,
			    const double3Vec& vphi,
			    const double3Vec& aphi);

  //! Angular acc. of the local system relative the base system. The vector 
  //! is expressed in the base system and we use 3,2,1 rotation scheme. We have: 
  //! "vOmega[cBase][cBase,cBase] =  vOmega[cBase][this,cBase] =
  //! vH[cBase] . vphi[cBase] + H[cBase] . aphi[cBase]"
  const double3Vec vOmega_H321(const double3Vec& phi,
			    const double3Vec& vphi,
			    const double3Vec& aphi);

  //! Transformation matrix from system cBase to local system.
  //! The local system has the x-axis in radial direction (cylindrical coordinates), i.e.
  //! rotation around the z-axis.
  inline const double33Mat Acyl(const double3Vec& RC);

  //! Transformation matrix from system cBase to local system.
  //! The local system has the x-axis in radial direction (cylindrical coordinates), i.e.
  //! rotation around the z-axis.
  inline const double33Mat Acyl(const double& RC_2);


  //! Transformation from cylindrical coordinates to Cartesian coordinates.
  inline const double3Vec RC2R(const double3Vec& RC);

  //! Transformation from cylindrical coordinates and its derivative to 
  //! velocity in Cartesian coordinates.
  inline const double3Vec vRC2vR(const double3Vec& RC,
			      const double3Vec& vRC);

  //! Transformation from cylindrical coordinates and its 1-st & 2-nd derivative to 
  //! acceleration in Cartesian coordinates.
  const double3Vec aRC2aR(const double3Vec& RC, 
		       const double3Vec& vRC,
		       const double3Vec& aRC);

  //! Transformation from Cartesian coordinates to cylindrical coordinates.
  //! Note that the angle, i.e. component 2, will be within [-pi, pi].
  //! This means the function is not an inverse function to RC2R where the angle
  //! can be larger. An requirement is that both x and y components of R cannot 
  //! be zero at the same time.
  inline const double3Vec R2RC(const double3Vec& R);

  //! Transformation from acc. in Cartesian coordinates to acc. in cylindrical coordinates.
  const double3Vec aR2aRC(const double3Vec& aR,
		       const double3Vec& RC, 
		       const double3Vec& vRC);


  //! Transformation of the elements of a symmetric matrix due to a rotation, i.e.
  //! "A . Asym . invA". The rotation matrix A must Transpose[A] == Inverse[A], i.e.
  //! orthogonal.
  //! Note, x is index 1, y is index 2, and z is index 3.
  void RotSym(const double axx, // Input parameters 
	      const double axy,
	      const double axz,
	      const double ayy,
	      const double ayz,
	      const double azz,
	      const double33Mat& A,
	      double& bxx,     // Output parameters 
	      double& bxy,
	      double& bxz,
	      double& byy,
	      double& byz,
	      double& bzz);



  //! Transform a moment from system c1 to system c2.
  //! With other words we have a force and moment couple acting at the
  //! origin of system c1, then we calculate a the corresponding force and moment
  //! but acting at origin of system c2.
  //! This works independent of what system the vectors are expressed in.
  inline void TFMc1toc2(const double3Vec& F_In,
			const double3Vec& M_c1_In,
			const double3Vec& c2_R_c1_In,
			// Output parameter
			double3Vec& M_c2_Out);


  //! Transform a moment from system c1 to system c2.
  //! With other words we have a force and moment couple acting at the
  //! origin of system c1, then we calculate a the corresponding force and moment
  //! but acting at origin of system c2.
  //! The components are also transformed from system c1 to c2.
  inline void TFMc1toc2(const double3Vec& F_c1_In,
			const double3Vec& M_c1_c1_In,
			const double3Vec& c2_R_c1_c1_In,
			const double33Mat& c2_A_c1_In,
			// Output parameters
			double3Vec& F_c2_Out,
			double3Vec& M_c2_c2_Out);



  //! Transform a moment from system c2 to system c1.
  //! With other words we have a force and moment couple acting at the
  //! origin of system c2, then we calculate a the corresponding force and moment
  //! but acting at origin of system c1.
  //! This works independent of what system the vectors are expressed in.
  inline void TFMc2toc1(const double3Vec& F_In,
			const double3Vec& M_c2_In,
			const double3Vec& c2_R_c1_In,
			// Output parameter
			double3Vec& M_c1_Out);


  //! Transform a moment from system c2 to system c2.
  //! With other words we have a force and moment couple acting at the
  //! origin of system c2, then we calculate a the corresponding force and moment
  //! but acting at origin of system c1.
  //! The components are also transformed from system c2 to c1.
  inline void TFMc2toc1(const double3Vec& F_c2_In,
			const double3Vec& M_c2_c2_In,
			const double3Vec& c2_R_c1_c1_In,
			const double33Mat& c2_A_c1_In,
			// Output parameters
			double3Vec& F_c1_Out,
			double3Vec& M_c1_c1_Out);


  //! Create a skew symmetric matrix. That is Cross(v, x) = Skew(v)*x;
  //! This function logically belongs to lightmat.
  inline void MakeSkewMatrix(const double3Vec& v, double33Mat& m);

  //! Create a skew symmetric matrix. That is Cross(v, x) = Skew(v)*x;
  inline double33Mat Skew(const double3Vec& a);

};

  //! //////////////////////////////////////////////////////
  //!
  //! .SS Inline definitions
  //!
  //! We give the inline definitions in a separate file.
  //!
#include "coordTrans.icc"

#endif // _COORDTRANSFORM_H
