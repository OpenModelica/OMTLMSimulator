model HangingMass1
  inner Modelica.Mechanics.MultiBody.World world(g = 0, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-122.517,30}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.SpringDamperParallel springDamperParallel(c = 1, s_unstretched = 1, d = 5) annotation(Placement(visible = true, transformation(origin = {-66.628,30}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(r = {0,-0.5,0}, r_CM = {0,0,0}, m = 16.5, r_0.start = {0,-2,0}, r_0.fixed = true, v_0.fixed = true) annotation(Placement(visible = true, transformation(origin = {27.971,31.89}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D TLMInterface annotation(Placement(visible = true, transformation(origin = {105,30}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.Ramp ramp(duration = 2, startTime = 6) annotation(Placement(visible = true, transformation(origin = {-106.539,-22.239}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.Constant const(k = 0) annotation(Placement(visible = true, transformation(origin = {-100,-85}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Sources.BooleanStep booleanStep(startTime = 6) annotation(Placement(visible = true, transformation(origin = {-106.539,-55}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Blocks.Logical.Switch switch1 annotation(Placement(visible = true, transformation(origin = {-51.506,-47.272}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.WorldForce force annotation(Placement(visible = true, transformation(origin = {8.344,-46.581}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(const.y,force.force[3]) annotation(Line(visible = true, origin = {-26.492,-65.79}, points = {{-62.508,-19.21},{19.836,-19.21},{19.836,19.21},{22.836,19.21}}, color = {0,0,127}));
  connect(const.y,force.force[1]) annotation(Line(visible = true, origin = {-45.333,-69.254}, points = {{-43.667,-15.746},{20.333,-15.746},{41.678,22.673}}, color = {0,0,127}));
  connect(switch1.y,force.force[2]) annotation(Line(visible = true, origin = {-14.368,-46.926}, points = {{-26.137,-0.346},{7.712,-0.346},{7.712,0.346},{10.712,0.346}}, color = {0,0,127}));
  connect(switch1.u3,const.y) annotation(Line(visible = true, origin = {-81.126,-70.136}, points = {{17.621,14.864},{-4.874,14.864},{-4.874,-14.864},{-7.874,-14.864}}, color = {0,0,127}));
  connect(switch1.u2,booleanStep.y) annotation(Line(visible = true, origin = {-86.031,-51.136}, points = {{22.525,3.864},{-6.508,3.864},{-6.508,-3.864},{-9.508,-3.864}}, color = {255,0,255}));
  connect(ramp.y,switch1.u1) annotation(Line(visible = true, origin = {-73.014,-30.756}, points = {{-22.525,8.517},{6.508,8.517},{6.508,-8.517},{9.508,-8.517}}, color = {0,0,127}));
  connect(force.frame_b,bodyShape.frame_a) annotation(Line(visible = true, origin = {18.154,-16.09}, points = {{0.191,-30.49},{3.191,-30.49},{3.191,-17.49},{-3.195,-17.49},{-3.195,47.98},{-0.183,47.98}}));
  connect(bodyShape.frame_b,TLMInterface.frame_a) annotation(Line(visible = true, origin = {66.846,28.833}, points = {{-28.875,3.057},{-1.846,1.167},{-1.846,-1.167},{31.355,0.863}}));
  connect(springDamperParallel.frame_b,bodyShape.frame_a) annotation(Line(visible = true, origin = {-2.185,30.945}, points = {{-54.443,-0.945},{17.143,-0.945},{17.143,0.945},{20.156,0.945}}));
  connect(world.frame_b,springDamperParallel.frame_a) annotation(Line(visible = true, origin = {-58.75,29.174}, points = {{-53.767,0.826},{-6.25,-0.826},{-6.25,0.826},{-17.878,0.826}}));
  annotation(Diagram(coordinateSystem(extent = {{-148.5,-105},{148.5,105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5,5})));
end HangingMass1;