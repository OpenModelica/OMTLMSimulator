model damper
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), diameter = 0.2, enforceStates = false, innerDiameter = 0.1, r = {0, 0, 1}) annotation(Placement(visible = true, transformation(origin = {-10, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(phi(displayUnit = "rad"), stateSelect = StateSelect.always, useAxisFlange = true) annotation(Placement(visible = true, transformation(origin = {-64, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 0) annotation(Placement(visible = true, transformation(origin = {-106, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper1(d = 1, phi_rel(displayUnit = "rad"), stateSelect = StateSelect.always) annotation(Placement(visible = true, transformation(origin = {-44, 26}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.Rotational.Components.Fixed fixed2 annotation(Placement(visible = true, transformation(origin = {-88, 44}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.Rotational.Components.Fixed fixed3 annotation(Placement(visible = true, transformation(origin = {-20, 44}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.Rotational.Components.Spring spring1(c = 100000, phi_rel(displayUnit = "rad"), phi_rel0(displayUnit = "rad"))  annotation(Placement(visible = true, transformation(origin = {-84, 28}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1 annotation(Placement(visible = true, transformation(origin = {22, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(revolute1.frame_b, bodyCylinder1.frame_a) annotation(Line(points = {{-54, 2}, {-54, 1}, {-24, 1}, {-24, 1.5}, {-20, 1.5}, {-20, 2}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_b, tLMInterface3D1.frame_a) annotation(Line(points = {{0, 2}, {15, 2}}, color = {95, 95, 95}));
  connect(revolute1.axis, damper1.flange_b) annotation(Line(points = {{-64, 12}, {-64, 26}, {-54, 26}}));
  connect(revolute1.support, spring1.flange_a) annotation(Line(points = {{-70, 12}, {-84, 12}, {-84, 18}}));
  connect(revolute1.frame_a, world.frame_b) annotation(Line(points = {{-74, 2}, {-96, 2}}, color = {95, 95, 95}));
  connect(spring1.flange_b, fixed2.flange) annotation(Line(points = {{-84, 38}, {-88, 38}, {-88, 44}, {-88, 44}}));
  connect(fixed3.flange, damper1.flange_a) annotation(Line(points = {{-20, 44}, {-20, 26}, {-34, 26}}));
  annotation(uses(Modelica(version = "3.2.1")));
end damper;
