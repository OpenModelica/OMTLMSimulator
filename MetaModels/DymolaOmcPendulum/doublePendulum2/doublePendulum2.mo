model doublePendulum2 "Simple double pendulum with two revolute joints and two bodies"
  Modelica.Mechanics.MultiBody.Parts.BodyBox boxBody1(angles_start(displayUnit = "rad"), density(displayUnit = "kg/m3"), enforceStates = false, r = {0.25, 0, 0}, r_0(fixed = true, start = {0, 0, 0}), v_0(fixed = true), width = 0.06) annotation(Placement(visible = true, transformation(extent = {{-10, 0}, {10, 20}}, rotation = 0)));
  FMITLM.FMITLM_Interface_3D.FMITLMInterface3D fMITLMInterface3D1(interfaceName = "fmitlm") annotation(Placement(visible = true, transformation(origin = {-38, 10}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  inner Modelica.Mechanics.MultiBody.World world(animateGravity = true, animateWorld = true, enableAnimation = false)  annotation(Placement(visible = true, transformation(extent = {{-88, 0}, {-68, 20}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Body body1 annotation(Placement(visible = true, transformation(origin = {82, 10}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute2 annotation(Placement(visible = true, transformation(extent = {{32, 0}, {52, 20}}, rotation = 0)));


  input Real f[3](start = zeros(3));
  input Real t[3](start = zeros(3));
equation
  fMITLMInterface3D1.f = f;
  fMITLMInterface3D1.t = t;

  connect(boxBody1.frame_b, revolute2.frame_a) annotation(Line(points = {{10, 10}, {32, 10}}, color = {95, 95, 95}, thickness = 0.5));
  connect(revolute2.frame_b, body1.frame_a) annotation(Line(points = {{52, 10}, {72, 10}, {72, 10}, {72, 10}}, color = {95, 95, 95}));
  connect(boxBody1.frame_a, fMITLMInterface3D1.frame_a) annotation(Line(points = {{-10, 10}, {-31, 10}}, color = {95, 95, 95}));
  annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, origin = {-10.651, -8.377}, lineColor = {0, 0, 255}, fillColor = {128, 128, 128}, fillPattern = FillPattern.Solid, extent = {{-89.349, -91.623}, {110.651, 108.377}}, radius = 20), Text(visible = true, origin = {-15.021, -3.686}, textColor = {255, 255, 255}, extent = {{-81.612, -54.575}, {111.654, 66.091}}, textString = "EX")}), experiment(StopTime = 3), Documentation(info = "<html>
<p>
This example demonstrates that by using joint and body
elements animation is automatically available. Also the revolute
joints are animated. Note, that animation of every component
can be switched of by setting the first parameter <b>animation</b>
to <b>false</b> or by setting <b>enableAnimation</b> in the <b>world</b>
object to <b>false</b> to switch off animation of all components.
</p>

<table border=0 cellspacing=0 cellpadding=0><tr><td valign=\"top\">
<IMG src=\"modelica://Modelica/Resources/Images/Mechanics/MultiBody/Examples/Elementary/DoublePendulum.png\"
ALT=\"model Examples.Elementary.DoublePendulum\">
</td></tr></table>

</HTML>"), uses(Modelica(version = "3.2.2")));
end doublePendulum2;
