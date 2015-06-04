model HangingMass2
  Modelica.Mechanics.MultiBody.Parts.Body body(r_CM = {0.5,0.5,0.5}, m = 16.5, r_0.start = {-2.5,-2.5,-2.5}, r_0.fixed = true, v_0.fixed = true) annotation(Placement(visible = true, transformation(origin = {72.193,27.592}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D TLMInterface annotation(Placement(visible = true, transformation(origin = {70,5}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 0, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-50,45}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(body.frame_a,TLMInterface.frame_a) annotation(Line(visible = true, origin = {60.849,16.144}, points = {{1.345,11.448},{-1.849,11.448},{-1.849,-11.448},{2.352,-11.448}}));
  annotation(Diagram(coordinateSystem(extent = {{-148.5,-105},{148.5,105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5,5})));
end HangingMass2;