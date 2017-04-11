model RotationalSpringDamper2
  Modelica.Mechanics.Rotational.Components.Fixed fixed2 annotation(
    Placement(visible = true, transformation(origin = {36, -4}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  Modelica.Mechanics.Rotational.Components.SpringDamper springDamper1(c = 150, d = 10)  annotation(
    Placement(visible = true, transformation(origin = {16, -4}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Inertia inertia1(J = 1)  annotation(
    Placement(visible = true, transformation(origin = {-14, -4}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_1D.TLMTorque1D tLMTorque1D1 annotation(
    Placement(visible = true, transformation(origin = {-46, -4}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
equation
  connect(inertia1.flange_a, tLMTorque1D1.flange_b) annotation(
    Line(points = {{-24, -4}, {-38, -4}}));
  connect(springDamper1.flange_a, inertia1.flange_b) annotation(
    Line(points = {{6, -4}, {-4, -4}}));
  connect(springDamper1.flange_b, fixed2.flange) annotation(
    Line(points = {{26, -4}, {36, -4}}));
  annotation(
    uses(Modelica(version = "3.2.1")));
end RotationalSpringDamper2;