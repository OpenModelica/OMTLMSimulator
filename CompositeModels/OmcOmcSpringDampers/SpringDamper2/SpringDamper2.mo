model SpringDamper2
  Modelica.Mechanics.Translational.Components.Mass mass1(m = 10) annotation(
    Placement(visible = true, transformation(origin = {-16, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Translational.Components.SpringDamper springDamper1(c = 150, d = 10) annotation(
    Placement(visible = true, transformation(origin = {10, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_1D.TLMForce1D tLMForce1D1 annotation(
    Placement(visible = true, transformation(origin = {-48, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.Translational.Components.Fixed fixed1 annotation(
    Placement(visible = true, transformation(origin = {48, 2}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
equation
  connect(springDamper1.flange_b, fixed1.flange) annotation(
    Line(points = {{20, 2}, {48, 2}}, color = {0, 127, 0}));
  connect(mass1.flange_a, tLMForce1D1.flange_b) annotation(
    Line(points = {{-26, 2}, {-40, 2}}, color = {0, 127, 0}));
  connect(mass1.flange_b, springDamper1.flange_a) annotation(
    Line(points = {{-6, 2}, {0, 2}, {0, 2}, {0, 2}}, color = {0, 127, 0}));
  annotation(
    uses(Modelica(version = "3.2.2")));
end SpringDamper2;