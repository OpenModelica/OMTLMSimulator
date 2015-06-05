model shaft1
  inner Modelica.Mechanics.MultiBody.World world(n = {-1,0,0}, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-71.9212,77.3399}, extent = {{-12,-12},{12,12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Spherical spherical1 annotation(Placement(visible = true, transformation(origin = {-6.89655,77.3399}, extent = {{-12,-12},{12,12}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(r_shape = {0,0,0}, r = {0,-0.1,0}) annotation(Placement(visible = true, transformation(origin = {49.2611,77.8325}, extent = {{-12,-12},{12,12}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tlminterface3domc1 annotation(Placement(visible = true, transformation(origin = {32.5123,2.46305}, extent = {{-12,-12},{12,12}}, rotation = 0)));
equation
  connect(bodycylinder1.frame_b,tlminterface3domc1.frame_a) annotation(Line(points = {{61.2611,77.8325},{79.9014,77.8325},{79.9014,2.09857},{24.3534,2.09857}}));
  connect(spherical1.frame_b,bodycylinder1.frame_a) annotation(Line(points = {{5.10345,77.3399},{31.0345,77.3399},{31.0345,77.8325},{37.2611,77.8325}}));
  connect(world.frame_b,spherical1.frame_a) annotation(Line(points = {{-59.9212,77.3399},{-45.8128,77.3399},{-45.8128,77.3399},{-18.8966,77.3399}}));
end shaft1;

