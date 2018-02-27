model bearing2
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false, g = 9.81, n = {0, -1, 0}) annotation(
    Placement(visible = true, transformation(origin = {6.079, 79.3399}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(debugFlg = true, interfaceName = "tlm2") annotation(
    Placement(visible = true, transformation(origin = {72, 24}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D2(debugFlg = true, interfaceName = "tlm1") annotation(
    Placement(visible = true, transformation(origin = {-62, 22}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(animation = false,n = {1, 0, 0}, stateSelect = StateSelect.default, useAxisFlange = true) annotation(
    Placement(visible = true, transformation(origin = {-8, 24}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape1(animation = false,enforceStates = false,m = 1, r = {0, 0, 0})  annotation(
    Placement(visible = true, transformation(origin = {32, 24}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape2(animation = false, enforceStates = false,m = 1, r = {0, 0, 0}) annotation(
    Placement(visible = true, transformation(origin = {-38, 22}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper1(d = 10)  annotation(
    Placement(visible = true, transformation(origin = {-14, 46}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(revolute1.support, damper1.flange_a) annotation(
    Line(points = {{-14, 34}, {-30, 34}, {-30, 46}, {-24, 46}, {-24, 46}}));
  connect(damper1.flange_b, revolute1.axis) annotation(
    Line(points = {{-4, 46}, {10, 46}, {10, 34}, {-8, 34}, {-8, 34}}));
  connect(bodyShape2.frame_b, revolute1.frame_a) annotation(
    Line(points = {{-28, 22}, {-20, 22}, {-20, 24}, {-18, 24}}, color = {95, 95, 95}));
  connect(tLMInterface3D2.frame_a, bodyShape2.frame_a) annotation(
    Line(points = {{-54, 22}, {-48, 22}}, color = {95, 95, 95}));
  connect(bodyShape1.frame_b, tLMInterface3D1.frame_a) annotation(
    Line(points = {{42, 24}, {66, 24}, {66, 24}, {66, 24}}, color = {95, 95, 95}));
  connect(revolute1.frame_b, bodyShape1.frame_a) annotation(
    Line(points = {{2, 24}, {20, 24}, {20, 26}, {22, 26}, {22, 24}}, color = {95, 95, 95}));
  annotation(
    uses(Modelica(version = "3.2.2")),
    Diagram,
    Icon,
    version = "",
    __OpenModelica_commandLineOptions = "");
end bearing2;
