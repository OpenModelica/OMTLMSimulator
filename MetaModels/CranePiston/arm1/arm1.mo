model arm1
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false, g = 9.81, n = {0, -1, 0}) annotation(Placement(visible = true, transformation(origin = {6.079, 79.3399}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), diameter = 0.2, r = {0, 0, 1}) annotation(Placement(visible = true, transformation(origin = {0, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder2(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), diameter = 0.2, r = {0, 0, 2}) annotation(Placement(visible = true, transformation(origin = {34, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed2(r = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {-104, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.FixedRotation fixedRotation3(angle = -90)  annotation(Placement(visible = true, transformation(origin = {-74, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(debugFlg = false, interfaceName = "tlm1")  annotation(Placement(visible = true, transformation(origin = {80, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D2(debugFlg = false, interfaceName = "tlm2") annotation(Placement(visible = true, transformation(origin = {18, -44}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute2(n = {1, 0, 0}, phi(displayUnit = "rad",fixed = true, start = 0.463647609000806), stateSelect = StateSelect.always) annotation(Placement(visible = true, transformation(origin = {18,14}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape1(m = 1, r = {0, 0, 0}, r_CM = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {18, -18}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.FixedRotation fixedRotation1(angle = 90) annotation(Placement(visible = true, transformation(origin = {58, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(tLMInterface3D1.frame_a, fixedRotation1.frame_b) annotation(Line(points = {{74, 38}, {68, 38}, {68, 38}, {68, 38}}, color = {95, 95, 95}));
  connect(bodyCylinder2.frame_b, fixedRotation1.frame_a) annotation(Line(points = {{44, 38}, {48, 38}}, color = {95, 95, 95}));
  connect(tLMInterface3D2.frame_a, bodyShape1.frame_b) annotation(Line(points = {{18, -37}, {18, -28}}, color = {95, 95, 95}));
  connect(revolute2.frame_b, bodyShape1.frame_a) annotation(Line(points = {{18, 4}, {18, -8}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_b, revolute2.frame_a) annotation(Line(points = {{10, 38}, {18, 38}, {18, 24}, {18, 24}}, color = {95, 95, 95}));
  connect(fixed2.frame_b, fixedRotation3.frame_a) annotation(Line(points = {{-94, 38}, {-84, 38}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_a, fixedRotation3.frame_b) annotation(Line(points = {{-10, 38}, {-64, 38}, {-64, 38}, {-64, 38}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_b, bodyCylinder2.frame_a) annotation(Line(points = {{10, 38}, {24, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end arm1;
