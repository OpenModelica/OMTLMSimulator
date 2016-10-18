model HangingMass2
  Modelica.Mechanics.MultiBody.Parts.Body body(r_CM = {0,0.5,0}, m = 16.5, r_0.start = {0,-2.5,0}, r_0.fixed = true, v_0.fixed = true) annotation(Placement(visible = true, transformation(origin = {72.193,27.592}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D TLMInterface annotation(Placement(visible = true, transformation(origin = {57.905,-47.798}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(g = 0, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-50,45}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.BooleanStep booleanStep(startTime = 2) annotation(Placement(visible = true, transformation(origin = {-90,-12.214}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.Constant const1(k = 0) annotation(Placement(visible = true, transformation(origin = {-125,-32.128}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Logical.Switch switch1 annotation(Placement(visible = true, transformation(origin = {-21.544,-10}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.WorldForce force annotation(Placement(visible = true, transformation(origin = {15,47.247}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.Ramp ramp(duration = 2, offset = 0, startTime = 2, height = 1) annotation(Placement(visible = true, transformation(origin = {-125,20}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(const1.y,force.force[3]) annotation(Line(visible = true, origin = {-27.75,7.559}, points = {{-86.25,-39.688},{27.75,-39.688},{27.75,39.688},{30.75,39.688}}, color = {0,0,127}));
  connect(const1.y,force.force[1]) annotation(Line(visible = true, origin = {-27.75,7.559}, points = {{-86.25,-39.688},{27.75,-39.688},{27.75,39.688},{30.75,39.688}}, color = {0,0,127}));
  connect(switch1.y,force.force[2]) annotation(Line(visible = true, origin = {-1.886,18.623}, points = {{-8.658,-28.623},{1.886,-28.623},{1.886,28.623},{4.886,28.623}}, color = {0,0,127}));
  connect(ramp.y,switch1.u1) annotation(Line(visible = true, origin = {-55.158,9}, points = {{-58.842,11},{18.614,11},{18.614,-11},{21.614,-11}}, color = {0,0,127}));
  connect(body.frame_a,force.frame_b) annotation(Line(visible = true, origin = {35.798,37.42}, points = {{26.395,-9.828},{-7.798,-9.828},{-7.798,9.828},{-10.798,9.828}}));
  connect(booleanStep.y,switch1.u2) annotation(Line(visible = true, origin = {-46.408,-11.107}, points = {{-32.592,-1.107},{9.864,-1.107},{9.864,1.107},{12.864,1.107}}, color = {255,0,255}));
  connect(const1.y,switch1.u3) annotation(Line(visible = true, origin = {-55.158,-25.064}, points = {{-58.842,-7.064},{18.614,-7.064},{18.614,7.064},{21.614,7.064}}, color = {0,0,127}));
  connect(body.frame_a,TLMInterface.frame_a) annotation(Line(visible = true, origin = {50.777,-10.255}, points = {{11.416,37.847},{-5.872,37.847},{-5.872,-37.847},{0.329,-37.847}}));
  annotation(Diagram(coordinateSystem(extent = {{-148.5,-105},{148.5,105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5,5})));
end HangingMass2;