model arm2
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(angles_fixed = false, angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), r = {0, 0, 1}, r_shape = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {20, 38}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), r = {0, 0, 2}) annotation(Placement(visible = true, transformation(origin = {58, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(interfaceName = "tlm1")  annotation(Placement(visible = true, transformation(origin = {-18, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D2(interfaceName = "tlm3") annotation(Placement(visible = true, transformation(origin = {40, -6}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  inner Modelica.Mechanics.MultiBody.World world annotation(Placement(visible = true, transformation(origin = {-56, 20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.FixedRotation fixedRotation1(angle = -45)  annotation(Placement(visible = true, transformation(origin = {40, 22}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.Body body1(m = 10)  annotation(Placement(visible = true, transformation(origin = {88, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(bodyCylinder1.frame_b, body1.frame_a) annotation(Line(points = {{68, 38}, {78, 38}, {78, 38}, {78, 38}}, color = {95, 95, 95}));
  connect(fixedRotation1.frame_b, tLMInterface3D2.frame_a) annotation(Line(points = {{40, 12}, {40, 12}, {40, 2}, {40, 2}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_b, fixedRotation1.frame_a) annotation(Line(points = {{32, 38}, {40, 38}, {40, 32}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_a, tLMInterface3D1.frame_a) annotation(Line(points = {{8, 38}, {-11, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end arm2;
