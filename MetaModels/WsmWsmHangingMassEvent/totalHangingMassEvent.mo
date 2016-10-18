model test
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyshape1(each r = {0,-0.5,0}, each r_CM = {0,0,0}, each m = 16.5, r_0.start = {0,-2,0}, each r_0.fixed = true, each v_0.fixed = true) annotation(Placement(visible = true, transformation(origin = {36,22}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 0) annotation(Placement(visible = true, transformation(origin = {-79.286,-43.286}, extent = {{-11.286,-11.286},{11.286,11.286}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Body body1(each r_CM = {0,0.5,0}, m = 16.5, r_0.start = {0,-2.5,0}) annotation(Placement(visible = true, transformation(origin = {86,24}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.WorldForce force annotation(Placement(visible = true, transformation(origin = {32,54}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Logical.Switch switch1 annotation(Placement(visible = true, transformation(origin = {-8,48}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.Constant const3(k = 0) annotation(Placement(visible = true, transformation(origin = {-84,2}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.BooleanStep booleanStep(startTime = 2) annotation(Placement(visible = true, transformation(origin = {-38,54}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.Ramp ramp(duration = 2, startTime = 2, height = 1) annotation(Placement(visible = true, transformation(origin = {-54,88}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.SpringDamperParallel springdamperparallel1(c = 1, d = 5, s_unstretched = 1) annotation(Placement(visible = true, transformation(origin = {-2,-44}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.Ramp ramp1(duration = 2, height = 1, startTime = 6) annotation(Placement(visible = true, transformation(origin = {-184,-40}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.BooleanStep booleanStep1(startTime = 2) annotation(Placement(visible = true, transformation(origin = {-168,-74}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.Constant const4(k = 0) annotation(Placement(visible = true, transformation(origin = {-180,-122}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Logical.Switch switch2 annotation(Placement(visible = true, transformation(origin = {-138,-80}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.WorldForce force1 annotation(Placement(visible = true, transformation(origin = {-98,-74}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(force1.frame_b,bodyshape1.frame_a) annotation(Line(visible = true, origin = {-4.006,-26}, points = {{-83.994,-48},{26.994,-48},{26.994,48},{30.006,48}}));
  connect(ramp1.y,switch2.u1) annotation(Line(visible = true, origin = {-157.25,-56}, points = {{-15.75,16},{4.25,16},{4.25,-16},{7.25,-16}}, color = {0,0,127}));
  connect(booleanStep1.y,switch2.u2) annotation(Line(visible = true, origin = {-153.25,-77}, points = {{-3.75,3},{0.25,3},{0.25,-3},{3.25,-3}}, color = {255,0,255}));
  connect(switch2.y,force1.force[2]) annotation(Line(visible = true, origin = {-115.75,-77}, points = {{-11.25,-3},{2.75,-3},{2.75,3},{5.75,3}}, color = {0,0,127}));
  connect(const4.y,switch2.u3) annotation(Line(visible = true, origin = {-156.25,-105}, points = {{-12.75,-17},{3.25,-17},{3.25,17},{6.25,17}}, color = {0,0,127}));
  connect(const4.y,force1.force[1]) annotation(Line(visible = true, origin = {-126.25,-98}, points = {{-42.75,-24},{13.25,-24},{13.25,24},{16.25,24}}, color = {0,0,127}));
  connect(const4.y,force1.force[3]) annotation(Line(visible = true, origin = {-126.25,-98}, points = {{-42.75,-24},{13.25,-24},{13.25,24},{16.25,24}}, color = {0,0,127}));
  connect(const3.y,force.force[3]) annotation(Line(visible = true, origin = {-4.75,28}, points = {{-68.25,-26},{21.75,-26},{21.75,26},{24.75,26}}, color = {0,0,127}));
  connect(const3.y,force.force[1]) annotation(Line(visible = true, origin = {-4.75,28}, points = {{-68.25,-26},{21.75,-26},{21.75,26},{24.75,26}}, color = {0,0,127}));
  connect(switch1.y,force.force[2]) annotation(Line(visible = true, origin = {14.25,51}, points = {{-11.25,-3},{2.75,-3},{2.75,3},{5.75,3}}, color = {0,0,127}));
  connect(springdamperparallel1.frame_b,bodyshape1.frame_a) annotation(Line(visible = true, origin = {19.994,-11}, points = {{-11.994,-33},{2.994,-33},{2.994,33},{6.006,33}}));
  connect(world.frame_b,springdamperparallel1.frame_a) annotation(Line(visible = true, origin = {-29.006,-43.643}, points = {{-38.994,0.357},{10.994,0.357},{10.994,-0.357},{17.006,-0.357}}));
  connect(ramp.y,switch1.u1) annotation(Line(visible = true, origin = {-27.25,72}, points = {{-15.75,16},{4.25,16},{4.25,-16},{7.25,-16}}, color = {0,0,127}));
  connect(force.frame_b,body1.frame_a) annotation(Line(visible = true, origin = {57.006,39}, points = {{-15.006,15},{6.994,15},{6.994,-15},{18.994,-15}}));
  connect(booleanStep.y,switch1.u2) annotation(Line(visible = true, origin = {-23.25,51}, points = {{-3.75,3},{0.25,3},{0.25,-3},{3.25,-3}}, color = {255,0,255}));
  connect(const3.y,switch1.u3) annotation(Line(visible = true, origin = {-34.75,21}, points = {{-38.25,-19},{11.75,-19},{11.75,19},{14.75,19}}, color = {0,0,127}));
  connect(bodyshape1.frame_b,body1.frame_a) annotation(Line(visible = true, origin = {66.994,23}, points = {{-20.994,-1},{5.994,-1},{5.994,1},{9.006,1}}));
  annotation(Icon(coordinateSystem(extent = {{-100,-100},{100,100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {2,2})), Diagram(coordinateSystem(extent = {{-100,-100},{100,100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {2,2})));
end test;

