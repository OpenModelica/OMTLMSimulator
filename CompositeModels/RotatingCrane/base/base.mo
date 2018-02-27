model base
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed2(animation = false, r = {0, 0, 0}) annotation(
    Placement(visible = true, transformation(origin = {-104, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(debugFlg = false, interfaceName = "tlm1") annotation(
    Placement(visible = true, transformation(origin = {38, 40}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(animation = false, n = {0, 1, 0}, stateSelect = StateSelect.never, useAxisFlange = true) annotation(
    Placement(visible = true, transformation(origin = {-38, 40}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_1D.TLMTorque1D tlm3(debugFlg = true, interfaceName = "tlm3") annotation(
    Placement(visible = true, transformation(origin = {-38, 80}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false) annotation(
    Placement(visible = true, transformation(origin = {28, 82}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape1(animateSphere = false, animation = false, m = 1, r = {0, 0, 0}, r_CM = {0, 0, 0}) annotation(
    Placement(visible = true, transformation(origin = {-2, 40}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(revolute1.frame_a, fixed2.frame_b) annotation(
    Line(points = {{-48, 40}, {-94, 40}, {-94, 38}, {-94, 38}}, color = {95, 95, 95}));
  connect(bodyShape1.frame_b, tLMInterface3D1.frame_a) annotation(
    Line(points = {{8, 40}, {30, 40}, {30, 40}, {32, 40}}, color = {95, 95, 95}));
  connect(revolute1.frame_b, bodyShape1.frame_a) annotation(
    Line(points = {{-28, 40}, {-12, 40}}, color = {95, 95, 95}));
  connect(revolute1.axis, tlm3.flange_b) annotation(
    Line(points = {{-38, 50}, {-38, 72}}));
end base;