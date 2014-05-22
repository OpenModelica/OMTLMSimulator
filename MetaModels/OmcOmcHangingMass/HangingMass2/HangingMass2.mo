model HangingMass2
  Real x;
  inner Modelica.Mechanics.MultiBody.World world(g = 0, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-37.2454,36.0839}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D mbs_tlminterface1 annotation(Placement(visible = true, transformation(origin = {1.95573,48.8941}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Body body(r_CM = {0,0.5,0}, m = 16.5, r_0(start = {0,-2.5,0}, each fixed = true)) annotation(Placement(visible = true, transformation(origin = {58.5565,29.5007}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealOutput pos annotation(Placement(visible = true, transformation(origin = {112.456,-27.1362}, extent = {{-10,-10},{10,10}}, rotation = 0), iconTransformation(origin = {65.7625,-31.7811}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Sensors.AbsolutePosition absoluteposition1 annotation(Placement(visible = true, transformation(origin = {74.3366,-28.4129}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(absoluteposition1.frame_a,body.frame_a) annotation(Line(points = {{64.3366,-28.4129},{22.515,-28.4129},{48.5565,20.6998},{48.5565,29.5007}}));
  connect(absoluteposition1.r[2],pos) annotation(Line(points = {{85.3366,-28.4129},{104.878,-28.4129},{104.878,-28.3586},{104.878,-28.3586}}));
  connect(body.frame_a,mbs_tlminterface1.frame_a) annotation(Line(points = {{48.5565,29.5007},{24.6915,29.5007},{1.95573,29.8254},{1.95573,48.8941}}));
  x = 1;
  annotation(Diagram(coordinateSystem(extent = {{-148.5,-105},{148.5,105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5,5})));
end HangingMass2;

