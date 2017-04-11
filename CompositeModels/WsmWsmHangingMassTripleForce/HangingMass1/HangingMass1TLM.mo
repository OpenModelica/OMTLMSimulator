model HangingMass1
  inner Modelica.Mechanics.MultiBody.World world(g = 0, enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(visible = true, transformation(origin = {-80,28.348}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.SpringDamperParallel springDamperParallel(c = 1, s_unstretched = 1, d = 5) annotation(Placement(visible = true, transformation(origin = {-25,30}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(r = {-0.5,-0.5,-0.5}, r_CM = {0,0,0}, m = 16.5, r_0.start = {-2,-2,-2}, r_0.fixed = true, v_0.fixed = true) annotation(Placement(visible = true, transformation(origin = {36.664,30}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D TLMInterface annotation(Placement(visible = true, transformation(origin = {97.518,27.97}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(springDamperParallel.frame_b,bodyShape.frame_a) annotation(Line(visible = true, origin = {5.832,30}, points = {{-20.832,0},{20.832,0}}));
  connect(bodyShape.frame_b,TLMInterface.frame_a) annotation(Line(visible = true, origin = {66.846,28.833}, points = {{-20.182,1.167},{-1.846,1.167},{-1.846,-1.167},{23.873,-1.167}}));
  connect(world.frame_b,springDamperParallel.frame_a) annotation(Line(visible = true, origin = {-58.75,29.174}, points = {{-11.25,-0.826},{-6.25,-0.826},{-6.25,0.826},{23.75,0.826}}));
  annotation(Diagram(coordinateSystem(extent = {{-148.5,-105},{148.5,105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5,5})));
end HangingMass1;