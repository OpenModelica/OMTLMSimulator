model arm2
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(angles_fixed = false, angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), r = {0, 0, 1}, r_0(fixed = true), r_shape = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {20, 38}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), r = {0, 0, 2}) annotation(Placement(visible = true, transformation(origin = {58, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(interfaceName = "tlm1")  annotation(Placement(visible = true, transformation(origin = {-18, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D2(interfaceName = "tlm3") annotation(Placement(visible = true, transformation(origin = {28, -26}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  inner Modelica.Mechanics.MultiBody.World world annotation(Placement(visible = true, transformation(origin = {-56, 20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.FixedRotation fixedRotation1(angle = -63.434948822922)  annotation(Placement(visible = true, transformation(origin = {28, 2}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.FixedRotation fixedRotation2(angle = 26.565051177078) annotation(Placement(visible = true, transformation(origin = {56, 4}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D3(interfaceName = "tlm4")  annotation(Placement(visible = true, transformation(origin = {56, -26}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D4(interfaceName = "tlm2")  annotation(Placement(visible = true, transformation(origin = {90, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(bodyCylinder1.frame_b, tLMInterface3D4.frame_a) annotation(Line(points = {{68, 38}, {82, 38}, {82, 38}, {84, 38}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_b, fixedRotation2.frame_a) annotation(Line(points = {{32, 38}, {44, 38}, {44, 26}, {56, 26}, {56, 14}}, color = {95, 95, 95}));
  connect(fixedRotation2.frame_b, tLMInterface3D3.frame_a) annotation(Line(points = {{56, -6}, {56, -19}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_b, fixedRotation1.frame_a) annotation(Line(points = {{32, 38}, {38, 38}, {38, 26}, {28, 26}, {28, 12}}, color = {95, 95, 95}));
  connect(fixedRotation1.frame_b, tLMInterface3D2.frame_a) annotation(Line(points = {{28, -8}, {28, -8}, {28, -18}, {28, -18}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_a, bodycylinder1.frame_b) annotation(Line(points = {{48, 38}, {32, 38}, {32, 38}, {32, 38}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_a, tLMInterface3D1.frame_a) annotation(Line(points = {{8, 38}, {-11, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end arm2;
