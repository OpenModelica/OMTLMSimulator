model Rotor "Ridig rotor with possibility to use GDW, BEM, Betz, Schmitz.
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
  OneWind.Environment.Wind.WindConstant wind(final numberPositions = numberPositions, final nacelleTiltAngle = nacelleTiltAngle, final nTElements = 1, final hubHeight = 0, final distanceHub2Shaft = 0, final overHang = 0, final radiusTowerElements = {0}) "Instance of wind model" annotation(Placement(visible = true, transformation(extent = {{-82, -74}, {-62, -54}}, rotation = 0)));
  Rotor rotor(animation = false, initialPitchAngle(displayUnit = "rad"), nacelleTiltAngle = nacelleTiltAngle, rhoFluid(displayUnit = "kg/m3") = rhoFluid) "Instance of rotor model" annotation(Placement(transformation(extent = {{-70, -10}, {-50, 10}})));
  Modelica.Blocks.Sources.Constant fixedPitchAngle(final k = 0) " constant source for pitch angle [deg]" annotation(Placement(transformation(extent = {{-92, 78}, {-72, 98}})));
  Modelica.Blocks.Math.UnitConversions.From_deg from_deg "to convert pitch angle from deg to rad" annotation(Placement(transformation(extent = {{-58, 76}, {-38, 96}})));
  inner Modelica.Mechanics.MultiBody.World world(enableAnimation = false, label2 = "z", n = {0, 0, -1}) annotation(Placement(visible = true, transformation(origin = {44, 0}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), animation = false, density(displayUnit = "kg/m3"), enforceStates = true) annotation(Placement(visible = true, transformation(origin = {-22, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(phi(displayUnit = "rad"), stateSelect = StateSelect.always, useAxisFlange = true)  annotation(Placement(visible = true, transformation(origin = {10, 0}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute2(phi(displayUnit = "rad"), stateSelect = StateSelect.always, useAxisFlange = true)  annotation(Placement(visible = true, transformation(origin = {24, 36}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed1 annotation(Placement(visible = true, transformation(origin = {-6, 36}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder2(angles_start(displayUnit = "rad"), animation = false, density(displayUnit = "kg/m3"), enforceStates = true) annotation(Placement(visible = true, transformation(origin = {54, 36}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
protected
  final parameter Modelica.SIunits.Conversions.NonSIunits.Angle_deg nacelleTiltAngle = 0.0 "Current yaw angle (yaw misalignment), [deg]";
  final parameter Integer numberPositions = size(rotor.cWind, 1) "Number of connectors for wind. Please add +1 for the hub. no.Blades*no.BladeElements+1";
  final parameter Boolean animation = false "Animate rotor, true or false";
protected
  final parameter Modelica.SIunits.Density rhoFluid = wind.rho "fluid density for drag calculation, [kg/m^3]";
protected
  //============================================================================================================
  //-- Internal Interfaces -------------------------------------------------------------------------------------
  OneWind.Interfaces.SignalBusInterfaces.RotorBus rotorBus "Connection to bus system between OperatingControl, Nacelle and Rotor." annotation(Placement(transformation(extent = {{-10, 80}, {10, 100}}), iconTransformation(extent = {{-36, 72}, {-4, 104}})));
equation
  connect(revolute1.frame_b, bodyCylinder1.frame_b) annotation(Line(points = {{20, 0}, {-12, 0}}, color = {95, 95, 95}));
  connect(revolute2.frame_a, bodyCylinder2.frame_a) annotation(Line(points = {{34, 36}, {44, 36}, {44, 36}, {44, 36}}, color = {95, 95, 95}));
  connect(revolute2.frame_b, fixed1.frame_b) annotation(Line(points = {{14, 36}, {4, 36}}, color = {95, 95, 95}));
  connect(revolute1.axis, revolute2.axis) annotation(Line(points = {{10, 10}, {10, 10}, {10, 20}, {24, 20}, {24, 26}, {24, 26}}));
  connect(world.frame_b, revolute1.frame_a) annotation(Line(points = {{34, 0}, {20, 0}}, color = {95, 95, 95}));
  connect(wind.cBladeWind, rotor.cWind) annotation(Line(points = {{-82, -56}, {-71, -56}, {-71, 0}, {-70, 0}}));
  connect(from_deg.y, rotorBus.referencePitchAngle) annotation(Line(points = {{-37, 80}, {-22.5, 80}, {-22.5, 40}, {-78, 40}}, color = {0, 0, 127}));
  connect(rotorBus, rotor.rotorBus) annotation(Line(points = {{-78, 40}, {-78, 23.4}, {-62, 23.4}, {-62, 8.8}}, color = {255, 204, 51}, thickness = 0.5));
  connect(bodyCylinder1.frame_a, rotor.frame_a) annotation(Line(points = {{-32, 0}, {-50, 0}, {-50, 0}, {-50, 0}}, color = {95, 95, 95}));
//==============================================
// connection wind - rotor
//==============================================
//==============================================
//------------------------------------------
//--------------------------------------------
  connect(fixedPitchAngle.y, from_deg.u) annotation(Line(points = {{-71, 88}, {-66, 88}, {-66, 86}, {-60, 86}}, color = {0, 0, 127}, smooth = Smooth.None));
  annotation(Placement(transformation(extent = {{66, -24}, {86, -4}})), Placement(transformation(extent = {{-16, -10}, {4, 10}})), Diagram(graphics), Documentation(info = "<html>Rotor test model with constant angular velocity. Purpose is to check aerodynamic.</html>"), uses(Modelica(version = "3.2.1")));
end Rotor;