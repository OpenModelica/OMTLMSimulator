model test
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyshape1(each r = {0,-0.5,0}, each r_CM = {0,0,0}, each m = 16.5, r_0.start = {0,-2,0}, each r_0.fixed = true, each v_0.fixed = true) annotation(Placement(visible = true, transformation(origin = {32.596,19.5576}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 0) annotation(Placement(visible = true, transformation(origin = {-76.9558,22.4738}, extent = {{-11.2864,-11.2864},{11.2864,11.2864}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.SpringDamperParallel springdamperparallel1(c = 1, s_unstretched = 1, d = 5) annotation(Placement(visible = true, transformation(origin = {-16.9965,22.5844}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Body body1(each r_CM = {0,0.5,0}, each m = 16.5) annotation(Placement(visible = true, transformation(origin = {84.4423,19.9146}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Sensors.CutForce cutForce annotation(Placement(visible = true, transformation(origin = {58,20}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  DummyTLM dummyTLM annotation(Placement(visible = true, transformation(origin = {74,-12}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(cutForce.force[2],dummyTLM.u) annotation(Line(visible = true, origin = {54,-5}, points = {{-4,14},{-4,-7},{8,-7}}, color = {0,0,127}));
  connect(cutForce.frame_b,body1.frame_a) annotation(Line(visible = true, origin = {71.325,19.957}, points = {{-3.325,0.043},{0.104,0.043},{0.104,-0.043},{3.117,-0.043}}));
  connect(bodyshape1.frame_b,cutForce.frame_a) annotation(Line(visible = true, origin = {45.986,19.779}, points = {{-3.39,-0.221},{0.689,-0.221},{0.689,0.221},{2.014,0.221}}));
  connect(springdamperparallel1.frame_b,bodyshape1.frame_a) annotation(Line(points = {{-6.99651,22.5844},{22.5844,22.5844},{22.5844,19.5576},{22.596,19.5576}}));
  connect(world.frame_b,springdamperparallel1.frame_a) annotation(Line(points = {{-65.6694,22.4738},{-27.4738,22.4738},{-26.9965,23.0501},{-26.9965,22.5844}}));
  annotation(Icon(coordinateSystem(extent = {{-100,-100},{100,100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {2,2})), Diagram(coordinateSystem(extent = {{-100,-100},{100,100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {2,2})));
end test;

