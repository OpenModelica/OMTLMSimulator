model Nacelle "Ridig rotor with possibility to use GDW, BEM, Betz, Schmitz.
For BEM, Betz, Schmitz: modify loadelement inside blade.
For GDW modify: loadelement inside blade and boolean inside bemcontroldata.
For DynamicStall modify loadelement and profile definition, set dsModel = true; see TestRigidRotorWithDS"
  /*
       * This computer program is property of the Fraunhofer-
       * Gesellschaft zur Foerderung der angewandten Forschung e.V. 
       * You as a licensee can only use the computer program if you 
       * agree with the "OneWind Open Source Software License 
       * Agreement" and only in accordance with the conditions of 
       * this "OneWind Open Source Software License Agreement" or if 
       * you own a commercial license.
       */
  import OneWind.Components.Rotor.Rotor;
  import OneWind.Environment.Wind.WindConstant;
  //  	import OneWind.Environment.Wind.WindTurbulent;
  import nSI = Modelica.SIunits.Conversions.NonSIunits;
  inner Modelica.Mechanics.MultiBody.World world(final n = {0, 0, -1}, label2 = "z") "Instance of world model with gravity and reference ccordinate system" annotation(Placement(transformation(extent = {{34, 6}, {54, 26}})));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute(animation = false, n = {1, 0, 0}, phi(displayUnit = "rad"), stateSelect = StateSelect.always, useAxisFlange = true) "revolute joint with two frames (one fixed!) and a flange. to convert rotation from flange to frame." annotation(Placement(visible = true, transformation(extent = {{-8, -10}, {-28, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Sources.Speed speed(phi(displayUnit = "rad"))  "transforms input rotation speed in rotation of a flange" annotation(Placement(transformation(extent = {{20, 30}, {0, 50}})));
  Modelica.Blocks.Math.UnitConversions.From_rpm from_rpm "converts rotation speed from 1/min to rad/s" annotation(Placement(transformation(extent = {{68, 38}, {48, 58}})));
  Modelica.Blocks.Sources.Constant constRotSpeed(final k = 9.0) "constant rotation speed [1/min]" annotation(Placement(transformation(extent = {{100, 38}, {80, 58}})));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1 annotation(Placement(visible = true, transformation(origin = {-82, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), animation = false, density(displayUnit = "kg/m3"), enforceStates = true) annotation(Placement(visible = true, transformation(origin = {-52, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
protected
  final parameter Modelica.SIunits.Conversions.NonSIunits.Angle_deg nacelleTiltAngle = 0.0 "Current yaw angle (yaw misalignment), [deg]";
  final parameter Boolean animation = false "Animate rotor, true or false";
  //============================================================================================================
  //-- Internal Interfaces -------------------------------------------------------------------------------------
equation
  connect(revolute.frame_b, bodyCylinder1.frame_b) annotation(Line(points = {{-28, 0}, {-42, 0}}, color = {95, 95, 95}));
  connect(speed.flange, revolute.axis) annotation(Line(points = {{-5.55112e-16, 40}, {-10, 40}, {-10, 34}, {-18, 34}, {-18, 10}}));
  connect(revolute.frame_a, world.frame_b) annotation(Line(points = {{-8, 0}, {26, 0}, {26, -8}, {72, -8}, {72, 16}, {54, 16}}, color = {95, 95, 95}, thickness = 0.5));
  connect(tLMInterface3D1.frame_a, bodyCylinder1.frame_a) annotation(Line(points = {{-75, 0}, {-62, 0}}, color = {95, 95, 95}));
  connect(constRotSpeed.y, from_rpm.u) annotation(Line(points = {{79, 48}, {70, 48}}, color = {0, 0, 127}));
//==============================================
// connection wind - rotor
//==============================================
//==============================================
//------------------------------------------
//--------------------------------------------
  connect(from_rpm.y, speed.w_ref) annotation(Line(points = {{47, 48}, {36, 48}, {36, 40}, {22, 40}}, color = {0, 0, 127}, smooth = Smooth.None));
  annotation(Placement(transformation(extent = {{66, -24}, {86, -4}})), Placement(transformation(extent = {{-16, -10}, {4, 10}})), Diagram(graphics), Documentation(info = "<html>Rotor test model with constant angular velocity. Purpose is to check aerodynamic.</html>"), uses(Modelica(version = "3.2.1")));
end Nacelle;