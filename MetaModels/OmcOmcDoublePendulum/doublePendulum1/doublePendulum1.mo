model doublePendulum1 "Simple double pendulum with two revolute joints and two bodies"
  inner Modelica.Mechanics.MultiBody.World world(enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(transformation(extent = {{-88,0},{-68,20}}, rotation = 0, origin = {0,0}), visible = true));
  TLM.TLM_Interface_3D.TLMInterface3D mbs_tlminterface1 annotation(Placement(visible = true, transformation(origin = {53.1062,7.41483}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.Rotational.Components.Damper damper(d = 0.1) annotation(Placement(visible = true, transformation(origin = {-46.8176,53.2064}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.BodyBox boxBody1(r = {0.25,0,0}, width = 0.06, r_0.fixed = true, r_0.start = {0,0,0}, v_0.fixed = true, v_0.start = {0,0,0}) annotation(Placement(visible = true, transformation(origin = {49.499,24.4289}, extent = {{-10,-10},{10,10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(useAxisFlange = true, phi(fixed = true), w(fixed = true)) annotation(Placement(visible = true, transformation(origin = {-46.4168,10.4008}, extent = {{-10,-10},{10,10}}, rotation = 0)));
equation
  connect(revolute1.support,damper.flange_a) annotation(Line(origin = {-35.858,30.826}, points = {{-52.4168,20.4008},{-51.969,31.022},{-60.618,34.2828},{-4.105,53.2064},{-56.8176,53.2064}}));
  connect(damper.flange_b,revolute1.axis) annotation(Line(origin = {-18.968,30.826}, points = {{-36.8176,53.2064},{5.23,53.2064},{-49.6798,32.2788},{-46.6266,28.4168},{-46.4168,20.4008}}));
  connect(world.frame_b,revolute1.frame_a) annotation(Line(origin = {-51.657,10}, points = {{-16.343,0},{-56.4168,10.4008}}, color = {95,95,95}, thickness = 0.5));
  connect(revolute1.frame_b,boxBody1.frame_a) annotation(Line(origin = {-3.959,10}, points = {{-36.4168,10.4008},{39.499,24.4289}}, color = {95,95,95}, thickness = 0.5));
  connect(boxBody1.frame_b,mbs_tlminterface1.frame_a) annotation(Line(points = {{59.499,24.4289},{46.493,24.4289},{46.493,7.01403},{46.493,7.01403}}));
  annotation(Icon(coordinateSystem(extent = {{-100,-100},{100,100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10,10}), graphics = {Rectangle(visible = true, origin = {-10.651,-8.377}, lineColor = {0,0,255}, fillColor = {128,128,128}, fillPattern = FillPattern.Solid, extent = {{-89.349,-91.623},{110.651,108.377}}, radius = 20),Text(visible = true, origin = {-15.021,-3.686}, textColor = {255,255,255}, extent = {{-81.612,-54.575},{111.654,66.091}}, textString = "EX")}), experiment(StopTime = 3), Documentation(info = "<html>
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

</HTML>"));
end doublePendulum1;

