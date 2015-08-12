model shaft2
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodycylinder1(r_shape = {0, 0, 0}, r = {0, -0.1, 0}) annotation(Placement(visible = true, transformation(origin = {9.37115, 59.4328}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(n = {-1, 0, 0}, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-76.4488, 81.381}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tlminterface3domc2(interfaceName = "tlm") annotation(Placement(visible = true, transformation(origin = {-38, 60}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(tlminterface3domc2.frame_a, bodycylinder1.frame_a) annotation(Line(points = {{-45, 60}, {-3, 60}, {-3, 59}}, color = {95, 95, 95}));
end shaft2;