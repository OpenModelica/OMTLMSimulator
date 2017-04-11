model SpringDamper1
  Modelica.Mechanics.Translational.Components.Mass mass1(m = 10)  annotation(
    Placement(visible = true, transformation(origin = {38, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Translational.Components.SpringDamper springDamper1(c = 100, d = 10)  annotation(
    Placement(visible = true, transformation(origin = {10, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Translational.Sources.Force force1 annotation(
    Placement(visible = true, transformation(origin = {-26, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Sources.Step step1(height = 100, startTime = 0.5)  annotation(
    Placement(visible = true, transformation(origin = {-58, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_1D.TLMForce1D tLMForce1D1(debugFlg = true)  annotation(
    Placement(visible = true, transformation(origin = {62, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(mass1.flange_b, tLMForce1D1.flange_b) annotation(
    Line(points = {{48, 2}, {54, 2}}, color = {0, 127, 0}));
  connect(force1.f, step1.y) annotation(
    Line(points = {{-38, 2}, {-47, 2}}, color = {0, 0, 127}));
  connect(springDamper1.flange_a, force1.flange) annotation(
    Line(points = {{0, 2}, {-16, 2}}, color = {0, 127, 0}));
  connect(springDamper1.flange_b, mass1.flange_a) annotation(
    Line(points = {{20, 2}, {28, 2}}, color = {0, 127, 0}));
  annotation(
    uses(Modelica(version = "3.2.2")));
end SpringDamper1;