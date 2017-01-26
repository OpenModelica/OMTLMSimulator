model HangingMass1
  input Real x;
  TLM.TLM_Interface_3D.TLMInterface3D mbs_tlminterface1 annotation(Placement(visible = true, transformation(origin = {20, -35}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(r = {0, -0.5, 0}, r_CM = {0, 0, 0}, m = 16.5, r_0(start = {0, 0, 0}, each fixed = true)) annotation(Placement(visible = true, transformation(origin = {-40, -35}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 9.81, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-115, -35}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(world.frame_b, bodyShape.frame_a) annotation(Line(points = {{-105, -35}, {-50, -35}}, color = {95, 95, 95}));
  connect(bodyShape.frame_b, mbs_tlminterface1.frame_a) annotation(Line(points = {{-30, -35}, {13, -35}}));
  annotation(Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
end HangingMass1;