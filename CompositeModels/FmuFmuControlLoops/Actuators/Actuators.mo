model Actuators
  FMITLM.FMITLM_Interface_3D.FMITLMInterfaceRotational1D fmi annotation(
    Placement(visible = true, transformation(origin = {70, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Sources.Torque torque annotation(
    Placement(visible = true, transformation(origin = {12, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  parameter Real Kp_phi = 0.01;
  input Real phi_ref;
  input Real phi;
  input Real tau;
  Modelica.Mechanics.Rotational.Components.Inertia inertia(J = 0.1)  annotation(
    Placement(visible = true, transformation(origin = {38, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(inertia.flange_b, fmi.flange_a) annotation(
    Line(points = {{48, -20}, {63, -20}}));
  connect(torque.flange, inertia.flange_a) annotation(
    Line(points = {{22, -20}, {28, -20}, {28, -20}, {28, -20}}));
  fmi.tau = tau;
  torque.tau = (phi_ref - phi) * Kp_phi;
  annotation(
    uses(Modelica(version = "3.2.2")));
end Actuators;