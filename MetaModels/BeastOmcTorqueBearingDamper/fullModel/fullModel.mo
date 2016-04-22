model fullModel
  Modelica.Mechanics.MultiBody.Forces.WorldTorque torque annotation(Placement(visible = true, transformation(origin = {26, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Blocks.Sources.Constant const2(k = -100) annotation(Placement(visible = true, transformation(origin = {58, 46}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Sources.Constant const annotation(Placement(visible = true, transformation(origin = {34, 26}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Sources.Constant const1 annotation(Placement(visible = true, transformation(origin = {32, 48}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder2(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), diameter = 0.2, innerDiameter = 0.1) annotation(Placement(visible = true, transformation(origin = {-82, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(phi(displayUnit = "rad"), stateSelect = StateSelect.always, useAxisFlange = true)  annotation(Placement(visible = true, transformation(origin = {-116, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 0) annotation(Placement(visible = true, transformation(origin = {-158, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper1(d = 1, phi_rel(displayUnit = "rad")) annotation(Placement(visible = true, transformation(origin = {-102, 34}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.Rotational.Components.Fixed fixed2 annotation(Placement(visible = true, transformation(origin = {-126, 52}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.Rotational.Components.Fixed fixed3 annotation(Placement(visible = true, transformation(origin = {-102, 54}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.Rotational.Components.Spring spring1(c = 100000, phi_rel(displayUnit = "rad"), phi_rel0(displayUnit = "rad"))  annotation(Placement(visible = true, transformation(origin = {-124, 32}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), diameter = 0.2, innerDiameter = 0.1) annotation(Placement(visible = true, transformation(origin = {-32, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(torque.frame_b, bodyCylinder1.frame_b) annotation(Line(points = {{16, 2}, {-20, 2}, {-20, 2}, {-22, 2}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_a, bodyCylinder2.frame_b) annotation(Line(points = {{-42, 2}, {-72, 2}, {-72, 2}, {-72, 2}}, color = {95, 95, 95}));
  connect(fixed3.flange, damper1.flange_a) annotation(Line(points = {{-102, 54}, {-102, 54}, {-102, 44}, {-102, 44}}));
  connect(revolute1.axis, damper1.flange_b) annotation(Line(points = {{-116, 12}, {-102, 12}, {-102, 24}, {-102, 24}}));
  connect(spring1.flange_b, fixed2.flange) annotation(Line(points = {{-124, 42}, {-126, 42}, {-126, 52}, {-126, 52}, {-126, 52}}));
  connect(revolute1.support, spring1.flange_a) annotation(Line(points = {{-122, 12}, {-124, 12}, {-124, 22}, {-124, 22}}));
  connect(world.frame_b, revolute1.frame_a) annotation(Line(points = {{-148, 2}, {-126, 2}}, color = {95, 95, 95}));
  connect(revolute1.frame_b, bodyCylinder2.frame_a) annotation(Line(points = {{-106, 2}, {-92, 2}, {-92, 2}, {-92, 2}}, color = {95, 95, 95}));
  connect(const1.y, torque.torque[2]) annotation(Line(points = {{43, 48}, {66, 48}, {66, 2}, {38, 2}}, color = {0, 0, 127}));
  connect(const.y, torque.torque[1]) annotation(Line(points = {{46, 26}, {56, 26}, {56, 2}, {38, 2}, {38, 2}}, color = {0, 0, 127}));
  connect(const2.y, torque.torque[3]) annotation(Line(points = {{69, 46}, {76, 46}, {76, 2}, {38, 2}}, color = {0, 0, 127}));
  annotation(uses(Modelica(version = "3.2.1")), Diagram(coordinateSystem(extent = {{-1000, -1000}, {1000, 1000}})), Icon(coordinateSystem(extent = {{-1000, -1000}, {1000, 1000}})), version = "");
end fullModel;