model load
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(interfaceName = "tlm1")  annotation(Placement(visible = true, transformation(origin = {-18, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = false, animateWorld = false, enableAnimation = false)  annotation(Placement(visible = true, transformation(origin = {-56, 20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Body body1(animation = false,m = 880, r_CM = {0, -0.3, 0})  annotation(Placement(visible = true, transformation(origin = {22, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(tLMInterface3D1.frame_a, body1.frame_a) annotation(Line(points = {{-10, 38}, {12, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end load;