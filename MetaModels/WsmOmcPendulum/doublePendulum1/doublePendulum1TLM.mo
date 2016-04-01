model doublePendulum1 "Simple double pendulum with two revolute joints and two bodies"
  inner Modelica.Mechanics.MultiBody.World world(enableAnimation = false, animateWorld = false, animateGravity = false) annotation(Placement(transformation(extent = {{-88,0},{-68,20}}, rotation = 0, origin = {0,0}), visible = true));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(useAxisFlange = true, phi(fixed = true), w(fixed = true)) annotation(Placement(transformation(extent = {{-48,0},{-28,20}}, rotation = 0, origin = {12.686,0}), visible = true));
  Modelica.Mechanics.Rotational.Components.Damper damper(d = 0.1) annotation(Placement(transformation(extent = {{-48,40},{-28,60}}, rotation = 0, origin = {8,11.961}), visible = true));
  Modelica.Mechanics.MultiBody.Parts.BodyBox boxBody1(r = {0.25,0,0}, width = 0.06, r_0.fixed = true, r_0.start = {0,0,0}, v_0.fixed = true, v_0.start = {0,0,0}) annotation(Placement(visible = true, transformation(origin = {15.362,-0}, extent = {{-10,0},{10,20}}, rotation = 0)));
  TLM.TLM_Interface_3D.TLMInterface3D TLMInterface annotation(Placement(visible = true, transformation(origin = {65.055,10}, extent = {{-14.945,-14.945},{14.945,14.945}}, rotation = 0)));
equation
  connect(boxBody1.frame_b,TLMInterface.frame_a) annotation(Line(visible = true, origin = {43.619,9.773}, points = {{-18.257,0.227},{3.491,0.227},{3.491,-0.227},{11.275,-0.227}}));
  connect(revolute1.frame_b,boxBody1.frame_a) annotation(Line(visible = true, origin = {-4.976,10}, points = {{-10.338,0},{10.338,-0}}, color = {95,95,95}, thickness = 0.5));
  connect(damper.flange_b,revolute1.axis) annotation(Line(points = {{0.926,23.887},{3.926,23.887},{3.926,-14.849},{-4.388,-14.849},{-4.388,-18.074}}, visible = true, origin = {-20.926,38.074}));
  connect(revolute1.support,damper.flange_a) annotation(Line(points = {{6.502,-18.074},{6.502,-14.849},{-5.409,-14.849},{-5.409,23.887},{-2.184,23.887}}, visible = true, origin = {-37.816,38.074}));
  connect(world.frame_b,revolute1.frame_a) annotation(Line(points = {{-16.343,0},{16.343,0}}, color = {95,95,95}, thickness = 0.5, visible = true, origin = {-51.657,10}));
  annotation(experiment(StopTime = 3), Documentation(info = "<html>
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