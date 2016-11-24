model rope
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(angles_fixed = false, angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), diameter = 0.02, r = {0, -1,0}, r_0(fixed = true), r_shape = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {20, 38}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(enableAnimation = false)  annotation(Placement(visible = true, transformation(origin = {-56, 20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  FMITLM.FMITLM_Interface_3D.FMITLMInterface3D fMITLMInterface3D1 annotation(Placement(visible = true, transformation(origin = {-20, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  FMITLM.FMITLM_Interface_3D.FMITLMInterface3D fMITLMInterface3D2 annotation(Placement(visible = true, transformation(origin = {74, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  
  input Real f1[3](start = zeros(3));
  input Real t1[3](start = zeros(3));
  input Real f2[3](start = zeros(3));
  input Real t2[3](start = zeros(3));
equation
  fMITLMInterface3D1.f = f1;
  fMITLMInterface3D1.t = t1;
  fMITLMInterface3D2.f = f2;
  fMITLMInterface3D2.t = t2;
  
  connect(bodycylinder1.frame_b, fMITLMInterface3D2.frame_a) annotation(Line(points = {{32, 38}, {66, 38}, {66, 38}, {68, 38}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_a, fMITLMInterface3D1.frame_a) annotation(Line(points = {{8, 38}, {-13, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end rope;
