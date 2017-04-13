model ControlLoops
  Modelica.Blocks.Sources.Sine sine1(freqHz = 0.4)  annotation(
    Placement(visible = true, transformation(origin = {-106, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Sources.Torque torque1 annotation(
    Placement(visible = true, transformation(origin = {-74, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Inertia inertia1(J = 0.5)  annotation(
    Placement(visible = true, transformation(origin = {50, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Inertia inertia2(J = 0.5)  annotation(
    Placement(visible = true, transformation(origin = {-48, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper1(d = 100)  annotation(
    Placement(visible = true, transformation(origin = {78, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Fixed fixed1 annotation(
    Placement(visible = true, transformation(origin = {104, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  Real error(start = 0);
  output Real phi_rad;
  Modelica.Mechanics.Rotational.Components.Spring TLM_spring(c = 10000000)  annotation(
    Placement(visible = true, transformation(origin = {18, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Inertia parasitic_inductance(J = 14) annotation(
    Placement(visible = true, transformation(origin = {-14, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(parasitic_inductance.flange_a, inertia2.flange_b) annotation(
    Line(points = {{-24, -20}, {-38, -20}, {-38, -20}, {-38, -20}}));
  connect(parasitic_inductance.flange_b, TLM_spring.flange_a) annotation(
    Line(points = {{-4, -20}, {8, -20}, {8, -20}, {8, -20}}));
  connect(torque1.flange, inertia2.flange_a) annotation(
    Line(points = {{-64, -20}, {-58, -20}}));
  connect(TLM_spring.flange_b, inertia1.flange_a) annotation(
    Line(points = {{28, -20}, {40, -20}, {40, -20}, {40, -20}}));
  connect(damper1.flange_b, fixed1.flange) annotation(
    Line(points = {{88, -20}, {104, -20}}));
  connect(damper1.flange_a, inertia1.flange_b) annotation(
    Line(points = {{68, -20}, {60, -20}}));
  error = sine1.y - inertia1.phi;
  torque1.tau = error * 10000;
  phi_rad = inertia1.phi;
  annotation(
    Diagram(coordinateSystem(extent = {{-200, -200}, {200, 200}})),
    Icon(coordinateSystem(extent = {{-200, -200}, {200, 200}})),
    version = "",
    uses(Modelica(version = "3.2.2")),
    __OpenModelica_commandLineOptions = "");end ControlLoops;