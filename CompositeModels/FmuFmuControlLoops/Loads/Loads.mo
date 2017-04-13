model Loads
  FMITLM.FMITLM_Interface_3D.FMITLMInterfaceRotational1D fmi annotation(
    Placement(visible = true, transformation(origin = {-52, -18}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.Rotational.Components.Inertia inertia(J = 1)  annotation(
    Placement(visible = true, transformation(origin = {-12, -18}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper(d = 10)  annotation(
    Placement(visible = true, transformation(origin = {20, -18}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Fixed fixed1 annotation(
    Placement(visible = true, transformation(origin = {48, -18}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  output Real phi;
  input Real tau;
equation
  fmi.tau = tau;
  phi = inertia.flange_b.phi;
  connect(fmi.flange_a, inertia.flange_a) annotation(
    Line(points = {{-46, -18}, {-22, -18}}));
  connect(inertia.flange_b, damper.flange_a) annotation(
    Line(points = {{-2, -18}, {10, -18}}));
  connect(damper.flange_b, fixed1.flange) annotation(
    Line(points = {{30, -18}, {48, -18}}));
  annotation(
    uses(Modelica(version = "3.2.2")));
end Loads;