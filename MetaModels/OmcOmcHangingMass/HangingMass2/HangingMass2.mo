model HangingMass2
  Real x;
  inner Modelica.Mechanics.MultiBody.World world(g = 9.81, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-40, 85}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D mbs_tlminterface1 annotation(Placement(visible = true, transformation(origin = {-30, 30}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Body body(r_CM = {0, -0.5, 0}, m = 16.5, r_0(start = {0, 0, 0}, each fixed = true)) annotation(Placement(visible = true, transformation(origin = {60, 30}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(body.frame_a, mbs_tlminterface1.frame_a) annotation(Line(points = {{50, 30}, {24.6915, 30}, {24.6915, 29.8254}, {-23, 29.8254}, {-23, 30}}));
  x = 1;
  annotation(Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
end HangingMass2;