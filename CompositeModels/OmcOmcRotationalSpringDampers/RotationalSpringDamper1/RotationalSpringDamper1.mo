model RotationalSpringDamper1
  Modelica.Mechanics.Rotational.Components.SpringDamper springDamper1(c = 100, d = 10) annotation(
    Placement(visible = true, transformation(origin = {-34, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.Rotational.Components.Inertia inertia1(J = 1) annotation(
    Placement(visible = true, transformation(origin = {0, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  TLM.TLM_Interface_1D.TLMTorque1D tLMTorque1D1 annotation(
    Placement(visible = true, transformation(origin = {34, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Sources.TorqueStep torqueStep1(offsetTorque = 0, startTime = 0.5, stepTorque = 100) annotation(
    Placement(visible = true, transformation(origin = {-70, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(springDamper1.flange_b, torqueStep1.flange) annotation(
    Line(points = {{-44, 0}, {-60, 0}}));
  connect(inertia1.flange_a, tLMTorque1D1.flange_b) annotation(
    Line(points = {{10, 0}, {26, 0}}));
  connect(springDamper1.flange_a, inertia1.flange_b) annotation(
    Line(points = {{-24, 0}, {-10, 0}}));
  annotation(
    uses(Modelica(version = "3.2.1")));
end RotationalSpringDamper1;