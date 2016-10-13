model crane
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false, g = 0, n = {0, 0, -1}) annotation(Placement(visible = true, transformation(origin = {-71.9212, 37.3399}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(angles_fixed = false, angles_start(displayUnit = "rad") , density(displayUnit = "kg/m3"), r = {0, 0, 1}, r_shape = {0, 0, 0}) annotation(Placement(visible = true, transformation(origin = {48, 38}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  input Real f1,f2,f3,t1,t2,t3;
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed1(r = {0, 0, 0.5}) annotation(Placement(visible = true, transformation(origin = {-58, -22}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  FMITLM.FMITLM_Interface_3D.FMITLMInterface1Dto3D fMITLMInterface1Dto3D1 annotation(Placement(visible = true, transformation(origin = {4, -8}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(n = {1, 0, 0}, phi(displayUnit = "rad", fixed = true, start = 3.1415 / 4), stateSelect = StateSelect.always, useAxisFlange = true) annotation(Placement(visible = true, transformation(origin = {-14, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper1(d = 100, phi_rel(displayUnit = "rad"))  annotation(Placement(visible = true, transformation(origin = {-18, 66}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(revolute1.axis, damper1.flange_b) annotation(Line(points = {{-14, 48}, {2, 48}, {2, 66}, {-8, 66}, {-8, 66}}));
  connect(damper1.flange_a, revolute1.support) annotation(Line(points = {{-28, 66}, {-42, 66}, {-42, 48}, {-20, 48}, {-20, 48}}));
  connect(revolute1.frame_a, world.frame_b) annotation(Line(points = {{-24, 38}, {-42, 38}, {-42, 38}, {-60, 38}, {-60, 38}, {-60, 38}}, color = {95, 95, 95}));
  connect(bodycylinder1.frame_a, revolute1.frame_b) annotation(Line(points = {{36, 38}, {-2, 38}, {-2, 38}, {-4, 38}}, color = {95, 95, 95}));
  connect(fixed1.frame_b, fMITLMInterface1Dto3D1.frame_a) annotation(Line(points = {{-48, -22}, {-30, -22}, {-30, -8}, {-6, -8}, {-6, -8}}, color = {95, 95, 95}));
  connect(fMITLMInterface1Dto3D1.frame_b, bodycylinder1.frame_b) annotation(Line(points = {{14.2009, -8.30374}, {42.2009, -8.30374}, {42.2009, -8.30374}, {70.2009, -8.30374}, {70.2009, 37.6963}, {60.2009, 37.6963}, {60.2009, 37.6963}, {60.2009, 37.6963}, {60.2009, 37.6963}}, color = {95, 95, 95}));
  fMITLMInterface1Dto3D1.f = f1;
  annotation(uses(Modelica(version = "3.2.2")));
end crane;
