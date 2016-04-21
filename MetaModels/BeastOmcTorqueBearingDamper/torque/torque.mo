model torque
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), diameter = 0.2, enforceStates = true, innerDiameter = 0.1, r = {0, 0, 1}) annotation(Placement(visible = true, transformation(origin = {-8, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1 annotation(Placement(visible = true, transformation(origin = {-50, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.MultiBody.Forces.WorldTorque torque annotation(Placement(visible = true, transformation(origin = {26, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Blocks.Sources.Constant const(k = 0)  annotation(Placement(visible = true, transformation(origin = {34, 26}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 9.81)  annotation(Placement(visible = true, transformation(origin = {-40, 72}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Sources.Constant const1(k = 0)  annotation(Placement(visible = true, transformation(origin = {32, 48}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Sources.Constant const2(k = -100)  annotation(Placement(visible = true, transformation(origin = {58, 46}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(const2.y, torque.torque[3]) annotation(Line(points = {{69, 46}, {76, 46}, {76, 2}, {38, 2}}, color = {0, 0, 127}));
  connect(const1.y, torque.torque[2]) annotation(Line(points = {{43, 48}, {66, 48}, {66, 2}, {38, 2}}, color = {0, 0, 127}));
  connect(bodyCylinder1.frame_a, tLMInterface3D1.frame_a) annotation(Line(points = {{-18, 2}, {-43, 2}}, color = {95, 95, 95}));
  connect(const.y, torque.torque[1]) annotation(Line(points = {{46, 26}, {56, 26}, {56, 2}, {38, 2}, {38, 2}}, color = {0, 0, 127}));
  connect(bodyCylinder1.frame_b, torque.frame_b) annotation(Line(points = {{2, 2}, {16, 2}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.1")));
end torque;