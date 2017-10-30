model boom
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false, g = 9.81, n = {0, -1, 0}) annotation(
    Placement(visible = true, transformation(origin = {6.079, 79.3399}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(angles_start(displayUnit = "rad"), animation = false, density(displayUnit = "kg/m3"), diameter = 0.2, r = {0, 1, 0}, r_0(fixed = true)) annotation(
    Placement(visible = true, transformation(origin = {0, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder2(angles_start(displayUnit = "rad"), animation = false, density(displayUnit = "kg/m3"), diameter = 0.2, r = {0, 2, 0}) annotation(
    Placement(visible = true, transformation(origin = {34, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(debugFlg = false, interfaceName = "tlm3") annotation(
    Placement(visible = true, transformation(origin = {80, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D2(debugFlg = false, interfaceName = "tlm2") annotation(
    Placement(visible = true, transformation(origin = {18, -74}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute2(animation = false, n = {1, 0, 0}, phi(displayUnit = "rad", fixed = true, start = 4.957367643), stateSelect = StateSelect.always) annotation(
    Placement(visible = true, transformation(origin = {18, -16}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape1(animateSphere = false, animation = false, m = 1, r = {0, 0, 0}, r_CM = {0, 0, 0}) annotation(
    Placement(visible = true, transformation(origin = {18, -48}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation1(animation = false, r = {0, 0, 0.5}) annotation(
    Placement(visible = true, transformation(origin = {18, 14}, extent = {{-10, -10}, {10, 10}}, rotation = -90)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D3(interfaceName = "tlm1") annotation(
    Placement(visible = true, transformation(origin = {-38, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
equation
  connect(bodyCylinder2.frame_b, tLMInterface3D1.frame_a) annotation(
    Line(points = {{44, 38}, {72, 38}, {72, 38}, {74, 38}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_a, tLMInterface3D3.frame_a) annotation(
    Line(points = {{-10, 38}, {-31, 38}}, color = {95, 95, 95}));
  connect(fixedTranslation1.frame_b, revolute2.frame_a) annotation(
    Line(points = {{18, 4}, {18, 4}, {18, -6}, {18, -6}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_b, fixedTranslation1.frame_a) annotation(
    Line(points = {{10, 38}, {16, 38}, {16, 24}, {18, 24}, {18, 24}}, color = {95, 95, 95}));
  connect(revolute2.frame_b, bodyShape1.frame_a) annotation(
    Line(points = {{18, -26}, {18, -38}}, color = {95, 95, 95}));
  connect(tLMInterface3D2.frame_a, bodyShape1.frame_b) annotation(
    Line(points = {{18, -67}, {18, -58}}, color = {95, 95, 95}));
  connect(bodyCylinder1.frame_b, bodyCylinder2.frame_a) annotation(
    Line(points = {{10, 38}, {24, 38}}, color = {95, 95, 95}));
end boom;