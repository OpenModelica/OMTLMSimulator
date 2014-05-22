model HangingMass1
  input Real x;
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(r = {0,-0.5,0}, r_CM = {0,0,0}, m = 16.5, r_0(start = {0,-2,0}, each fixed = true)) annotation(Placement(visible = true, transformation(origin = {51.0877,-39.6741}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D mbs_tlminterface1 annotation(Placement(visible = true, transformation(origin = {110.256,-43.2712}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.SpringDamperParallel springDamperParallel(c = 1, s_unstretched = 1, d = 5) annotation(Placement(visible = true, transformation(origin = {1.47806,-34.5402}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 0, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-116.517,-32.6337}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(world.frame_b,springDamperParallel.frame_a) annotation(Line(origin = {-58.75,29.174}, points = {{-106.517,-32.6337},{-6.25,-32.6337},{-8.52194,0.826},{-8.52194,-34.5402}}));
  connect(springDamperParallel.frame_b,bodyShape.frame_a) annotation(Line(points = {{11.4781,-34.5402},{41.3155,-34.5402},{41.3155,-39.6741},{41.0877,-39.6741}}));
  connect(bodyShape.frame_b,mbs_tlminterface1.frame_a) annotation(Line(points = {{61.0877,-39.6741},{103.166,-39.6741},{103.166,-44.0047},{103.166,-44.0047}}));
  annotation(Diagram(coordinateSystem(extent = {{-148.5,-105},{148.5,105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5,5})));
end HangingMass1;

