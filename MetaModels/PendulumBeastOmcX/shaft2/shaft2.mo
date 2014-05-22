model shaft2
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(r_shape = {0,0,0}, r = {0,-0.1,0}) annotation(Placement(visible = true, transformation(origin = {9.37115,59.4328}, extent = {{-12,-12},{12,12}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(n = {-1,0,0}, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-76.4488,81.381}, extent = {{-12,-12},{12,12}}, rotation = 0)));
  TLM.TLM_Interface_3D.OMC.TLMInterface3DOMC tlminterface3domc1 annotation(Placement(visible = true, transformation(origin = {-41.1837,58.693}, extent = {{-12,-12},{12,12}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tlminterface3domc2(interfaceName = "tlm2") annotation(Placement(visible = true, transformation(origin = {60,60}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(bodycylinder1.frame_b,tlminterface3domc2.frame_a) annotation(Line(points = {{21.3712,59.4328},{53.2033,59.4328},{53.2033,60.4457},{53.2033,60.4457}}));
  connect(tlminterface3domc1.frame_a,bodycylinder1.frame_a) annotation(Line(points = {{-49.3426,58.3285},{-2.46609,58.3285},{-2.46609,59.4328},{-2.62885,59.4328}}));
end shaft2;

