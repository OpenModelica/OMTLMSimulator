model load
  Modelica.Mechanics.MultiBody.Parts.Body body1(angles_start(displayUnit = "rad"), enforceStates = true, m = 1, r_CM = {0, 0, 0})  annotation(Placement(visible = true, transformation(origin = {24, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(debugFlg = true)  annotation(Placement(visible = true, transformation(origin = {-24, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  inner Modelica.Mechanics.MultiBody.World world(n = {0, 0, -1})  annotation(Placement(visible = true, transformation(origin = {-20, -10}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(body1.frame_a, tLMInterface3D1.frame_a) annotation(Line(points = {{14, 38}, {-16, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end load;
