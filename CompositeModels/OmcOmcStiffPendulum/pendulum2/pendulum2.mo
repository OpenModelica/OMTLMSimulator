model pendulum2
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(r_shape = {0, 0, 0}, r = {0, 0, 0.5}) annotation(Placement(visible = true, transformation(origin = {9.37115, 59.4328}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-76.4488, 81.381}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tlminterface3domc1 annotation(Placement(visible = true, transformation(origin = {-41.1837, 58.693}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
equation
  connect(tlminterface3domc1.frame_a, bodycylinder1.frame_a) annotation(Line(points = {{-49.3426, 58.3285}, {-2.46609, 58.3285}, {-2.46609, 59.4328}, {-2.62885, 59.4328}}));
end pendulum2;
