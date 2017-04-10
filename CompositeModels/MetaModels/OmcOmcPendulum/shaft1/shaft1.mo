model shaft1
  inner Modelica.Mechanics.MultiBody.World world(enableAnimation = false, animateWorld = false, animateGravity = false, n = {-1, 0, 0}) annotation(Placement(visible = true, transformation(origin = {-71.9212, 77.3399}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Spherical spherical1 annotation(Placement(visible = true, transformation(origin = {-6.89655, 77.3399}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tlminterface3domc1 annotation(Placement(visible = true, transformation(origin = {26, -4}, extent = {{-19, -19}, {19, 19}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(r_shape = {0, 0, 0}, r = {0, 0, 0.5}) annotation(Placement(visible = true, transformation(origin = {48, 78}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
equation
  connect(spherical1.frame_b, bodycylinder1.frame_a) annotation(Line(points = {{5.10345, 77.3399}, {31.0345, 77.3399}, {31.0345, 78}, {36, 78}}));
  connect(bodycylinder1.frame_b, tlminterface3domc1.frame_a) annotation(Line(points = {{60, 78}, {79.9014, 78}, {79.9014, -5}, {13, -5}}));
  connect(world.frame_b, spherical1.frame_a) annotation(Line(points = {{-59.9212, 77.3399}, {-45.8128, 77.3399}, {-45.8128, 77.3399}, {-18.8966, 77.3399}}));
end shaft1;
