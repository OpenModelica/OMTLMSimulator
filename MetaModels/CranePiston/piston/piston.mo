model piston
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false, g = 0, n = {0, 0, -1}) annotation(Placement(visible = true, transformation(origin = {-65.9212, -10.6601}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  TLM.TLM_Interface_1D_to_3D.TLMInterface1Dto3D TLM1D(interfaceName = "tlm1D") annotation(Placement(visible = true, transformation(origin = {16, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D TLM1(interfaceName = "tlm1") annotation(Placement(visible = true, transformation(origin = {-66, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(interfaceName = "tlm2") annotation(Placement(visible = true, transformation(origin = {104, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape1(m = 10, r = {0, 0, 0}, r_CM = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {-38, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape2(m = 10, r = {0, 0, 0}, r_0(fixed = false), r_CM = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {78, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic1(n = {0, 0, 1}, s(fixed = true, start = 2.23606797749979))  annotation(Placement(visible = true, transformation(origin = {14, 66}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(prismatic1.frame_b, bodyShape2.frame_a) annotation(Line(points = {{24, 66}, {42, 66}, {42, 38}, {68, 38}, {68, 38}}, color = {95, 95, 95}));
  connect(bodyShape1.frame_b, prismatic1.frame_a) annotation(Line(points = {{-28, 38}, {-12, 38}, {-12, 68}, {4, 68}, {4, 66}}, color = {95, 95, 95}));
  connect(TLM1D.frame_b, bodyShape2.frame_a) annotation(Line(points = {{24, 38}, {68, 38}, {68, 38}, {68, 38}}, color = {95, 95, 95}));
  connect(TLM1D.frame_a, bodyShape1.frame_b) annotation(Line(points = {{10, 38}, {-28, 38}, {-28, 38}, {-28, 38}}, color = {95, 95, 95}));
  connect(bodyShape2.frame_b, tLMInterface3D1.frame_a) annotation(Line(points = {{88, 38}, {96, 38}, {96, 38}, {98, 38}}, color = {95, 95, 95}));
  connect(TLM1.frame_a, bodyShape1.frame_a) annotation(Line(points = {{-58, 38}, {-48, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end piston;
