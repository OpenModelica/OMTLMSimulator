model pendulum1 "Simple double pendulum with two revolute joints and two bodies"
  inner Modelica.Mechanics.MultiBody.World world(enableAnimation = false, animateWorld = false, animateGravity = false) annotation(
    Placement(visible = true, transformation(origin = {24, 14}, extent = {{-88, 0}, {-68, 20}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper(d = 0.1) annotation(
    Placement(visible = true, transformation(origin = {-2.8176, 55.2064}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyBox boxBody1(r = {0.25, 0, 0}, width = 0.06, r_0.fixed = true, r_0.start = {0, 0, 0}, v_0.fixed = true, v_0.start = {0, 0, 0}) annotation(
    Placement(visible = true, transformation(origin = {49.499, 24.4289}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(useAxisFlange = true, phi(fixed = true), w(fixed = true)) annotation(
    Placement(visible = true, transformation(origin = {-0.416801, 24.4008}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  FMITLM.FMITLM_Interface_3D.FMITLMInterface3D fmi annotation(
    Placement(visible = true, transformation(origin = {82, 24}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
    input Real f[3];
    input Real t[3];
equation
  fmi.f = f;
  fmi.t = t;
  connect(revolute1.support, damper.flange_a) annotation(
    Line(points = {{-6, 34}, {-6, 34}, {-6, 44}, {-20, 44}, {-20, 56}, {-12, 56}, {-12, 56}}));
  connect(damper.flange_b, revolute1.axis) annotation(
    Line(points = {{7, 55}, {16, 55}, {16, 44}, {0, 44}, {0, 34}}));
  connect(world.frame_b, revolute1.frame_a) annotation(
    Line(points = {{-44, 24}, {-10, 24}}, color = {95, 95, 95}, thickness = 0.5));
  connect(revolute1.frame_b, boxBody1.frame_a) annotation(
    Line(points = {{10, 24}, {39, 24}}, color = {95, 95, 95}, thickness = 0.5));
  connect(boxBody1.frame_b, fmi.frame_a) annotation(
    Line(points = {{60, 24}, {75, 24}}, color = {95, 95, 95}));
  annotation(
    Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, origin = {-10.651, -8.377}, lineColor = {0, 0, 255}, fillColor = {128, 128, 128}, fillPattern = FillPattern.Solid, extent = {{-89.349, -91.623}, {110.651, 108.377}}, radius = 20), Text(visible = true, origin = {-15.021, -3.686}, textColor = {255, 255, 255}, extent = {{-81.612, -54.575}, {111.654, 66.091}}, textString = "EX")}),
    experiment(StopTime = 3),
    Documentation(info = "<html>
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

</HTML>"),
    Diagram);
end pendulum1;