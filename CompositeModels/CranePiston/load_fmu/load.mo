model load
  inner Modelica.Mechanics.MultiBody.World world annotation(Placement(visible = true, transformation(origin = {-56, 20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Body body1(m = 880, r_CM = {0, -0.3, 0})  annotation(Placement(visible = true, transformation(origin = {22, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  FMITLM.FMITLM_Interface_3D.FMITLMInterface3D fMITLMInterface3D1 annotation(Placement(visible = true, transformation(origin = {-16, 38}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));

  input Real f[3](start = zeros(3));
  input Real t[3](start = zeros(3));
equation
  fMITLMInterface3D1.f = f;
  fMITLMInterface3D1.t = t;
  
  connect(body1.frame_a, fMITLMInterface3D1.frame_a) annotation(Line(points = {{12, 38}, {-8, 38}, {-8, 38}, {-10, 38}}, color = {95, 95, 95}));
  annotation(uses(Modelica(version = "3.2.2")));
end load;
