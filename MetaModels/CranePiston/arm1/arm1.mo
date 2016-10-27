model arm1
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false, g = 9.81, n = {0, -1, 0}) annotation(Placement(visible = true, transformation(origin = {6.079, 79.3399}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(angles_fixed = false, angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), r = {0, 0, 1}, r_shape = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {-39, 38}, extent = {{-11, -11}, {11, 11}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(n = {1, 0, 0}, phi(displayUnit = "rad", fixed = true, start = -3.14159265359 / 2), stateSelect = StateSelect.always, useAxisFlange = true) annotation(Placement(visible = true, transformation(origin = {-70, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper1(d = 100, phi_rel(displayUnit = "rad")) annotation(Placement(visible = true, transformation(origin = {-70, 56}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), r = {0, 0, 1}) annotation(Placement(visible = true, transformation(origin = {0, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder2(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), r = {0, 0, 1}) annotation(Placement(visible = true, transformation(origin = {34, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D2(interfaceName = "tlm2") annotation(Placement(visible = true, transformation(origin = {18, -8}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D3(interfaceName = "tlm1") annotation(Placement(visible = true, transformation(origin = {84, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.FixedRotation fixedRotation1(angle = 90, n = {1, 0, 0}, rotationType = Modelica.Mechanics.MultiBody.Types.RotationTypes.RotationAxis) annotation(Placement(visible = true, transformation(origin = {62, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed2(r = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {-104, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed1(r = {0, 0, 0.5}) annotation(Placement(visible = true, transformation(origin = {-18, -28}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  TLM.TLM_Interface_1D_to_3D.TLMInterface1Dto3D tLMInterface1Dto3D1(interfaceName = "tlm1D") annotation(Placement(visible = true, transformation(origin = {-18, 10}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.FixedRotation fixedRotation2(angle = 45, n = {1, 0, 0}, rotationType = Modelica.Mechanics.MultiBody.Types.RotationTypes.RotationAxis) annotation(Placement(visible = true, transformation(origin = {18, 16}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
equation
  connect(fixedRotation2.frame_b, tLMInterface3D2.frame_a) annotation(Line(points = {{18, 6}, {18, 6}, {18, 6}, {18, 6}, {18, 0}, {18, 0}, {18, 0}, {18, 0}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_b, fixedRotation2.frame_a) annotation(Line(points = {{10, 38}, {18, 38}, {18, 26}}, color = {95, 95, 95}));
  connect(fixed1.frame_b, tLMInterface1Dto3D1.frame_b) annotation(Line(points = {{-18, -18}, {-18, 2}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_b, tLMInterface1Dto3D1.frame_a) annotation(Line(points = {{-28, 38}, {-28, 38.25}, {-18, 38.25}, {-18, 17}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_a, revolute1.frame_b) annotation(Line(points = {{-50, 38}, {-60, 38}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_b, bodyCylinder1.frame_a) annotation(Line(points = {{-28, 38}, {-10, 38}}, color = {95, 95, 95}));
  connect(fixedRotation1.frame_a, bodyCylinder2.frame_b) annotation(Line(points = {{52, 38}, {44, 38}}, color = {95, 95, 95}));
  connect(fixedRotation1.frame_b, tLMInterface3D3.frame_a) annotation(Line(points = {{72, 38}, {77, 38}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_b, bodyCylinder2.frame_a) annotation(Line(points = {{10, 38}, {24, 38}}, color = {95, 95, 95}));
  connect(revolute1.frame_a, fixed2.frame_b) annotation(Line(points = {{-80, 38}, {-94, 38}}, color = {95, 95, 95}));
  connect(revolute1.axis, damper1.flange_b) annotation(Line(points = {{-70, 48}, {-60, 48}, {-60, 56}}));
  connect(damper1.flange_a, revolute1.support) annotation(Line(points = {{-80, 56}, {-86, 56}, {-86, 48}, {-76, 48}}));
  annotation(uses(Modelica(version = "3.2.2")));
end arm1;
