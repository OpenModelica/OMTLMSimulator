model sling
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(angles_fixed = false, angles_start(displayUnit = "rad"), animation = false, density(displayUnit = "kg/m3"), diameter = 0.02, r = {0, -1,0}, r_0(fixed = true), r_shape = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {20, 38}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(interfaceName = "tlm1")  annotation(Placement(visible = true, transformation(origin = {-18, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false)  annotation(Placement(visible = true, transformation(origin = {-56, 20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D3(interfaceName = "tlm2")  annotation(Placement(visible = true, transformation(origin = {86, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(bodycylinder1.frame_a, tLMInterface3D1.frame_a) annotation(Line(points = {{8, 38}, {-11, 38}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_b, tLMInterface3D3.frame_a) annotation(Line(points = {{32, 38}, {78, 38}, {78, 38}, {80, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end sling;